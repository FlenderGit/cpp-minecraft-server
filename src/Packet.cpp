#include "Packet.hpp"

#include "Logger.hpp"
namespace packet
{

    int ClientPacket::load(int socket)
    {
        int bytesReceived = read(socket, this->bytes, PACKET_BUFFER_SIZE);
        if (bytesReceived == 0)
            Logger::log(INFO, "Client disconnected");
        if (bytesReceived == SOCKET_ERROR)
            Logger::log(ERR, "Failed to read from socket: " + std::string(strerror(errno)));

        int length = readVarInt();

        if (length != bytesReceived - this->position)
        {
            Logger::log(ERR, "Invalid packet length");
            Logger::log(ERR, "Expected: " + std::to_string(length) + " Got: " + std::to_string(bytesReceived - this->position));
            return 0;
        }
        this->id = readVarInt();
        this->length = length;

        return 1;
    }

    int ClientPacket::readVarInt()
    {
        int value = 0;
        int position = 0;
        char current_byte;
        Logger::log(INFO, "Position: " + std::to_string(this->position));
        for (;;) {
            current_byte = this->bytes[this->position + position];
            value |= (current_byte & SEGMENT_BITS) << position;
            if ((current_byte & CONTINUE_BIT) == 0)
                break;
            position += 7;
        }
        this->position += position / 7 + 1;
        return value;
    }

    std::string ClientPacket::readString()
    {
        int length = readVarInt();
        std::string str(this->bytes + this->position, length);
        this->position += length;
        return str;
    }

    short ClientPacket::readShort()
    {
        short value = (this->bytes[this->position] << 8) | this->bytes[this->position + 1];
        this->position += 2;
        return value;
    }

    unsigned short ClientPacket::readUnsignedShort()
    {
        unsigned short value = (this->bytes[this->position] << 8) | this->bytes[this->position + 1];
        this->position += 2;
        return value;
    }

    template <typename... Types>
    ResponsePacket::ResponsePacket(int id, Type<Types>... types)
    {
        ByteList bytes;
        // Utilisation d'une expression lambda pour parcourir les types variadiques
        auto addBytes = [&](auto &type)
        {
            bytes.insert(type.write());
        };

        // Applique la fonction à chaque élément de types...
        (addBytes(types), ...);

        this->id = id;
        this->length = bytes.size();
        this->bytes = bytes;
    }

    template <>
    ByteList VarInt::write()
    {
        ByteList bytes;
        int value = this->data;
        while (true)
        {
            if ((value & ~SEGMENT_BITS) == 0)
            {
                bytes.push_back(value);
                break;
            }
            bytes.push_back((value & SEGMENT_BITS) | CONTINUE_BIT);
            value >>= 7;
        }
        return bytes;
    }

    template <>
    ByteList String::write()
    {
        ByteList bytes;
        ByteList strBytes(this->data.begin(), this->data.end());
        bytes.push_back(strBytes.size());
        bytes.insert(bytes.end(), strBytes.begin(), strBytes.end());
        return bytes;
    }
}

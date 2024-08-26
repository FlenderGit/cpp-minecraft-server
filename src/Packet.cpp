#include "Packet.hpp"

#include "Logger.hpp"
namespace packet
{

    void Packet::setLength(int length)
    {
        this->length = length;
    }

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
            Logger::log(ERR, "Expected: " + std::to_string(length) + " Got: " + std::to_string(bytesReceived));
            return 0;
        }
        this->length = length;
        this->id = readVarInt();
        return 1;
    }

    int ClientPacket::readVarInt()
    {
        int value = 0;
        int position = this->position;
        char current_byte;
        do
        {
            current_byte = this->bytes[position];
            int valuePart = (current_byte & SEGMENT_BITS);
            value |= (valuePart << (7 * position));
            position++;
        } while ((current_byte & CONTINUE_BIT) != 0);
        this->position = position;
        return value;
    }

    std::string ClientPacket::readString()
    {
        int length = readVarInt();
        std::string str( this->bytes + this->position, length );
        this->position += length;
        return str;
    }

    short ClientPacket::readShort()
    {
        short value = (this->bytes[this->position] << 8) | this->bytes[this->position + 1];
        this->position += 2;
        return value;
    }

    template <typename... Types>
    ResponsePacket::ResponsePacket(int id, Type<Types>... types) : id(id)
    {
        ByteList bytes;
        for (Type type : types)
            bytes.insert(type.write());
        this->length = bytes.size();
        this->bytes = bytes;
    }

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

    ByteList String::write()
    {
        ByteList bytes;
        ByteList strBytes(this->data.begin(), this->data.end());
        bytes.push_back(strBytes.size());
        bytes.insert(bytes.end(), strBytes.begin(), strBytes.end());
        return bytes;
    }
}

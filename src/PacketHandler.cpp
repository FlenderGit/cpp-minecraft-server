#include "PacketHandler.hpp"

#include "string.h"

namespace packet
{

    int PacketHandler::handle(ClientPacket *packet)
    {
        // Log the packet content as hex
        // Logger::log(INFO, "Packet ID: " + std::to_string(packet->id));
        // Logger::log(INFO, "Packet length: " + std::to_string(packet->length));

        switch (client->state)
        {
        case Handshake:     handleHandshake(packet);    break;
        case Login:         handleLogin(packet);        break;
        case Play:          handlePlay(packet);         break;
        case Configuration: handleConfiguration(packet);break;
        default:
            Logger::log(ERR, "Invalid state");
            return 0;
        }
        return 1;
    }

    inline int PacketHandler::handleHandshake(ClientPacket *packet)
    {
        struct
        {
            int protocolVersion;
            std::string serverAddress;
            short serverPort;
            int nextState;
        } p = {packet->readVarInt(), packet->readString(), packet->readShort(), packet->readVarInt()};

        Logger::log(INFO, "Protocol version: " + std::to_string(p.protocolVersion));
        Logger::log(INFO, "Server address: " + p.serverAddress);
        Logger::log(INFO, "Server port: " + std::to_string(p.serverPort));

        switch (p.nextState)
        {
        case 1: client->state = Status; break;
        case 2: client->state = Login;  break;
        default:
            Logger::log(ERR, "Invalid next state");
            return 0;
        }

        return 1;
    }

    inline int PacketHandler::handleLogin(ClientPacket *packet)
    {
        switch (packet->getID())
        {
        case 0x00: {
            struct      { std::string username; std::string uuid; }
            p =    {packet->readString(), packet->readString()};
            Logger::log(INFO, "Username: " + p.username);
            Logger::log(INFO, "UUID: " + p.uuid);
            sendPacket(ResponsePacket(0x02, String("Jean"), String("1234")));
            break;
        }
        default:
            Logger::log(ERR, "Invalid packet ID");
            return 0;
        }
        return 1;
    }

    inline int PacketHandler::handleStatus(ClientPacket *packet)
    {
        return 1;
    }

    void PacketHandler::sendPacket(packet::ResponsePacket packet)
    {
        // Maybe more with the length of the ID and the length
        send(client->socket, packet.getBuffer(), packet.getLength(), 0);
    }

}
#include "PacketHandler.hpp"

#include "string.h"

namespace packet
{

    int PacketHandler::handle(ClientPacket *packet)
    {
        // Log the packet content as hex
        Logger::log(INFO, "Packet ID: " + std::to_string(packet->getID()));
        Logger::log(INFO, "Packet length: " + std::to_string(packet->getLength()));
        // Print each byte of the packet as hex
        char *bytes = packet->getBuffer();
        for (int i = 0; i < packet->getLength(); i++) {
            printf("%02X ", bytes[i]);
        }
        printf("\n");

        switch (client->state)
        {
        case Handshake:     handleHandshake(packet);    break;
        case Login:         handleLogin(packet);        break;
        case Play:          handlePlay(packet);         break;
        case Configuration: handleConfiguration(packet);break;
        default:
            Logger::log(ERR, "Invalid state");
            return R_ERR;
        }
        return R_OK;
    }

    inline int PacketHandler::handleHandshake(ClientPacket *packet)
    {
        Logger::log(INFO, "Handshake packet");
        struct
        {
            int protocolVersion;
            std::string serverAddress;
            unsigned short serverPort;
            int nextState;
        } p = {packet->readVarInt(), packet->readString(), packet->readUnsignedShort(), packet->readVarInt()};
        Logger::log(INFO, "Protocol version: " + std::to_string(p.protocolVersion));
        Logger::log(INFO, "Server: " + p.serverAddress + ":" + std::to_string(p.serverPort));
        Logger::log(INFO, "Next state: " + std::to_string(p.nextState));
        
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
            //sendPacket(ResponsePacket(0x02, String("Jean"), String("1234")));
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
        packet->readVarInt();
        return 1;
    }

    inline int PacketHandler::handlePlay(ClientPacket *packet)
    {
        packet->readVarInt();
        return 1;
    }

    inline int PacketHandler::handleConfiguration(ClientPacket *packet)
    {
        packet->readVarInt();
        return 1;
    }

    void PacketHandler::sendPacket(packet::ResponsePacket packet)
    {
        // Maybe more with the length of the ID and the length
        send(client->socket, packet.getBuffer(), packet.getLength(), 0);
    }

}
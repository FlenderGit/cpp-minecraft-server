#pragma once

class Client;

#include "Client.hpp"
#include "Packet.hpp"

namespace packet {

class PacketHandler {
    public:
        Client *client;

        PacketHandler(Client *client): client(client) {}
        int handle(ClientPacket *packet);

    private:
    
        int handleHandshake(ClientPacket *packet);
        int handlePlay(ClientPacket *packet);
        int handleConfiguration(ClientPacket *packet);
        int handleStatus(ClientPacket *packet);
        int handleLogin(ClientPacket *packet);

        void sendPacket(ResponsePacket packet);
};

}
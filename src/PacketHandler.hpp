#pragma once

#include "Client.hpp"
#include "Packet.hpp"

class PacketHandler {
    public:
        Client* client;

        PacketHandler(Client* client);

        int handle(Packet *packet);
        bool isValid(Packet *packet);
};
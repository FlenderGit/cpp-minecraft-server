#pragma once

class Client;

#include "Client.hpp"
#include "Packet.hpp"


class PacketHandler {
    public:
        Client *client;
        Packet *currentPacket;

        PacketHandler(Client *client);

        int loadPacket(Packet *packet);
        int handle();

    private:

        static const int SEGMENT_BITS = 0x7F;
        static const int CONTINUE_BIT = 0x80;

        int readVarInt();
        long readVarLong();
        std::string readString();
        short readShort();

        int handleHandshake();
};
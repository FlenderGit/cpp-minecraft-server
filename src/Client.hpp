#pragma once

#include <thread>

#include "Packet.hpp"
#include "PacketHandler.hpp"
#include "Logger.hpp"
#include "errno.h"


enum State {
    Handshake,
    Status,
    Login,
    Play,
    Configuration,
};

class Client {

    public:
        int socket;
        State state;
        packet::PacketHandler *packetHandler;

        Client(int socket);

        int start();

    private:
        int handleConnection();

};
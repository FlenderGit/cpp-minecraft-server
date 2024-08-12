#pragma once

#include <thread>

#ifdef _WIN32
#include <io.h>
#define access _access

#else
#include <unistd.h>
#endif

#include "Logger.hpp"
#include "Packet.hpp"
#include "PacketHandler.hpp"

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
        PacketHandler *packetHandler;

        Client(int socket);

        int start();

    private:
        int handleConnection();

};
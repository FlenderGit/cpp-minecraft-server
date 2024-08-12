#pragma once

#include <thread>

#ifdef _WIN32
#include <io.h>
#include <winsock2.h>
#define read recv

#else
#include <unistd.h>
#define SOCKET_ERROR -1
#define closesocket close
#endif

class PacketHandler;

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
        PacketHandler *packetHandler;

        Client(int socket);

        int start();

    private:
        int handleConnection();

};
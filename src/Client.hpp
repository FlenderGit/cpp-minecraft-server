#pragma once

#include <thread>

namespace packet {
    class PacketHandler;
}

#include "Packet.hpp"
#include "PacketHandler.hpp"
#include "Logger.hpp"
#include "errno.h"

/**
 * @brief All possible states of the client
 */
enum State {
    Handshake,
    Status,
    Login,
    Play,
    Configuration,
};

/**
 * @brief Class Client to handle the client connection and its data
 */
class Client {

    public:
        int socket;

        /**
         * @brief The current state of the client
         * @see State
         */
        State state;

        /**
         * @brief The PacketHandler to handle the packets (used to not have all the logic in the Client class)
         */
        packet::PacketHandler *packetHandler;

        Client(int socket);

        /**
         * @brief Start a new thread to handle the connection with the client
         */
        void start();

    private:

        /**
         * @brief The callback function to handle the connection with the client
         * @return R_OK if the connection was successful, R_ERR otherwise
         */
        int handleConnection();

};
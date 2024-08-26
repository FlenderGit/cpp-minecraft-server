#pragma once

#include <mutex> 
#include <string>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#define socklen_t int
#else
#include <sys/socket.h>
#endif

#include "Logger.hpp"
#include "Client.hpp"

class Server {

    public:

        Server(std::string ip, int port);

        int run();

        // Handle client deconnection et server shutdown
        void disable();

        // Send a message to all clients, and disable the server
        void stop(std::string message, bool error);

        //void sendMessage(std::string message);


    private:

        std::string ip;
        int port;
        int server_socket;

        std::vector<Client*> clients;
        std::mutex clientsMutex;

        int init();

        int addClient(int client_socket);
        int removeClient(int client_socket);

        //void sendPacket(Packet packet);
        //void sendBytes(char* bytes, int length);

};
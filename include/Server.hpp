#pragma once

#include <mutex> 
#include <string>
#include <vector>

#include "Packet.hpp"
#include "Logger.hpp"
#include "Client.hpp"

class Server {

    public:

        Server(std::string ip, int port);

        void run();

        //void sendMessage(std::string message);


    private:

        std::string ip;
        int port;

        std::vector<Client*> clients;
        std::mutex clientsMutex;

        int init();

        int addClient(int client_socket);
        int removeClient(int client_socket);

        //void sendPacket(Packet packet);
        //void sendBytes(char* bytes, int length);

};
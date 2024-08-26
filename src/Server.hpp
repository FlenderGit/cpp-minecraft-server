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

/**
 * @brief Class Server to create a server using the Minecraft TCP protocol
 */
class Server {

    public:
        /**
         * @param ip Server address IP
         * @param port Server port
         */
        Server(std::string ip, int port);

        /**
         * @brief Start the server
         * @retval 0 if the server failed to start
         */
        int run();

        /**
         * @brief Disable the server and close the sockets
         * 
         * Used because Winsock needs to be closed before the program ends
         */
        void disable();

        /**
         * @brief Print a message and stop the server
         * 
         * @param message Message to print
         * @param error If the message is an error
         * 
         * @see disable is called after the message is printed
         */
        void stop(std::string message, bool error);

    private:

        std::string ip;         /**< Server IP (default localhost) */
        int port;               /**< Server port (default 25565) */
        int server_socket;      /**< Server socket, used to accept clients */

        std::vector<Client*> clients;   /**< List of clients */
        std::mutex clientsMutex;        /**< Mutex to protect the clients list */

        int init();

        int addClient(int client_socket);
        int removeClient(int client_socket);

        //void sendPacket(Packet packet);
        //void sendBytes(char* bytes, int length);

};
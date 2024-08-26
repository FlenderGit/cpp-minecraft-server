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
         * @param ip
         * @param port
         */
        Server(std::string ip, int port);

        /**
         * @brief Start the server
         * @retval R_ERR if the server failed to start
         * @retval R_OK else
         */
        int run();

        /**
         * @brief Disable the server and close the sockets
         * 
         * Used because Winsock needs close the sockets before the program ends
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

        std::string ip;         /**< default localhost */
        int port;               /**< default 25565 */
        int server_socket;

        std::vector<Client*> clients;
        std::mutex clientsMutex;        /**< Mutex to protect the clients list */

        /**
         * @brief Initialize the TCP server
         * 
         * @retval R_ERR if the server failed to initialize
         * @retval R_OK else
         */
        int init();

        /**
         * @brief Add a client to the server
         * 
         * @param client_socket The socket ID of the client
         */
        void addClient(int client_socket);

        /**
         * @brief Remove a client from the server
         * 
         * @param client_socket The socket ID of the client
         * @retval R_ERR if the client was not found
         * @retval R_OK else
         */
        int removeClient(int client_socket);

        //void sendPacket(Packet packet);
        //void sendBytes(char* bytes, int length);

};
#include "Server.hpp"

Server::Server(std::string ip, int port) {
    this->ip = ip;
    this->port = port;
}

int Server::run() {
    if (!init()) {
        return 0;
    }

    for(;;) {
        struct sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
        if (client_socket == -1) {
            Logger::log(ERROR, "Failed to accept client");
            return 0;
        }

        addClient(client_socket);
    }

    return 1;
}

int Server::addClient(int client_socket) {
    std::lock_guard<std::mutex> lock(clientsMutex);

    Client *client = new Client(client_socket);
    client->start();
    
    clients.push_back(client);

    return 1;
}

int Server::removeClient(int client_socket) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (int i = 0; i < clients.size(); i++) {
        if (clients[i]->socket == client_socket) {
            clients.erase(clients.begin() + i);
            return 1;
        }
    }
    return 0;
}

int Server::init() {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        Logger::log(ERROR, "Failed to create socket");
        return 0;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(25565);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        Logger::log(ERROR, "Failed to bind socket");
        return 0;
    }

    if (listen(server_socket, 5) == -1) {
        Logger::log(ERROR, "Failed to listen on socket");
        return 0;
    }

    return 1;
}
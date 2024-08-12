#include "Server.hpp"

Server::Server(std::string ip, int port) {
    this->address = sprintf("%s:%d", ip.c_str(), port);
}

void Server::sendBytes(char* bytes, int length) {
    
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
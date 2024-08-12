#include "Server.hpp"

Server::Server(std::string ip, int port) {
    this->address = sprintf("%s:%d", ip.c_str(), port);
    this->logger = new Logger();
}

void Server::sendBytes(char* bytes, int length) {
    
}

int Server::addClient(int client_socket) {
    std::lock_guard<std::mutex> lock(clientsMutex);

    Client *client = new Client(client_socket);
    clients.push_back(client);

    std::thread(&Server::handleClient, this, client_socket).detach();

    return 1;
}

int Server::removeClient(int client_socket) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    for (int i = 0; i < clients.size(); i++) {
        if (clients[i] == client_socket) {
            clients.erase(clients.begin() + i);
            return 1;
        }
    }
    return 0;
}

int Server::handleClient(int client_socket) {
    


    for (;;) {
        /*
        char buffer[1024];
        int bytesReceived = recv(client_socket, buffer, 1024, 0);
        if (bytesReceived <= 0) {
            removeClient(client_socket);
            close(client_socket);
            return 0;
        }
        Packet packet;
        packet.bytes = buffer;
        packet.length = bytesReceived;
        sendPacket(packet);
        */
    }

}
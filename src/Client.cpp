#include "Client.hpp"

Client::Client(int socket) {
    this->socket = socket;
    this->state = Login;
    this->packetHandler = new PacketHandler(this);
}

int Client::start() {
    std::thread(&Client::handleConnection, this).detach();
    return 1;
}

int Client::handleConnection() {


    for (;;) {
    
        // Create a new packet, read the bytes from the socket and test if 
        Packet *packet;
        int bytesReceived = read(socket, packet->bytes, 1024);
        if (bytesReceived <= 0) {
            close(socket);
            Logger::log(INFO, "Client disconnected");
            return 0;
        }

        if (packetHandler->isValid(packet) == false) {
            Logger::log(ERROR, "Invalid packet");
            return 0;
        }

        if (packetHandler->handle(packet) == 0) {
            Logger::log(ERROR, "Failed to handle packet");
            return 0;
        }

        
    }

    return 1;
}
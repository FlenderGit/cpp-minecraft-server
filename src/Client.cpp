#include "Client.hpp"

Client::Client(int socket) {
    this->socket = socket;
    this->state = Handshake;
    this->packetHandler = new packet::PacketHandler(this);
}

void Client::start() {
    std::thread(&Client::handleConnection, this).detach();
}

int Client::handleConnection() {

    for (;;) {
    
        // Create a new packet, read the bytes from the socket and test if 
        packet::ClientPacket *packet = new packet::ClientPacket();
        packet->load(socket);

        if (packetHandler->handle(packet) == 0) {
            Logger::log(ERR, "Failed to handle packet");
            break;
        }
        
    }

    // Close the socket and remove the client from the server
    closesocket(socket);
    Logger::log(INFO, "Client disconnected --> " + std::to_string(socket));

    return R_OK;
}
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
        Packet *packet = new Packet();

        int bytesReceived = read(socket, packet->bytes, 1024, 0);
        
        Logger::log(INFO, "Bytes received: " + std::to_string(bytesReceived));
        if (bytesReceived == SOCKET_ERROR) {
            Logger::log(ERR, "Failed to read from socket: " + std::string(strerror(errno)));
            break;
        }

        // Load the packet, test it and handle it
        packet->length = bytesReceived;
        packetHandler->loadPacket(packet);

        if (packetHandler->handle() == 0) {
            Logger::log(ERR, "Failed to handle packet");
            break;
        }
        
    }

    // Close the socket and remove the client from the server
    closesocket(socket);

    return 0;
}
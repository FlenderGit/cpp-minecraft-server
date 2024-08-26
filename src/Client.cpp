#include "Client.hpp"

Client::Client(int socket) {
    this->socket = socket;
    this->state = Login;
    this->packetHandler = new packet::PacketHandler(this);
}

int Client::start() {
    std::thread(&Client::handleConnection, this).detach();
    return 1;
}

int Client::handleConnection() {
    using namespace packet;

    for (;;) {
    
        // Create a new packet, read the bytes from the socket and test if 
        ClientPacket *packet = new ClientPacket();
        packet->load(socket);

        if (packetHandler->handle(packet) == 0) {
            Logger::log(ERR, "Failed to handle packet");
            break;
        }

       /*  int bytesReceived = read(socket, packet->getBuffer(), 1024);
        Logger::log(INFO, "Bytes received: " + std::to_string(bytesReceived));

        if (bytesReceived == 0) {
            Logger::log(INFO, "Client disconnected");
            break;
        }

        if (bytesReceived == SOCKET_ERROR) {
            Logger::log(ERR, "Failed to read from socket: " + std::string(strerror(errno)));
            break;
        }

        // Load the packet, test it and handle it
        packet->setLength(bytesReceived);



        packetHandler->loadPacket(packet); */
        
    }

    // Close the socket and remove the client from the server
    closesocket(socket);
    Logger::log(INFO, "Client disconnected --> " + std::to_string(socket));

    return 0;
}
#include "PacketHandler.hpp"

PacketHandler::PacketHandler(Client *client) {
    this->client = client;
}

int PacketHandler::loadPacket(Packet *packet) {
    currentPacket = packet;

    int length = readVarInt();
    if (length != packet->length) {
        Logger::log(ERR, "Invalid packet length");
        return 0;
    }

    int packetId = readVarInt();
    packet->id = packetId;

    return 1;
}

int PacketHandler::handle() {

    switch (client->state) {
        case Handshake:
            handleHandshake();
            break;
        case Login:
            break;
        case Play:
            break;
        case Configuration:
            break;
        default:
            Logger::log(ERR, "Invalid state");
            return 0;
    }

    return 1;
}

inline int PacketHandler::handleHandshake() {
    int protocolVersion = readVarInt();
    std::string serverAddress = readString();
    short serverPort = readShort();
    int nextState = readVarInt();

    Logger::log(INFO, "Protocol version: " + std::to_string(protocolVersion));
    Logger::log(INFO, "Server address: " + serverAddress);
    Logger::log(INFO, "Server port: " + std::to_string(serverPort));

    if (nextState == 1) {
        client->state = Status;
    } else if (nextState == 2) {
        client->state = Login;
    } else {
        Logger::log(ERR, "Invalid next state");
        return 0;
    }

    return 1;
}

int PacketHandler::readVarInt() {
    uint numRead = currentPacket->bytesRead;
    int result = 0;
    char read;
    do {
        read = currentPacket->bytes[numRead];
        int value = (read & SEGMENT_BITS);
        result |= (value << numRead);

        numRead += 7;
        if (numRead > 32) {
            Logger::log(ERR, "VarInt is too big");
            return 0;
        }
    } while ((read & CONTINUE_BIT) != 0);

    currentPacket->bytesRead = numRead;
    return result;
}

long PacketHandler::readVarLong() {
    return 0;
}

std::string PacketHandler::readString() {
    return "";
}

short PacketHandler::readShort() {
    return 0;
}
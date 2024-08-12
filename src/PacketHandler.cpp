#include "PacketHandler.hpp"

PacketHandler::PacketHandler(Client* client) {
    this->client = client;
}

int PacketHandler::loadPacket(Packet *packet) {
    currentPacket = packet;

    int length = readVarInt();
    if (length != packet->length) {
        Logger::log(ERROR, "Invalid packet length");
        return 0;
    }

    int packetId = readVarInt();

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
            Logger::log(ERROR, "Invalid state");
            return 0;
    }

    return 1;
}

inline int PacketHandler::handleHandshake() {
    int protocolVersion = readVarInt();
    std::string serverAddress = readString();
    short serverPort = readShort();
    int nextState = readVarInt();

    if (nextState == 1) {
        client->state = Status;
    } else if (nextState == 2) {
        client->state = Login;
    } else {
        Logger::log(ERROR, "Invalid next state");
        return 0;
    }

    return 1;
}

int PacketHandler::readVarInt() {
    int numRead = 0;
    int result = 0;
    char read;
    do {
        read = currentPacket->bytes[numRead];
        int value = (read & SEGMENT_BITS);
        result |= (value << numRead);

        numRead += 7;
        if (numRead > 32) {
            Logger::log(ERROR, "VarInt is too big");
            return 0;
        }
    } while ((read & CONTINUE_BIT) != 0);

    return result;
}
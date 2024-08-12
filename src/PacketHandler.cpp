#include "PacketHandler.hpp"

PacketHandler::PacketHandler(Client* client) {
    this->client = client;
}

int PacketHandler::handle(Packet *packet) {
    return 0;
}

bool PacketHandler::isValid(Packet *packet) {
    return false;
}
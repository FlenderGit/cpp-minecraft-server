#pragma once

#include "Types.hpp"

#define PACKET_BUFFER_SIZE 1024

class Packet {

    public:
        int id;
        byte bytes[PACKET_BUFFER_SIZE];
        int length;
        uint bytesRead;

        Packet();

    private:

};
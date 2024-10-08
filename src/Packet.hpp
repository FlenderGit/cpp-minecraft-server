#pragma once

#include "string"
#include "vector"

#include "Types.hpp"

#ifdef _WIN32
#include <io.h>
#include <winsock2.h>
#define read(sock, buf, len) recv(sock, buf, len, 0)

#else
#include <unistd.h>
#define SOCKET_ERROR -1
#define closesocket close
#endif

namespace packet
{
#define SEGMENT_BITS 0x7F
#define CONTINUE_BIT 0x80
#define PACKET_BUFFER_SIZE 1024
    using ByteList = std::vector<uint8_t>;

    /**
     * @brief Class Packet to handle ClientPacket and ResponsePacket
     */
    class Packet
    {
    public:
        Packet(): id(-1), length(0) {}
        int getID() { return id; }
        char *getBuffer() { return bytes; }
        int getLength() { return length; }
        void setLength(int length) { this->length = length; }

    protected:
        int id;
        int length;
        char bytes[PACKET_BUFFER_SIZE];

    private:
    };

    /**
     * @brief Class Type to handle different types of data
     * @tparam T The type of the data
     * @see https://wiki.vg/Protocol#Data_types for more information
     */
    template <typename T>
    class Type
    {
    public:
        Type(T data) : data(data) {}
        virtual ByteList write();

    private:
        T data;
    };

    using VarInt = Type<int>;
    using String = Type<std::string>;

    /**
     * @brief Class ClientPacket to handle packets from the client
     */
    class ClientPacket : public Packet
    {
    public:
        ClientPacket(): position(0) {}
        int load(int socket);
        void handle();

        int readVarInt();
        std::string readString();
        short readShort();
        unsigned short readUnsignedShort();

        int getID() { return id; }
    private:
        int position;
    };

    class ResponsePacket : public Packet
    {

    public:
        template <typename... Types>
        ResponsePacket(int id, Type<Types>...);

        /* template <typename... Types>
        static ResponsePacket *create(Type<Types>...); */
    };

}

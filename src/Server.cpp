#include "Server.hpp"

#include "Types.hpp"

Server::Server(std::string ip, int port) {
    this->ip = ip;
    this->port = port;
}

int Server::run() {
    if (!init()) {
        return 0;
    }

    for(;;) {
        struct sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
        if (client_socket == -1) {
            Logger::log(ERR, "Failed to accept client");
            return 0;
        }

        Logger::log(INFO, "New client connected --> " + std::to_string(client_socket));
        addClient(client_socket);
    }

    disable();

    return 1;
}

inline int Server::addClient(int client_socket) {
    std::lock_guard<std::mutex> lock(clientsMutex);

    Client *client = new Client(client_socket);
    client->start();
    
    clients.push_back(client);

    return 1;
}

int Server::removeClient(int client_socket) {
    std::lock_guard<std::mutex> lock(clientsMutex);
    uint size = static_cast<uint>(clients.size());
    for (uint i = 0; i < size; i++) {
        if (clients[i]->socket == client_socket) {
            clients.erase(clients.begin() + i);
            return 1;
        }
    }
    return 0;
}

int Server::init() {

    Logger::log(INFO, "Starting server on " + ip + ":" + std::to_string(port));

    #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        Logger::log(ERR, "Failed to start WSA");
        return 0;
    }
    #endif

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        stop("Failed to create socket", true);
        return 0;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        stop("Failed to bind", true);
        return 0;
    }

    if (listen(server_socket, 5) == -1) {
        stop("Failed to listen", true);
        return 0;
    }

    Logger::log(INFO, "Server started");

    return 1;
}

void Server::stop(std::string message, bool error) {
    Logger::log(error ? ERR : INFO, "Stopping server: " + message);
    disable();
    exit(0);
}

void Server::disable() {
    #ifdef _WIN32
    closesocket(server_socket);
    uint size = static_cast<uint>(clients.size());
    for (uint i = 0; i < size; i++) {
        closesocket(clients[i]->socket);
    }
    WSACleanup();

    #endif
}
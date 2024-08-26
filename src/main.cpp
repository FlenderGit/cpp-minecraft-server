
#include <stdio.h>

#include "Server.hpp"

int main() {
    Server *server = new Server("localhost", 25565);
    server->run();
    return EXIT_SUCCESS;
}
#pragma once

class Client {

    public:
        int socket;

        Client(int socket);

    private:
        int handleConnection();

};
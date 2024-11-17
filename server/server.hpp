#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#include "../utils/utils.hpp"

const size_t kServerPort = 8080;

class Server {

public:
    const size_t kConnectionReqs = 2;
    const size_t kBufferSize     = 1024;

    Server(Addr addr);

    void run();

    ~Server() {
        close(socket_);
    }

private:
    void addNewClient(Addr public_addr, Addr local_addr);
    void sendOtherClientId(Addr client_addr, int client_socket);

    int  socket_;
    std::vector<PublicLocalAddr> client_addrs;
};

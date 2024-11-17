#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "../server/server.hpp"

enum class ClientType {
    STARTER,
    WAITER
};

enum class ConnectionType {
    LOCAL,
    PUBLIC
};

class Client {
    const uint64_t kConnectionMsg = 0xDEADBEAF;
    const size_t   kBufferSize    = 1024;
    const char*    kMsg           = "Hello world!";
    const size_t   kMgsNumber     = 10;

public:
    Client(Addr client_addr, Addr server_addr, ClientType client_type);

    void run();

private:
    void initiateConnection();
    
    void waitConnection();

    void sendToServer(ServerRequest request);
    void tryPunchHole();

    Addr getConnectionAddr();

    Addr addr_;
    Addr server_addr_;
    
    PublicLocalAddr other_client_;
    ConnectionType  connection_type_;

    ClientType type_;
    int        socket_;
};
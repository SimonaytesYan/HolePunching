#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

const size_t kServerPort = 8080;

struct IPv4 {
    uint8_t p1;
    uint8_t p2;
    uint8_t p3;
    uint8_t p4;
    
    IPv4() {}
    IPv4(sockaddr_in sock_addr);

    bool operator==(const IPv4& other) const;

    operator in_addr_t() const;
};

enum class RequestType {
    NEW_CLIENT,
    CONNECT_TO_OTHER
};

struct ServerRequest {
    RequestType type;
    Addr        local_addr;

    ServerRequest() :
    type(RequestType::NEW_CLIENT),
    local_addr() { }
};

struct PublicLocalAddr {
    Addr public_addr;
    Addr local_addr;

    PublicLocalAddr() { }
    PublicLocalAddr(Addr public_addr, Addr local_addr);

    bool operator==(const PublicLocalAddr& other) const;
    bool operator!=(const PublicLocalAddr& other) const;
};

struct Addr {
    IPv4     ip;
    uint16_t port;

    Addr() { }
    Addr(const char* str);
    Addr(sockaddr_in sock_addr);

    bool operator==(const Addr& other) const;
    bool operator!=(const Addr& other) const;
};

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
    sockaddr_in createServerAddr(Addr addr);

    void addNewClient(Addr public_addr, Addr local_addr);

    void sendOtherClientId(Addr client_addr, int client_socket);

    sockaddr_in server_addr_;
    IPv4 own_ip_;
    int  socket_;

    std::vector<PublicLocalAddr> client_addrs;
};

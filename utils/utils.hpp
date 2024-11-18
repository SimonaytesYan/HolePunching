#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

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

struct Addr {
    IPv4     ip;
    uint16_t port;

    Addr() { }
    Addr(const char* str);
    Addr(sockaddr_in sock_addr);

    bool operator==(const Addr& other) const;
    bool operator!=(const Addr& other) const;

    operator sockaddr_in() const;
};

std::ostream& operator<<(std::ostream& out, const Addr& addr);

struct PublicLocalAddr {
    Addr public_addr;
    Addr local_addr;

    PublicLocalAddr() { }
    PublicLocalAddr(Addr public_addr, Addr local_addr);

    bool operator==(const PublicLocalAddr& other) const;
    bool operator!=(const PublicLocalAddr& other) const;
};
std::ostream& operator<<(std::ostream& out, const PublicLocalAddr& pl_addr);

enum class RequestType {
    NEW_CLIENT,
    CONNECT_TO_OTHER
};

struct ServerRequest {
    RequestType type;
    Addr        local_addr;

    ServerRequest(RequestType type, Addr local_addr) :
    type(type),
    local_addr(local_addr) { }

    ServerRequest() :
    type(RequestType::NEW_CLIENT),
    local_addr() { }

};

std::ostream& operator<<(std::ostream& out, const ServerRequest& request);
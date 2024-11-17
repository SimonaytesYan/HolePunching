#include "utils.hpp"

#include <iostream>
#include <cstring>

IPv4::IPv4(sockaddr_in sock_addr) {
    char* sock_addr_iterator = reinterpret_cast<char*>(&sock_addr);
        
    p1 = sock_addr_iterator[0];
    p2 = sock_addr_iterator[1];
    p3 = sock_addr_iterator[2];
    p4 = sock_addr_iterator[3];
}

bool IPv4::operator==(const IPv4& other) const {
    return p1 == other.p1 &&
           p2 == other.p2 &&
           p3 == other.p3 &&
           p4 == other.p4;
}

IPv4::operator in_addr_t() const {
    in_addr_t addr;
    memcpy(&addr, this, sizeof(IPv4));
        
    return addr;
}

PublicLocalAddr::PublicLocalAddr(Addr public_addr, Addr local_addr) :
public_addr(public_addr),
local_addr(local_addr) { }

bool PublicLocalAddr::operator==(const PublicLocalAddr& other) const {
    return public_addr == other.public_addr && 
           local_addr == other.local_addr;
}

bool PublicLocalAddr::operator!=(const PublicLocalAddr& other) const {
    return !(operator==(other));
}

Addr::Addr(const char* str) {
    sscanf(str, "%d:%d:%d:%d:%d", &ip.p1, &ip.p2, &ip.p3, &ip.p4, &port);
}

Addr::Addr(sockaddr_in sock_addr) :
ip(sock_addr),
port(sock_addr.sin_port) { }

bool Addr::operator==(const Addr& other) const {
    return other.ip == ip && other.port == port;
}

bool Addr::operator!=(const Addr& other) const {
    return !(operator==(other));
}

Addr::operator sockaddr_in() const {
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htons(ip);

    return addr;
}
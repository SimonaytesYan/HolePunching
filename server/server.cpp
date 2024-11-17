#include "server.hpp"

#include <cstring>
#include <iostream>
#include <vector>

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


Server::Server(Addr addr) {
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    server_addr_ = createServerAddr(addr);
    bind(socket_, (struct sockaddr*)&server_addr_, sizeof(server_addr_));
    
    listen(socket_, kConnectionReqs);
}

sockaddr_in Server::createServerAddr(Addr addr) {
    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(addr.port);
    server_addr.sin_addr.s_addr = addr.ip;

    memcpy(&server_addr.sin_addr.s_addr, &server_addr_, sizeof(IPv4));

    return server_addr;
}

void Server::run() {
    while (true) {
        sockaddr_in client_addr     = {};
        uint32_t    client_addr_len = 0;
        int         client_socket   = accept(socket_, 
                                             (struct sockaddr*)&client_addr, 
                                             &client_addr_len);

        ServerRequest request = {};
        ssize_t read_n = recv(client_socket, &request, sizeof(request), 0);

        switch (request.type) {
            case RequestType::NEW_CLIENT: {
                addNewClient(client_addr, request.local_addr);
            }
            
            case RequestType::CONNECT_TO_OTHER: {
                sendOtherClientId(client_addr, client_socket);
            }

            default:
                std::cerr << "Wrong request type\n";
        }
            
        close(client_socket);
    }
}

void Server::addNewClient(Addr public_addr, Addr local_addr) {
    PublicLocalAddr client_addr = {public_addr, local_addr};
    
    for (auto client : client_addrs) {
        if (client == client_addr)
            return;
    }

    client_addrs.push_back(client_addr);
}

void Server::sendOtherClientId(Addr client_addr, int client_socket) {
    const size_t size = client_addrs.size();

    if (size < 2) {
        std::cerr << "Only " << size << " clients registered\n";
        return;
    }

    for (size_t index = 0; index < size; index++) {
        PublicLocalAddr client = client_addrs[index];

    if (client.public_addr != client_addr) {
            send(client_socket, &client, sizeof(client), 0);

            return;
        }
    }
}

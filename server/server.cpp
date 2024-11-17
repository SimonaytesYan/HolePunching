#include "server.hpp"

#include <cstring>
#include <iostream>
#include <vector>

Server::Server(Addr addr) {
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr = addr;
    bind(socket_, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(socket_, kConnectionReqs);
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

    PublicLocalAddr a_client;
    PublicLocalAddr b_client;
    for (size_t index = 0; index < size; index++) {
        PublicLocalAddr client = client_addrs[index];
    
        if (client.public_addr != client_addr) {
            b_client = client;

            return;
        }
        else {
            a_client = client;
        }
    }

    sockaddr_in a_client_sock_addr = a_client.public_addr;
    sendto(socket_, &b_client, sizeof(b_client), 0, 
            (sockaddr*)&a_client_sock_addr, sizeof(a_client_sock_addr));

    sockaddr_in b_client_sock_addr = b_client.public_addr;
    sendto(socket_, &a_client, sizeof(a_client), 0, 
            (sockaddr*)&b_client_sock_addr, sizeof(b_client_sock_addr));
}

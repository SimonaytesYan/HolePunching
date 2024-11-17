#include "client.hpp"

#include <cstring>
#include <iostream>

Client::Client(Addr client_addr, Addr server_addr, ClientType client_type) :
    addr_       (client_addr),
    server_addr_(server_addr),
    type_       (client_type) {
    socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    
    sockaddr_in sock_addr = addr_;
    bind(socket_, (struct sockaddr*)&sock_addr, sizeof(sock_addr));

    sendToServer(ServerRequest(RequestType::NEW_CLIENT, addr_));
}

void Client::sendToServer(ServerRequest request) {
    sockaddr_in server_sock_addr = server_addr_;
    sendto(socket_, &request, sizeof(request), 0, 
            (sockaddr*)&server_sock_addr, sizeof(server_addr_));
}


void Client::run() {

    if (type_ == ClientType::STARTER) {
        initiateConnection();
        Addr other_client_addr = getConnectionAddr();
        
        char buffer[kBufferSize] = {};
        for (size_t i = 0; i < kMgsNumber; i++) {
            sockaddr_in other_client_sockaddr = other_client_addr;
            sendto(socket_, kMsg, strlen(kMsg), 0, 
                  (sockaddr*)&other_client_addr, sizeof(other_client_addr));

            memset(buffer, 0, kBufferSize);
            size_t read_n = recvfrom(socket_, buffer, sizeof(buffer), 0,
                                 nullptr, nullptr);
        }
    }
    else if (type_ == ClientType::WAITER) {
        waitConnection();
        Addr other_client_addr = getConnectionAddr();

        char buffer[kBufferSize] = {};
        for (size_t i = 0; i < kMgsNumber; i++) {
            memset(buffer, 0, kBufferSize);
            size_t read_n = recvfrom(socket_, buffer, sizeof(buffer), 0,
                                 nullptr, nullptr);
            
            size_t buffer_len = strlen(buffer);
            buffer[buffer_len] = '!';
            buffer_len++;
            buffer[buffer_len] = '\0';

            sockaddr_in other_client_sockaddr = other_client_addr;
            sendto(socket_, buffer, buffer_len, 0,
                  (sockaddr*)&other_client_addr, sizeof(other_client_addr));
        }
    }
}

void Client::initiateConnection() {
    sendToServer(ServerRequest(RequestType::NEW_CLIENT, {}));

    recvfrom(socket_, &other_client_, sizeof(other_client_), 0, nullptr, nullptr);

    tryPunchHole();
    tryPunchHole();

    uint64_t connection_msg_buffer = 0;
    sockaddr_in other_sock_addr = {};
    uint32_t other_sock_addr_len = 0;

    recvfrom(socket_, &connection_msg_buffer, sizeof(connection_msg_buffer), 0, 
             (sockaddr*) &other_sock_addr, &other_sock_addr_len);
    
    if (kConnectionMsg != connection_msg_buffer) {
        std::cerr << "CLIENT GOT NOT CONNECTION MSG\n";
        return;
    }

    if (Addr(other_sock_addr) == other_client_.local_addr) {
        connection_type_ = ConnectionType::LOCAL;
    }
    else if (Addr(other_sock_addr) == other_client_.public_addr) {
        connection_type_ = ConnectionType::PUBLIC;
    }
    else {
        std::cerr << "CLIENT FAIL TO CONNECT";
        return;
    }
}

Addr Client::getConnectionAddr() {
    if (connection_type_ == ConnectionType::LOCAL)
        return other_client_.local_addr;
    return other_client_.public_addr;
}

void Client::tryPunchHole() {
    sockaddr_in other_client_sockaddr = other_client_.local_addr;
    
    sendto(socket_, &kConnectionMsg, sizeof(kConnectionMsg), 0,
          (sockaddr*)&other_client_sockaddr, sizeof(other_client_sockaddr));
    
    other_client_sockaddr = other_client_.public_addr;
    sendto(socket_, &kConnectionMsg, sizeof(kConnectionMsg), 0, 
          (sockaddr*)&other_client_sockaddr, sizeof(other_client_sockaddr));
}

void Client::waitConnection() {
    recvfrom(socket_, &other_client_, sizeof(other_client_), 0, nullptr, nullptr);
}

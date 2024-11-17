#include <iostream>
#include <string.h>
#include <unistd.h>

#include "client.hpp"
#include "../server/server.hpp"

const char* message = "Hello world!";

const size_t kBufferSize = 1024;
const char* kEndRequests = "stop";

void run_client();

int main(int argc, char** argv) {
    
    if (argc != 2) {
        std::cerr << "Strange number of arguments";
        return -1;
    }

}

void run_client() {
    int client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in server_addr     = createServerAddr();
    socklen_t   server_addr_len = sizeof(server_addr);
    while (true) {
        char buffer[kBufferSize] = {};
        
        scanf("%s", buffer);
        if (!strcmp(buffer, kEndRequests))
            break;

        sendto(client_socket, buffer, strlen(buffer), 0, 
               (sockaddr*)&server_addr, server_addr_len);

        memset(buffer, 0, kBufferSize);
        size_t read_n = recvfrom(client_socket, buffer, sizeof(buffer), 0,
                                 nullptr, nullptr);
        printf("client: UDP (%zu)<%s>\n", read_n, buffer);
    }

    close(client_socket);
}


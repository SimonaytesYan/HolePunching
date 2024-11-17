#include <iostream>
#include <string.h>
#include <unistd.h>

#include "client.hpp"

void run_client();

int main(int argc, char** argv) {    
    if (argc != 4) {
        std::cerr << "Strange number of arguments";
        return -1;
    }

    Addr       server_addr(argv[1]);
    Addr       client_addr(argv[2]);
    ClientType client_type;

    if (!strcmp(argv[3], "waiter")) {
        client_type = ClientType::WAITER;
    }
    else if (!strcmp(argv[3], "starter")) {
        client_type = ClientType::STARTER;
    }
    else {
        std::cerr << "WRONG CLIENT TYPE";
    }

    Client client(client_addr, server_addr, client_type);
    client.run();
}

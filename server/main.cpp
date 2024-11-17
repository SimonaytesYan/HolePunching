#include <iostream>

#include "server.hpp"

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cerr << "Strange number of arguments";
        return -1;
    }

    Addr addr(argv[1]);
    Server server(addr);

    server.run();
}
#include "Socket.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "ERROR no port provided");
        exit(1);
    }

    try {
        Socket server;

        server.open(atoi(argv[1]));

        while (true) {
            Socket client;
            client.accept(server);
            
            std::array<uint8_t, 256> buffer;
            client.recieve(buffer);
            std::cout << "Here is the message: " << (const char*) buffer.data() << "\n";

            const std::string message = "Hello world!";
            client.send((const uint8_t*) message.data(), message.length());
        }

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}

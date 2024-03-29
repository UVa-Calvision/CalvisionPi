#include "Command.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>

void wait_for_key() {
    std::cout << "Waiting for key...\n";
    std::cin.get();
}

void send_command(Socket& server, BaseCommand& command) {
    command.dump_command(std::cout);

    try {
        command.write(server);

        uint16_t return_code = 0;
        server.read<uint16_t>(return_code);
        auto error_code = ErrorCodeTable.from_index(return_code);

        if (!error_code) {
            std::cerr << "[ERROR] Unrecognized error code: " << return_code << "\n";
            return;
        }

        if (*error_code == ErrorCode::Success) {
            std::cout << "Command success\n";
            command.read_return_value(server);
            std::cout << "\n";
        } else {
            std::cerr << "[ERROR] Return Code: " << *ErrorCodeTable.get<CommandCodeValues::Name>(*error_code) << "\n";
            wait_for_key();
        }

    } catch (const std::runtime_error& e) {
        std::cerr << "[ERROR] While sending command: " << e.what() << "\n";
        wait_for_key();
    }
}

std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    size_t last = 0;
    for (size_t i = 0; i < line.length(); i++) {
        if (line[i] == ' ') {
            tokens.push_back(line.substr(last, i - last));
            last = i+1;
        }
    }
    if (last < line.length()) {
        tokens.push_back(line.substr(last));
    }
    return tokens;
}

void run_commands(std::istream& input, Socket& socket) {

    std::string line;

    while (socket.good() && std::getline(input, line)) {

        const std::vector<std::string> tokens = tokenize(line);

        if (tokens.empty()) continue;

        std::unique_ptr<BaseCommand> command = make_command(tokens);

        if (!command) {
            std::cout << "Invalid command: " << line << "\nSkipping...\n";
            wait_for_key();
            continue;
        }

        send_command(socket, *command);
    }
}


/*
 * Raspberry pi local address:
 * 172.27.137.34:7777
 */
int main(int argc, char** argv) {

    if (argc < 3 || argc > 5) {
        std::cout << "Usage: " << argv[0] << " address port [command script]\n"
            << "Usage: " << argv[0] << " address port -c \"Command Text\"\n"
            << "Omitting command script will accept commands from stdin instead.\n";
        return 1;
    }

    try {
        const std::string hostname = argv[1];
        const int port = atoi(argv[2]);

        Socket server;

        std::cout << "Attempting to connect to server " << hostname << ":" << port << "\n";
        server.connect(hostname.c_str(), port);

        std::cout << "Connected.\n";

        if (argc == 5) {
            if (std::string(argv[3]) != "-c") {
                std::cout << "Unrecognized command tag: " << argv[3] << "\n";
                return 1;
            }
            std::cout << "Reading command line argument: " << argv[4] << "\n";
            std::stringstream input(argv[4]);
            run_commands(input, server);
        } else if (argc == 4) {
            std::cout << "Reading from " << argv[3] << "\n";
            std::ifstream input(argv[3]);
            run_commands(input, server);
        } else {
            std::cout << "Reading from cin...\n";
            run_commands(std::cin, server);
        }

    } catch (const std::runtime_error& e) {
        std::cerr << "[FATAL ERROR] " << e.what() << "\n";
        return 1;
    }

    return 0;
}

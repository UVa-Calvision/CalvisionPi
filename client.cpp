#undef BUILD_SERVER

#include "Command.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <memory>

void wait_for_key() {
    std::cout << "Waiting for key+enter...\n";
    std::string buffer;
    std::cin >> buffer;
}

void send_command(Socket& server, Command& command) {
    std::cout << "Sending command " << command.get_name() << "\n";

    try {
        server.write<uint16_t>(static_cast<uint16_t>(command.get_code()));

        command.write(server);

        uint16_t return_code = 0;
        server.read<uint16_t>(return_code);

        std::cout << "Command exited with return code: " << return_code << "\n";

        if (return_code != 0) {
            std::cerr << "Non-zero return code: " << return_code << "\n";
            wait_for_key();
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "[ERROR] While sending command: " << e.what() << "\n";
        wait_for_key();
    }
}

std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> tokens;
    int last = 0;
    for (int i = 0; i < line.length(); i++) {
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

#define ElseIfName(ENUM) \
    } else if (tokens[0] == Command##ENUM::name()) { \
        if (tokens.size() != Command##ENUM::size() + 1) { \
            std::cout << "Expected " << Command##ENUM::size() << " arguments for " << Command##ENUM::name() << "\n"; \
            break; \
        } \
        command = std::make_unique<Command##ENUM>();

    std::string line;

    while (std::getline(input, line)) {
        const std::vector<std::string> tokens = tokenize(line);

        if (tokens.empty()) continue;

        std::unique_ptr<Command> command = nullptr;

        if (false) {
        ElseIfName(EnableHighVoltage)
        ElseIfName(DisableHighVoltage)
        ElseIfName(SetHighVoltage)
            ((CommandSetHighVoltage*) command.get())->set_voltage(std::stof(tokens[1]));
        ElseIfName(SetLowVoltage)
            ((CommandSetLowVoltage*) command.get())->set_voltage(std::stof(tokens[1]));
        }

        if (!command) {
            std::cout << "Invalid command: " << line << "\nSkipping...\n";
            std::cout << "Waiting for key+enter...\n";
            std::cin >> line;
            continue;
        }

        send_command(socket, *command);

        std::cout << "\n";
    }
}


/*
 * Raspberry pi local address:
 * 172.27.137.24:7777
 */
int main(int argc, char** argv) {

    if (argc < 3 || argc > 4) {
        std::cout << "Usage: " << argv[0] << " address port [command script]\n"
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

        if (argc == 4) {
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

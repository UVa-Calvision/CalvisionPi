#include "Command.h"
#include "StringUtil.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <memory>
#include <sstream>

static bool batch_mode = false;

void wait_for_key() {
    // if (!batch_mode) {
    //     std::cout << "Waiting for key...\n";
    //     std::cin.get();
    // }
}

void send_command(Socket& server, BaseCommand& command) {

    if (!batch_mode) command.dump_command(std::cout);

    try {
        command.write(server);

        uint16_t return_code = 0;
        server.read<uint16_t>(return_code);
        auto error_code = ErrorCodeTable.from_index(return_code);

        if (!error_code) {
            if (!batch_mode) std::cerr << "[ERROR] Unrecognized error code: " << return_code << "\n";
            if (batch_mode) std::exit(-1);
            return;
        }

        if (*error_code == ErrorCode::Success) {
            if (!batch_mode) std::cout << "Command success\n";
            command.read_return_value(server);
            if (!batch_mode) std::cout << "\n";
            if (batch_mode) std::exit(0);
        } else {
            if (batch_mode) std::exit(return_code);
            std::cerr << "[ERROR] Return Code: " << *ErrorCodeTable.get<ErrorCodeValue::Name>(*error_code) << "\n";
            wait_for_key();
        }

    } catch (const std::runtime_error& e) {
        if (batch_mode) std::exit(-1);
        std::cerr << "[ERROR] While sending command: " << e.what() << "\n";
        wait_for_key();
    }
}

void run_commands(std::istream& input, Socket& socket) {

    std::string line;

    while (socket.good() && std::getline(input, line)) {

        const std::vector<std::string> tokens = tokenize(line);

        if (tokens.empty()) continue;

        std::unique_ptr<BaseCommand> command = make_command(tokens);

        if (!command) {
            if (!batch_mode) std::cout << "Invalid command: " << line << "\nSkipping...\n";
            wait_for_key();
            continue;
        }

        send_command(socket, *command);
    }
}

void usage(const std::string& argv0) {
    if (!batch_mode) {
        std::cout << "Usage: " << argv0 << " address port [flags]\n"
            << "        -c \"Command Text\"    run single command\n"
            << "        -b \"Command Text\"    run single command in batch mode\n"
            << "        -f script_file         run commands in file\n"
            << "        -h                     detailed help message\n"
            << "Omitting command script will accept commands from stdin instead.\n";
    }
}


/*
 * Raspberry pi local address:
 * 172.27.137.34:7777
 */
int main(int argc, char** argv) {

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-h") {
            print_command_help();
            return 0;
        }
    }


    if (argc != 3 && argc != 5) {
        usage(argv[0]);
        return -1;
    }

    try {
        const std::string hostname = argv[1];
        const int port = atoi(argv[2]);

        if (argc == 5 && std::string(argv[3]) == "-b") batch_mode = true;

        Socket server;

        if (!batch_mode) std::cout << "Attempting to connect to server " << hostname << ":" << port << "\n";
        server.connect(hostname.c_str(), port);

        if (!batch_mode) std::cout << "Connected.\n";

        if (argc == 5) {
            std::string flag = std::string(argv[3]);
            if (flag == "-c" || flag == "-b") {
                if (!batch_mode) std::cout << "Reading command line argument: " << argv[4] << "\n";
                std::stringstream input(argv[4]);
                run_commands(input, server);
            } else if (flag == "-f") {
                if (!batch_mode) std::cout << "Reading from " << argv[4] << "\n";
                std::ifstream input(argv[4]);
                run_commands(input, server);
            } else {
                if (!batch_mode) std::cout << "Unrecognized command tag: " << argv[3] << "\n";
                usage(argv[0]);
                return 1;
            }
        } else {
            if (!batch_mode) std::cout << "Reading from cin...\n";
            run_commands(std::cin, server);
        }
    } catch (const std::runtime_error& e) {
        if (!batch_mode) std::cerr << "[FATAL ERROR] " << e.what() << "\n";
        return 1;
    }

    return 0;
}

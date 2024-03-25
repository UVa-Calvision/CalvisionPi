#include "Command.h"

#include <iostream>

class ServerListener {
public:
    ServerListener(int port, Context* context)
        : context_(context)
    {
        server_.listen(port);
    }

    void listen() {
        while (true) {
            std::cout << "Waiting to listen to client...\n";

            Socket client;
            client.accept(server_);

            std::cout << "Accepted client\n";

            while (true) {
                uint16_t command_code = 0;
                try {
                    client.read<uint16_t>(command_code);
                } catch(const std::runtime_error& e) {
                    std::cerr << "[ERROR] Reading command code: " << e.what() << "\n";
                    break;
                }

                ErrorCode return_code = ErrorCode::Success;
                std::unique_ptr<Command> command = create_command(command_code);
                if (command) {
                    try {
                        command->read(client);
                    } catch(const std::runtime_error& e) {
                        std::cerr << "[ERROR] failed to read " << command->get_name() << ": " << e.what() << "\n";
                        return_code = ErrorCode::PoorlyStructuredCommand;
                    }

                    try { 
                        return_code = command->execute(*context_);
                    } catch(const std::runtime_error& e) {
                        std::cerr << "[ERROR] executing " << command->get_name() << ": " << e.what() << "\n";
                        return_code = ErrorCode::UnspecifiedFailure;
                    }
                } else {
                    std::cerr << "[ERROR] Unrecognized command code: " << command_code << "\n";
                    return_code = ErrorCode::InvalidCommand;
                }

                try {
                    client.write<uint16_t>(static_cast<uint16_t>(return_code));       
                } catch (const std::runtime_error& e) {
                    std::cerr << "[ERROR] Writing return code: " << e.what() << "\n";
                    break;
                }

                if (command_code == static_cast<uint16_t>(CommandCode::Quit))
                    return;
            }
        }
    }

private:
    Socket server_;
    Context* context_;
};

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " port\n";
        return 1;
    }

    int port = atoi(argv[1]);

    try {
        Context context;

        ServerListener server(port, &context);
        server.listen();
    } catch (const std::runtime_error& e) {
        std::cerr << "[FATAL ERROR]: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

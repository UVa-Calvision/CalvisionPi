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
                ErrorCode return_code = ErrorCode::Success;
                std::unique_ptr<BaseCommand> command = nullptr;
                try {
                     command = read_command(client);
                } catch(const std::runtime_error& e) {
                    break;
                }

                if (command) {
                    std::cout << "Have command!\n";
                    try { 
                        return_code = command->execute(*context_);
                    } catch(const std::runtime_error& e) {
                        std::cerr << "[ERROR] executing: " << e.what() << "\n";
                        return_code = ErrorCode::UnspecifiedFailure;
                    }
                } else {
                    std::cerr << "[ERROR] Unrecognized command\n";
                    return_code = ErrorCode::InvalidCommand;
                }

                try {
                    client.write<uint16_t>(static_cast<uint16_t>(return_code));       
                } catch (const std::runtime_error& e) {
                    std::cerr << "[ERROR] Writing return code: " << e.what() << "\n";
                    break;
                }

                if (command && command->code() == CommandCode::Quit)
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

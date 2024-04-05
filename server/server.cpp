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
        while (!context_->quit) {
            std::cout << "Waiting to listen to client...\n";

            Socket client;
            client.accept(server_);

            std::cout << "Accepted client\n";

            while (!context_->quit) {
                ReturnData return_data(ErrorCode::Success);
                std::unique_ptr<BaseCommand> command = nullptr;
                try {
                     command = read_command(client);
                } catch(const std::runtime_error& e) {
                    break;
                }

                if (command) {
                    try { 
                        return_data = command->execute(*context_);
                    } catch(const std::runtime_error& e) {
                        std::cerr << "[ERROR] executing: " << e.what() << "\n";
                        return_data = ReturnData(ErrorCode::UnspecifiedFailure);
                    }
                } else {
                    std::cerr << "[ERROR] Unrecognized command\n";
                    return_data = ReturnData(ErrorCode::InvalidCommand);
                }

                try {
                    client.write(static_cast<uint16_t>(*ErrorCodeTable.to_index(return_data.error_code)));
                    if (return_data.return_value) {
                        client.write<raw_type>(*return_data.return_value);
                    } 
                } catch (const std::runtime_error& e) {
                    std::cerr << "[ERROR] Writing return code: " << e.what() << "\n";
                    break;
                }
            }
        }
    }

private:
    Socket server_;
    Context* context_;
};

int main(int argc, char** argv) {

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " [port] [config_file]\n";
        return 1;
    }

    int port = atoi(argv[1]);
    std::string config_file = argv[2];

    try {
        ContextInput config(config_file);

        Context context(config);

        ServerListener server(port, &context);
        server.listen();
    } catch (const std::runtime_error& e) {
        std::cerr << "[FATAL ERROR]: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

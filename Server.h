#include "Command.h"

class ServerListener {
public:
    ServerListener(Context* context)
        : context_(context)
    {
        server_.open(7777);
    }

    void listen() {
        while (true) {
            std::cout << "Waiting to listen to client...\n";

            Socket client;
            client.accept(server_);

            std::cout << "Accepted client\n";

            uint16_t command_code = 0;
            client.read<uint16_t>(command_code);

            int16_t return_code = 0;
            std::unique_ptr<Command> command = create_command(command_code);
            if (command) {
                command->read(client);

                try { 
                    return_code = command->execute(*context_);
                } catch(const std::runtime_error& e) {
                    std::cerr << "Error in " << command->name() << ": " << e.what() << "\n";
                    return_code = -1;
                }
            }  

            client.write<int16_t>(return_code);
        }
    }

private:
    Socket server_;
    Context* context_;
};

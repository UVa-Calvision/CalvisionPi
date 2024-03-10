#undef BUILD_SERVER

#include "Command.h"
#include <iostream>


void send_command(Socket& server, Command& command) {
    
    std::cout << "Sending command " << command.name() << "\n";

    server.write<uint16_t>(static_cast<uint16_t>(command.code()));

    command.write(server);

    uint16_t return_code = 0;
    server.read<uint16_t>(return_code);

    std::cout << "Command exited with return code: " << return_code << "\n";

    if (return_code != 0) {
        std::cerr << "Non-zero return code: " << return_code << "\n";
    }
}


int main(void) {

    try {
        Socket server;
        server.connect("172.27.137.34", 7777);

        CommandEnableHighVoltage enable_hv;
        send_command(server, enable_hv);

        CommandDisableHighVoltage disable_hv;
        send_command(server, disable_hv);

        CommandSetHighVoltage set_hv;
        set_hv.set_voltage(10000.0);
        send_command(server, set_hv);

        CommandSetLowVoltage set_lv;
        set_lv.set_voltage(500.0);
        send_command(server, set_lv);

    } catch (const std::runtime_error& e) {
        std::cerr << "Error encountered: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

#include "Command.h"

class ServerListener {
public:
    ServerListener(int port, Context* context)
        : context_(context)
    {
        server_.open(port);
    }

    void listen() {
        while (true) {
            std::cout << "Waiting to listen to client...\n";

            Socket client;
            client.accept(server_);

            std::cout << "Accepted client\n";

            while (read_command(client));
        }
    }

    bool read_command(Socket& client) {
        uint16_t command_code = 0;
        try {
            client.read<uint16_t>(command_code);
        } catch(const std::runtime_error& e) {
            std::cerr << "[ERROR] Reading command code: " << e.what() << "\n";
            return false;
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
            return false;
        }

        return true;
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

//     // Enable GPIO
//     const unsigned int hv_enable_offset = 27;   // GPIO27 on the board
//     const int value = 0;
// 
//     GpioChip chip("/dev/gpiochip0");
//     chip.set_line("hv_enable", hv_enable_offset, value);

    
//     // Setup HV
//     try {
//         mcp4725 voltage_control(I2C_BUS_ID, HIGH_VOLTAGE_DEV_ID);
// 
//         uint16_t dac_min = 0x1c2;
//         float voltage_min = 59.03e3;
//         voltage_control.set_voltage_min(voltage_min);
//         voltage_control.set_dac_min(dac_min);
// 
//         uint16_t dac_max = 0x7d0;
//         float voltage_max = 4.459e3;
//         voltage_control.set_voltage_max(voltage_max);
//         voltage_control.set_dac_max(dac_max);
// 
//         // Calibrate max / min voltages
//         // std::cout << "Setting dac min value (" << std::hex << dac_min << std::dec << ")...\n";
//         // voltage_control.set_int(dac_min);
//         // std::cout << "Enter voltage (mV): ";
//         // voltage_min = 0;
//         // std::cin >> voltage_min;
//         // voltage_control.set_voltage_min(voltage_min);
// 
//         // std::cout << "Setting dac max value (" << std::hex << dac_max << std::dec << ")...\n";
//         // voltage_control.set_int(dac_max);
//         // std::cout << "Enter voltage (mV): ";
//         // voltage_max = 0;
//         // std::cin >> voltage_max;
//         // voltage_control.set_voltage_max(voltage_max);
// 
//         // Check DAC -> Voltage conversion
//         std::string temp;
//         for (int i = 0; i <= 100; i += 25) {
//             uint16_t value = (uint16_t) linear_interpolate(i, 0, 100, dac_min, dac_max);
//             std::cout << "Setting " << i << "\% of max voltage (" << std::hex << value << std::dec << "): "
//                 << voltage_control.dac_to_voltage(value) << " mV\n";
//             voltage_control.set_int(value);
//             std::cin >> temp;
//         }
// 
//         // Check voltage -> DAC conversion
//         for (int i = 0; i <= 100; i += 25) {
//             float voltage = linear_interpolate(i, 0, 100, voltage_max, voltage_min);
//             std::cout << "Setting " << i << "\% of max voltage: " << voltage << " mV\n";
//             voltage_control.set_int(voltage_control.voltage_to_dac(voltage));
//             std::cin >> temp;
// 
//         }
// 
//     } catch (const std::exception& e) {
//         std::cerr << "Exception: " << e.what() << "\n";
//     }
// 
//     std::cout << "Done.\n";

    return 0;
}

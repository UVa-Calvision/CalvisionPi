#include "gpio_manager.h"
#include "voltage_control.h"
#include "Socket.h"
#include <memory>

struct Context {
    
    Context()
        : chip("/dev/gpiochip0")
        , lv_control()
        , hv_control()
    {}

    GpioChip chip;
    LowVoltageControl lv_control;
    HighVoltageControl hv_control;
};

enum class CommandCode : uint16_t {
    NOpt = 0,
    EnableHighVoltage,
    DisableHighVoltage,
    SetHighVoltage,
    SetLowVoltage,
};

class Command {
public:
    Command(const std::string& cmd_name)
        : name_(cmd_name)
    {}

    virtual void write(Socket& socket) = 0;
    virtual void read(Socket& socket) = 0;
    virtual int16_t execute(Context& context) = 0;

    const std::string& name() const { return name_; }

private:
    std::string name_;
};

class CommandEnableHighVoltage : public Command {
public:
    CommandEnableHighVoltage()
        : Command("EnableHV")
    {}

    virtual void write(Socket& socket) override {}
    virtual void read(Socket& socket) override {}

    virtual int16_t execute(Context& context) {
        std::cout << "Enabling high voltage.\n";
        return 0;
    }
};

class CommandDisableHighVoltage : public Command {
public:
    CommandDisableHighVoltage()
        : Command("DisableHV")
    {}

    virtual void write(Socket& socket) override {}
    virtual void read(Socket& socket) override {}

    virtual int16_t execute(Context& context) {
        std::cout << "Disabling high voltage.\n";
        return 0;
    }
};

class CommandSetHighVoltage : public Command {
public:
    CommandSetHighVoltage()
        : Command("SetHV"), voltage_(0)
    {}

    void set_voltage(float v) { voltage_ = v; }

    virtual void write(Socket& socket) override {
        socket.write(voltage_);
    }

    virtual void read(Socket& socket) override {
        socket.read<float>(voltage_);
    }

    virtual int16_t execute(Context& context) {
        std::cout << "Setting high voltage to " << voltage_ << " mV\n";
        return 0;
    }

private:
    float voltage_;
};

class CommandSetLowVoltage : public Command {
public:
    CommandSetLowVoltage()
        : Command("SetLV"), voltage_(0)
    {}

    void set_voltage(float v) { voltage_ = v; }

    virtual void write(Socket& socket) override {
        socket.write<float>(voltage_);
    }

    virtual void read(Socket& socket) override {
        socket.read<float>(voltage_);
    }

    virtual int16_t execute(Context& context) {
        std::cout << "Setting low voltage to " << voltage_ << " mV\n";
        return 0;
    }

private:
    float voltage_;
};

inline
std::unique_ptr<Command> create_command(uint16_t command_code) {
#define CommandCase(ENUM) case static_cast<uint16_t>(CommandCode::ENUM): return std::make_unique<Command##ENUM>();

    switch (command_code) {
        CommandCase(EnableHighVoltage)
        CommandCase(DisableHighVoltage)
        CommandCase(SetHighVoltage)
        CommandCase(SetLowVoltage)
        default: return nullptr;
    }

#undef CommandCase
}

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

// /*
// * Sending:
// * Read 16 bit number which is size of data transferred
// * First 16 bit number is command code
// * Remaining data is command data
// *
// * Recieving:
// * First 16 bit number is size of data
// * Then return code
// * Then remaining values
// */
// void execute(CommandCode code, Context& context) {
//     switch (code) {
//         case EnableHighVoltage:
//             try {
//                 context.chip.set_line("hv_enable", 27, 1);
//             } catch (const std::runtime_error& e) {
//                 std::cerr << "Error in command EnableHighVoltage: " << e.what() << "\n";
//             }
//             break;
//         case DisableHighVoltage:
//             try {
//                 context.chip.set_line("hv_enable", 27, 0);
//             } catch (const std::runtime_error& e) {
//                 std::cerr << "Error in command DisableHighVoltage: " << e.what() << "\n";
//             }
//             break;
//         case SetHighVoltage:
//             try {
//             } catch (const std::runtime_error& e) {
//             }
//     }
// }
// 
// 
// class CommandEnableHighVoltage {
// public:
// 
//     CommandEnableHighVoltage() {}
// 
//     void send() {}
// 
//     void execute(Context& context) {
//         try {
//             context.chip.set_line("hv_enable", 27, 1);
//         } catch (const std::runtime_error& e) {
//             std::cerr << "Error in command EnableHighVoltage: " << e.what() << "\n";
//         }
//     }
// 
// private:
// };

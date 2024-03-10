#include "Socket.h"
#include <memory>

#ifdef BUILD_SERVER

#include "gpio_manager.h"
#include "voltage_control.h"
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

#endif



enum class CommandCode : uint16_t {
    NOpt = 0,
    EnableHighVoltage,
    DisableHighVoltage,
    SetHighVoltage,
    SetLowVoltage,
};

class Command {
public:
    Command(const std::string& cmd_name, CommandCode code)
        : name_(cmd_name), code_(code)
    {}

    virtual void write(Socket& socket) = 0;
    virtual void read(Socket& socket) = 0;
    
    CommandCode code() const { return code_; }
    const std::string& name() const { return name_; }

#ifdef BUILD_SERVER
    virtual int16_t execute(Context& context) = 0;
#endif

private:
    std::string name_;
    CommandCode code_;
};

class CommandEnableHighVoltage : public Command {
public:
    CommandEnableHighVoltage()
        : Command("EnableHV", CommandCode::EnableHighVoltage)
    {}

    virtual void write(Socket& socket) override {}
    virtual void read(Socket& socket) override {}

#ifdef BUILD_SERVER
    virtual int16_t execute(Context& context) {
        std::cout << "Enabling high voltage.\n";
        return 0;
    }
#endif
};

class CommandDisableHighVoltage : public Command {
public:
    CommandDisableHighVoltage()
        : Command("DisableHV", CommandCode::DisableHighVoltage)
    {}

    virtual void write(Socket& socket) override {}
    virtual void read(Socket& socket) override {}

#ifdef BUILD_SERVER
    virtual int16_t execute(Context& context) {
        std::cout << "Disabling high voltage.\n";
        return 0;
    }
#endif
};

class CommandSetHighVoltage : public Command {
public:
    CommandSetHighVoltage()
        : Command("SetHV", CommandCode::SetHighVoltage), voltage_(0)
    {}

    void set_voltage(float v) { voltage_ = v; }

    virtual void write(Socket& socket) override {
        socket.write(voltage_);
    }

    virtual void read(Socket& socket) override {
        socket.read<float>(voltage_);
    }

#ifdef BUILD_SERVER
    virtual int16_t execute(Context& context) {
        std::cout << "Setting high voltage to " << voltage_ << " mV\n";
        return 0;
    }
#endif

private:
    float voltage_;
};

class CommandSetLowVoltage : public Command {
public:
    CommandSetLowVoltage()
        : Command("SetLV", CommandCode::SetLowVoltage), voltage_(0)
    {}

    void set_voltage(float v) { voltage_ = v; }

    virtual void write(Socket& socket) override {
        socket.write<float>(voltage_);
    }

    virtual void read(Socket& socket) override {
        socket.read<float>(voltage_);
    }

#ifdef BUILD_SERVER
    virtual int16_t execute(Context& context) {
        std::cout << "Setting low voltage to " << voltage_ << " mV\n";
        return 0;
    }
#endif

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

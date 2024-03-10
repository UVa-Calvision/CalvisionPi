#pragma once

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
    virtual void write(Socket& socket) = 0;
    virtual void read(Socket& socket) = 0;
    
    virtual CommandCode get_code() = 0;
    virtual const std::string& get_name() = 0;

#ifdef BUILD_SERVER
    virtual int16_t execute(Context& context) = 0;
#endif
};

#define ClassFields(ENUM) \
    const static CommandCode code = CommandCode::ENUM; \
    static const std::string& name() { static std::string n = #ENUM; return n; } \
    virtual CommandCode get_code() override { return Command##ENUM::code; } \
    virtual const std::string& get_name() override { return Command##ENUM::name(); }

class CommandEnableHighVoltage : public Command {
public:
    CommandEnableHighVoltage()
    {}

    virtual void write(Socket& socket) override {}
    virtual void read(Socket& socket) override {}

    ClassFields(EnableHighVoltage);
    static size_t size() { return 0; }

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
    {}

    virtual void write(Socket& socket) override {}
    virtual void read(Socket& socket) override {}

    ClassFields(DisableHighVoltage)
    static size_t size() { return 0; }

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
        : voltage_(0)
    {}

    void set_voltage(float v) { voltage_ = v; }

    virtual void write(Socket& socket) override {
        socket.write(voltage_);
    }

    virtual void read(Socket& socket) override {
        socket.read<float>(voltage_);
    }

    ClassFields(SetHighVoltage)
    static size_t size() { return 1; }

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
        : voltage_(0)
    {}

    void set_voltage(float v) { voltage_ = v; }

    virtual void write(Socket& socket) override {
        socket.write<float>(voltage_);
    }

    virtual void read(Socket& socket) override {
        socket.read<float>(voltage_);
    }

    ClassFields(SetLowVoltage)
    static size_t size() { return 1; }

#ifdef BUILD_SERVER
    virtual int16_t execute(Context& context) {
        std::cout << "Setting low voltage to " << voltage_ << " mV\n";
        return 0;
    }
#endif

private:
    float voltage_;
};

#undef ClassFields

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

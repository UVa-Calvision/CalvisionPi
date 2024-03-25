#pragma once

#include "CppUtils/networking/Socket.h"
#include "CppUtils/c_util/CUtil.h"
#include <memory>
#include <iostream>
#include "SipmRegister.h"

#ifdef BUILD_SERVER

#include "GpioManager.h"
#include "VoltageControl.h"
#include "SiPMSupply.h"

struct Context {
    
    Context()
        : chip("/dev/gpiochip0")
        , lv_control()
        , hv_control()
    {}

    GpioChip chip;
    LowVoltageControl lv_control;
    HighVoltageControl hv_control;
    SipmI2cControl sipm_control;
};

#endif



enum class CommandCode : uint16_t {
    NOpt = 0,
    Quit,
    EnableHighVoltage,
    DisableHighVoltage,
    SetHighVoltage,
    SetLowVoltage,
    SipmVoltageControl,
};

enum class ErrorCode : uint16_t {
    Success = 0,
    InvalidCommand,
    PoorlyStructuredCommand,
    ResourceUnavailable,
    VoltageOutOfRange,
    UnspecifiedFailure,
};

class Command {
public:
    virtual void write(Socket& socket) = 0;
    virtual void read(Socket& socket) = 0;
    
    virtual CommandCode get_code() = 0;
    virtual const std::string& get_name() = 0;

#ifdef BUILD_SERVER
    virtual ErrorCode execute(Context& context) = 0;
#endif
};

#define ClassFields(ENUM) \
    const static CommandCode code = CommandCode::ENUM; \
    static const std::string& name() { static std::string n = #ENUM; return n; } \
    virtual CommandCode get_code() override { return Command##ENUM::code; } \
    virtual const std::string& get_name() override { return Command##ENUM::name(); }

class CommandQuit : public Command {
public:
    CommandQuit()
    {}

    virtual void write(Socket&) override {}
    virtual void read(Socket&) override {}

    ClassFields(Quit);
    static size_t size() { return 0; }

#ifdef BUILD_SERVER
    virtual ErrorCode execute(Context&) override {
        return ErrorCode::Success;
    }
#endif
};

class CommandEnableHighVoltage : public Command {
public:
    CommandEnableHighVoltage()
    {}

    virtual void write(Socket&) override {}
    virtual void read(Socket&) override {}

    ClassFields(EnableHighVoltage);
    static size_t size() { return 0; }

#ifdef BUILD_SERVER
    virtual ErrorCode execute(Context&) override;
#endif
};

class CommandDisableHighVoltage : public Command {
public:
    CommandDisableHighVoltage()
    {}

    virtual void write(Socket&) override {}
    virtual void read(Socket&) override {}

    ClassFields(DisableHighVoltage)
    static size_t size() { return 0; }

#ifdef BUILD_SERVER
    virtual ErrorCode execute(Context& context) override;
#endif
};

class CommandSetHighVoltage : public Command {
public:
    CommandSetHighVoltage()
        : voltage_(0)
    {}

    CommandSetHighVoltage(float voltage)
        : voltage_(voltage)
    {}

    virtual void write(Socket& socket) override {
        socket.write(voltage_);
    }

    virtual void read(Socket& socket) override {
        socket.read<float>(voltage_);
    }

    ClassFields(SetHighVoltage)
    static size_t size() { return 1; }

#ifdef BUILD_SERVER
    virtual ErrorCode execute(Context& context) override;
#endif

private:
    float voltage_;
};

class CommandSetLowVoltage : public Command {
public:
    CommandSetLowVoltage()
        : voltage_(0)
    {}

    CommandSetLowVoltage(float voltage)
        : voltage_(voltage)
    {}

    virtual void write(Socket& socket) override {
        socket.write<float>(voltage_);
    }

    virtual void read(Socket& socket) override {
        socket.read<float>(voltage_);
    }

    ClassFields(SetLowVoltage)
    static size_t size() { return 1; }

#ifdef BUILD_SERVER
    virtual ErrorCode execute(Context& context) override;
#endif

private:
    float voltage_;
};

class CommandSipmVoltageControl : public Command {
public:
    CommandSipmVoltageControl()
        : reg_(std::nullopt), raw_value_(0)
    {}

    CommandSipmVoltageControl(SipmControlRegister reg, uint32_t raw_value)
        : reg_(reg), raw_value_(raw_value)
    {}

    CommandSipmVoltageControl(const std::string_view& reg_name, const std::string& raw)
    {
        if (auto reg = SipmRegisterTable.lookup<SipmRegisterValue::Name>(reg_name)) {
            reg_ = *reg;
            switch (*SipmRegisterTable.get<SipmRegisterValue::Type>(*reg_)) {
                case SipmRegisterType::Bool:
                case SipmRegisterType::Int: {
                        int32_t temp = std::stoi(raw);
                        copy_raw_buffer(&raw_value_, &temp, 1);
                        break;
                    }
                case SipmRegisterType::Float: {
                        float temp = std::stof(raw);
                        copy_raw_buffer(&raw_value_, &temp, 1);
                        break;
                    }
            }
        } else {
            throw std::runtime_error("Invalid sipm control register name: " + std::string(reg_name));
        }
    }

    virtual void write(Socket& socket) override {
        if (!reg_)
            throw std::runtime_error("Invalid sipm control register");

        const uint8_t reg = *SipmRegisterTable.get<SipmRegisterValue::Register>(*reg_);
        socket.write<uint8_t>(reg);
        socket.write<uint32_t>(raw_value_);
    }

    virtual void read(Socket& socket) override {
        uint8_t reg = 0;
        socket.read<uint8_t>(reg);
        if (auto opt_reg = SipmRegisterTable.lookup<SipmRegisterValue::Register>(reg)) {
            reg_ = *opt_reg;
        } else {
            throw std::runtime_error("Invalid sipm control register: " + std::to_string(reg));
        }
        socket.read<uint32_t>(raw_value_);
    }

    ClassFields(SipmVoltageControl)
    static size_t size() { return 1; }

#ifdef BUILD_SERVER
    virtual ErrorCode execute(Context& context) override;
#endif

private:
    std::optional<SipmControlRegister> reg_;
    uint32_t raw_value_;
};

#undef ClassFields

std::unique_ptr<Command> create_command(uint16_t command_code);

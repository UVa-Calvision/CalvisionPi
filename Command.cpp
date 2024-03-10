#include "Command.h"

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


#ifdef BUILD_SERVER

ErrorCode CommandEnableHighVoltage::execute(Context& context) {
    std::cout << "Enabling high voltage.\n";
    try {
        context.chip.enable_hv();
    } catch (const std::runtime_error& e) {
        std::cerr << "[ERROR] While enabling high voltage: " << e.what() << "\n";
        return ErrorCode::ResourceUnavailable;
    }
    return ErrorCode::Success;
}

ErrorCode CommandDisableHighVoltage::execute(Context& context) {
    std::cout << "Disabling high voltage.\n";
    try {
        context.chip.disable_hv();
    } catch (const std::runtime_error& e) {
        std::cerr << "[ERROR] While disabling high voltage: " << e.what() << "\n";
        return ErrorCode::ResourceUnavailable;
    }
    return ErrorCode::Success;
}

ErrorCode CommandSetHighVoltage::execute(Context& context) {
    std::cout << "Setting high voltage to " << voltage_ << " mV\n";
    if (!context.hv_control.voltage_in_range(voltage_)) {
        std::cerr << "Voltage out of range\n";
        return ErrorCode::VoltageOutOfRange;
    }

    try {
        context.hv_control.set_voltage(voltage_);
    } catch (const std::runtime_error& e) {
        std::cerr << "[ERROR] While setting high voltage: " << e.what() << "\n";
        return ErrorCode::ResourceUnavailable;
    }

    return ErrorCode::Success;
}

ErrorCode CommandSetLowVoltage::execute(Context& context) {
    std::cout << "Setting low voltage to " << voltage_ << " mV\n";
    if (!context.lv_control.voltage_in_range(voltage_)) {
        std::cerr << "Voltage out of range\n";
        return ErrorCode::VoltageOutOfRange;
    }

    try {
        context.lv_control.set_voltage(voltage_);
    } catch (const std::runtime_error& e) {
        std::cerr << "[ERROR] While setting low voltage: " << e.what() << "\n";
        return ErrorCode::ResourceUnavailable;
    }

    return ErrorCode::Success;
}

#endif

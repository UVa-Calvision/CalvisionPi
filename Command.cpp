#include "Command.h"

#ifdef BUILD_SERVER

ErrorCode CommandEnableHighVoltage::execute(Context& context) {
    std::cout << "Enabling high voltage.\n";
    return ErrorCode::Success;
}

ErrorCode CommandDisableHighVoltage::execute(Context& context) {
    std::cout << "Disabling high voltage.\n";
    return ErrorCode::Success;
}

ErrorCode CommandSetHighVoltage::execute(Context& context) {
    std::cout << "Setting high voltage to " << voltage_ << " mV\n";
    if (!context.hv_control.voltage_in_range(voltage_)) {
        return ErrorCode::VoltageOutOfRange;
    }

    return ErrorCode::Success;
}

ErrorCode CommandSetLowVoltage::execute(Context& context) {
    std::cout << "Setting low voltage to " << voltage_ << " mV\n";
    if (!context.lv_control.voltage_in_range(voltage_)) {
        return ErrorCode::VoltageOutOfRange;
    }
    return ErrorCode::Success;
}

#endif

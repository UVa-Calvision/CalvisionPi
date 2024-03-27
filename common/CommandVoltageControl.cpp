#include "CommandVoltageControl.h"

#ifdef BUILD_SERVER
ErrorCode CommandVoltageControl::execute(Context& context, VoltageControlCommand action) {
    switch(action) {
        case VoltageControlCommand::HighVoltageEnable: {
            const auto& [enable] = unpack_raw<VoltageControlCommand::HighVoltageEnable>();
            std::cout << enable << "\n";
            if (enable)
                context.chip.enable_hv();
            else
                context.chip.disable_hv();
            }
            break;
        case VoltageControlCommand::HighVoltageSet: {
            const auto& [voltage] = unpack_raw<VoltageControlCommand::HighVoltageSet>();
            std::cout << voltage << "\n";
            if (!context.hv_control.voltage_in_range(voltage)) {
                std::cerr << "[ERROR] Voltage out of range\n";
                return ErrorCode::VoltageOutOfRange;
            }
            context.hv_control.set_voltage(voltage);
            }
            break;
        case VoltageControlCommand::LowVoltageSet: {
            const auto& [voltage] = unpack_raw<VoltageControlCommand::LowVoltageSet>();
            std::cout << voltage << "\n";
            if (!context.lv_control.voltage_in_range(voltage)) {
                std::cerr << "[ERROR] Voltage out of range\n";
                return ErrorCode::VoltageOutOfRange;
            }
            context.lv_control.set_voltage(voltage);
            }
            break;
    }
    return ErrorCode::Success;
}
#endif

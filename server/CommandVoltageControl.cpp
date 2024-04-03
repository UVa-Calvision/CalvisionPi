#include "CommandVoltageControl.h"

ReturnData CommandVoltageControl::execute(Context& context, VoltageControlCommand action) {
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
                return ReturnData(ErrorCode::VoltageOutOfRange);
            }
            context.hv_control.execute(voltage);
            }
            break;
        case VoltageControlCommand::LowVoltageSet: {
            const auto& [voltage] = unpack_raw<VoltageControlCommand::LowVoltageSet>();
            std::cout << voltage << "\n";
            if (!context.lv_control.voltage_in_range(voltage)) {
                std::cerr << "[ERROR] Voltage out of range\n";
                return ReturnData(ErrorCode::VoltageOutOfRange);
            }
            context.lv_control.execute(voltage);
            }
            break;
    }
    return ReturnData(ErrorCode::Success);
}

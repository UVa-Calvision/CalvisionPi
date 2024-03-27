#pragma once

#include "Command.h"
#include "CppUtils/c_util/Enum.h"
#include <string_view>

INDEXED_ENUM(VoltageControlCommand,
    HighVoltageEnable,
    HighVoltageSet,
    LowVoltageSet
);

constexpr static auto VoltageControlTable = CommandEnumTable<VoltageControlCommandIndexer>::make_table(
    std::tuple(VoltageControlCommand::HighVoltageEnable,   "HighVoltageEnable",  DataFormatTypes{DataFormat::Bool}),
    std::tuple(VoltageControlCommand::HighVoltageSet,      "HighVoltageSet",     DataFormatTypes{DataFormat::Float}),
    std::tuple(VoltageControlCommand::LowVoltageSet,       "LowVoltageSet",      DataFormatTypes{DataFormat::Float})
);

class CommandVoltageControl : public Command<CommandVoltageControl, VoltageControlCommand> {
public:

    constexpr static CommandCode code = CommandCode::VoltageControl;

    constexpr static const CommandEnumTable<VoltageControlCommandIndexer>& command_table() {
        return VoltageControlTable;
    }

    CommandVoltageControl(std::vector<raw_type> raw)
        : Command<CommandVoltageControl, VoltageControlCommand>(CommandCode::VoltageControl, std::move(raw))
    {}

#ifdef BUILD_SERVER
    virtual ErrorCode execute(Context& context, VoltageControlCommand action) override {
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
};

template <>
struct CommandMapping<CommandCode::VoltageControl> { using type = CommandVoltageControl; };

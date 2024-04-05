#pragma once

#include "Command.h"
#include "CppUtils/c_util/Enum.h"
#include <string_view>

INDEXED_ENUM(VoltageControlCommand,
    HighVoltageEnable,
    HighVoltageSet,
    LowVoltageSet
);

constexpr static auto VoltageControlTable = CommandEnumTable<VoltageControlCommandIndexer, 1>::make_table(
    std::pair(VoltageControlCommand::HighVoltageEnable,   std::tuple("HighVoltageEnable",  DataFormatTypes<1>{DataFormat::Bool} , std::nullopt, "Enable LED High Voltage DAC")),
    std::pair(VoltageControlCommand::HighVoltageSet,      std::tuple("HighVoltageSet",     DataFormatTypes<1>{DataFormat::Float}, std::nullopt, "Set voltage of LED High Voltage DAC (in units of mV)")),
    std::pair(VoltageControlCommand::LowVoltageSet,       std::tuple("LowVoltageSet",      DataFormatTypes<1>{DataFormat::Float}, std::nullopt, "Set voltage of LED bias (in units of mV)"))
);

CommandClass(VoltageControl);

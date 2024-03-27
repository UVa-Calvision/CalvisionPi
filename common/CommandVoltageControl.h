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

CommandClass(VoltageControl);

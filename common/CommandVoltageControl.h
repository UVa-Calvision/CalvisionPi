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
    std::pair(VoltageControlCommand::HighVoltageEnable,   std::tuple("HighVoltageEnable",  DataFormatTypes<1>{DataFormat::Bool} , std::nullopt)),
    std::pair(VoltageControlCommand::HighVoltageSet,      std::tuple("HighVoltageSet",     DataFormatTypes<1>{DataFormat::Float}, std::nullopt)),
    std::pair(VoltageControlCommand::LowVoltageSet,       std::tuple("LowVoltageSet",      DataFormatTypes<1>{DataFormat::Float}, std::nullopt))
);

CommandClass(VoltageControl);

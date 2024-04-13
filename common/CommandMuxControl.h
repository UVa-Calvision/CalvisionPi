#pragma once

#include "Command.h"

INDEXED_ENUM(MuxControlCommand,
    Left,
    Right
)

constexpr inline auto MuxControlTable = CommandEnumTable<MuxControlCommandIndexer, 0>::make_table(
    std::pair(MuxControlCommand::Left,          std::tuple("Left",      DataFormatTypes<0>{}, std::nullopt, "Select left sensors")),
    std::pair(MuxControlCommand::Right,         std::tuple("Right",     DataFormatTypes<0>{}, std::nullopt, "Select right sensors"))
);

CommandClass(MuxControl);

#pragma once

#include "Command.h"

INDEXED_ENUM(SipmDacControlCommand,
    UpdateRegister,
    PowerDownRegister,
    PowerDownChip,
    SelectInternalReference,
    SelectExternalReference
);

constexpr inline auto SipmDacControlTable = CommandEnumTable<SipmDacControlCommandIndexer, 1>::make_table(
    std::pair(SipmDacControlCommand::UpdateRegister,                std::tuple("UpdateRegister",           DataFormatTypes<1>{DataFormat::Uint},   std::nullopt, "Update (power up) DAC register")),
    std::pair(SipmDacControlCommand::PowerDownRegister,             std::tuple("PowerDownRegister",        DataFormatTypes<1>{DataFormat::Uint},   std::nullopt, "Power down DAC register")),
    std::pair(SipmDacControlCommand::PowerDownChip,                 std::tuple("PowerDownChip",            DataFormatTypes<1>{DataFormat::Uint},   std::nullopt, "Power down all registers")),
    std::pair(SipmDacControlCommand::SelectInternalReference,       std::tuple("SelectInternalReference",  DataFormatTypes<1>{DataFormat::Uint},   std::nullopt, "Power up internal reference")),
    std::pair(SipmDacControlCommand::SelectExternalReference,       std::tuple("SelectExternalReference",  DataFormatTypes<1>{DataFormat::Uint},   std::nullopt, "Power down internal reference"))
);

CommandClass(SipmDacControl);


INDEXED_ENUM(SipmDacWriteCommand,
    Write,
    WriteUpdate,
    WriteUpdateAll
);

constexpr inline auto SipmDacWriteTable = CommandEnumTable<SipmDacWriteCommandIndexer, 2>::make_table(
    std::pair(SipmDacWriteCommand::Write,          std::tuple("Write",             DataFormatTypes<2>{DataFormat::Uint, DataFormat::Float},   std::nullopt, "Write to input register WITHOUT updating")),
    std::pair(SipmDacWriteCommand::WriteUpdate,    std::tuple("WriteUpdate",       DataFormatTypes<2>{DataFormat::Uint, DataFormat::Float},   std::nullopt, "Write to input register and update")),
    std::pair(SipmDacWriteCommand::WriteUpdateAll, std::tuple("WriteUpdateAll",    DataFormatTypes<2>{DataFormat::Uint, DataFormat::Float},   std::nullopt, "Write to input register, then update all registers"))
);

CommandClass(SipmDacWrite);

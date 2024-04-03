#pragma once

#include "Command.h"

INDEXED_ENUM(TemperatureControlCommand,
    ShutdownMode,
    ThermostatMode,
    Polarity,
    FaultQueue,
    OneShot,
    ExtendedMode,
    ConversionRate
);

constexpr static auto TemperatureControlTable = CommandEnumTable<TemperatureControlCommandIndexer, 1>::make_table(
    std::pair(TemperatureControlCommand::ShutdownMode,     std::tuple("Shutdown",      DataFormatTypes<1>{DataFormat::Bool},      std::nullopt)),
    std::pair(TemperatureControlCommand::ThermostatMode,   std::tuple("Thermostat",    DataFormatTypes<1>{DataFormat::Bool},      std::nullopt)),
    std::pair(TemperatureControlCommand::Polarity,         std::tuple("Polarity",      DataFormatTypes<1>{DataFormat::Bool},      std::nullopt)),
    std::pair(TemperatureControlCommand::FaultQueue,       std::tuple("FaultQueue",    DataFormatTypes<1>{DataFormat::Uint},      std::nullopt)),
    std::pair(TemperatureControlCommand::OneShot,          std::tuple("OneShot",       DataFormatTypes<1>{DataFormat::Bool},      std::nullopt)),
    std::pair(TemperatureControlCommand::ExtendedMode,     std::tuple("ExtendedMode",  DataFormatTypes<1>{DataFormat::Bool},      std::nullopt)),
    std::pair(TemperatureControlCommand::ConversionRate,   std::tuple("Conversion",    DataFormatTypes<1>{DataFormat::Uint},      std::nullopt))
);

CommandClass(TemperatureControl);

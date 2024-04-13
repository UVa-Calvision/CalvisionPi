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

constexpr inline auto TemperatureControlTable = CommandEnumTable<TemperatureControlCommandIndexer, 2>::make_table(
    std::pair(TemperatureControlCommand::ShutdownMode,     std::tuple("Shutdown",      DataFormatTypes<2>{DataFormat::Uint, DataFormat::Bool},      std::nullopt, "Shutdown device")),
    std::pair(TemperatureControlCommand::ThermostatMode,   std::tuple("Thermostat",    DataFormatTypes<2>{DataFormat::Uint, DataFormat::Bool},      std::nullopt, "Comparator mode (0) or interrupt mode (1)")),
    std::pair(TemperatureControlCommand::Polarity,         std::tuple("Polarity",      DataFormatTypes<2>{DataFormat::Uint, DataFormat::Bool},      std::nullopt, "Set polarity of ALERT flag to active low (0) or high (1)")),
    std::pair(TemperatureControlCommand::FaultQueue,       std::tuple("FaultQueue",    DataFormatTypes<2>{DataFormat::Uint, DataFormat::Uint},      std::nullopt, "Number of consecutive faults required to trigger alert (0: 1 fault, 1: 2 faults, 2: 4 faults, 3: 6 faults)")),
    std::pair(TemperatureControlCommand::OneShot,          std::tuple("OneShot",       DataFormatTypes<2>{DataFormat::Uint, DataFormat::Bool},      std::nullopt, "When in shutdown mode, (true) makes a single temperature conversion then returns to shutdown")),
    std::pair(TemperatureControlCommand::ExtendedMode,     std::tuple("ExtendedMode",  DataFormatTypes<2>{DataFormat::Uint, DataFormat::Bool},      std::nullopt, "Use 13-bit format (1) or 12-bit format (0)")),
    std::pair(TemperatureControlCommand::ConversionRate,   std::tuple("Conversion",    DataFormatTypes<2>{DataFormat::Uint, DataFormat::Uint},      std::nullopt, "Temperature conversion rate (0: 0.25 Hz, 1: 1Hz, 2: 4 Hz (default), 3: 8 Hz)"))
);

CommandClass(TemperatureControl);



INDEXED_ENUM(TemperatureReadCommand,
    Read
);

constexpr inline auto TemperatureReadTable = CommandEnumTable<TemperatureReadCommandIndexer, 1>::make_table(
    std::pair(TemperatureReadCommand::Read,     std::tuple("Read",      DataFormatTypes<1>{DataFormat::Uint},   DataFormat::Float,  "Read temperature of specified sensor in degrees Celsius"))
);

CommandClass(TemperatureRead);

#pragma once

#include "Command.h"

INDEXED_ENUM(HumidityControlCommand,
    RequestRead,
    ReadHumidity,
    ReadTemperature,
    SoftReset
);

constexpr inline auto HumidityControlTable = CommandEnumTable<HumidityControlCommandIndexer, 0>::make_table(
    std::pair(HumidityControlCommand::RequestRead,      std::tuple("RequestRead",      DataFormatTypes<0>{}, std::nullopt,          "Request the humidity sensor to make a read (takes ~75ms before data is ready)")),
    std::pair(HumidityControlCommand::ReadHumidity,     std::tuple("ReadHumidity",     DataFormatTypes<0>{}, DataFormat::Float,     "Read the last measured humidity (percentage)")),
    std::pair(HumidityControlCommand::ReadTemperature,  std::tuple("ReadTemperature",  DataFormatTypes<0>{}, DataFormat::Float,     "Read the last measured temperature (degrees Celsius)")),
    std::pair(HumidityControlCommand::SoftReset,        std::tuple("SoftReset",        DataFormatTypes<0>{}, std::nullopt,          "Perform a soft-reset"))
);

CommandClass(HumidityControl);

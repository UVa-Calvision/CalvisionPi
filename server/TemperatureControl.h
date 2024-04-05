#pragma once

#include "CppUtils/io/I2cHandle.h"
#include "CppUtils/c_util/Enum.h"
#include <string_view>

#include "CppUtils/c_util/BitArray.h"

#include "Input.h"
#include "DataFormat.h"

// INDEXED_ENUM(TMP112_ConversionRate,
//     CR_0_25Hz,
//     CR_1Hz,
//     CR_4Hz,
//     CR_8Hz
// );
// 
// INDEXED_ENUM(TMP112_ConversionRateValues,
//     Name,
//     Frequency,
//     Code,
// );
// 
// constexpr inline auto TMP112_ConversionRateTable = EnumTable<TMP112_ConversionRateIndexer, TMP112_ConversionRateValues, std::string_view, float, uint8_t>::make_table(
//     std::pair(TMP112_ConversionRate::CR_0_25Hz, std::tuple("0_25Hz",    0.25,   0b00)),
//     std::pair(TMP112_ConversionRate::CR_1Hz,    std::tuple("1Hz",       1,      0b01)),
//     std::pair(TMP112_ConversionRate::CR_4Hz,    std::tuple("4Hz",       4,      0b10)),
//     std::pair(TMP112_ConversionRate::CR_8Hz,    std::tuple("8Hz",       8,      0b11))
// );


INDEXED_ENUM(TMP112_Configuration,
    ShutdownMode,
    ThermostatMode,
    Polarity,
    FaultQueue,
    OneShot,
    ExtendedMode,
    ConversionRate
);

INDEXED_ENUM(TMP112_ConfigurationValues,
    Name,
    Format,
    Address
);

constexpr inline auto TMP112_ConfigurationTable = EnumTable<TMP112_ConfigurationIndexer, TMP112_ConfigurationValuesIndexer, std::string_view, DataFormat, uint8_t>::make_table(
    std::pair(TMP112_Configuration::ShutdownMode,   std::tuple("Shutdown",      DataFormat::Bool,    0x8)),
    std::pair(TMP112_Configuration::ThermostatMode, std::tuple("Thermostat",    DataFormat::Bool,    0x9)),
    std::pair(TMP112_Configuration::Polarity,       std::tuple("Polarity",      DataFormat::Bool,    0xA)),
    std::pair(TMP112_Configuration::FaultQueue,     std::tuple("FaultQueue",    DataFormat::Uint,    0xB)),
    std::pair(TMP112_Configuration::OneShot,        std::tuple("OneShot",       DataFormat::Bool,    0xF)),
    std::pair(TMP112_Configuration::ExtendedMode,   std::tuple("ExtendedMode",  DataFormat::Bool,    0x4)),
    std::pair(TMP112_Configuration::ConversionRate, std::tuple("Conversion",    DataFormat::Uint,    0x6))
);



class tmp112 : private I2cReaderWriter {
    using ConfigStateType = BitArray<Endianness::Big, 16>;

public:
    tmp112(const TemperatureInput& input);

    using I2cReaderWriter::good;

    float read_temperature();

    template <TMP112_Configuration c, typename T>
    void write_configuration(T value) {
        constexpr DataFormat d = TMP112_ConfigurationTable.get<c, TMP112_ConfigurationValues::Format>();
        static_assert(d == type_to_format<T>());
        write_config<c>(static_cast<uint8_t>(make_raw<T>(value)));
    }

protected:

    template <TMP112_Configuration c>
    void write_configuration(ConfigStateType& config_state, uint8_t raw_value) {
        constexpr auto entry = TMP112_ConfigurationTable.get<c>();
        constexpr uint8_t address = entry.template get<TMP112_ConfigurationValues::Address>();

        if (entry.template get<TMP112_ConfigurationValues::Format>() == DataFormat::Bool) {
            BitArray<Endianness::Big, 1> bits(&raw_value);
            config_state.set_bit(address, bits[0]);
        } else if (entry.template get<TMP112_ConfigurationValues::Format>() == DataFormat::Uint) {
            BitArray<Endianness::Big, 2> bits(&raw_value);
            config_state.set_bit(address+0, bits[0]);
            config_state.set_bit(address+1, bits[1]);
        }
    }

    template <TMP112_Configuration c>
    void write_config(uint8_t raw_value) {
        std::array<uint8_t, 3> buffer;
        buffer[0] = TMP112_ConfigurationTable.get<c, TMP112_ConfigurationValues::Address>();

        write<uint8_t>(buffer[0]);
        read<uint8_t, 2>(&buffer[1]);

        BitArray<Endianness::Big, 16> config_state(&buffer[1]);

        write_configuration<c>(config_state, raw_value);
        
        write_buffer(buffer);
    }

    constexpr static uint8_t reg_addr_temperature   = 0b00;
    constexpr static uint8_t reg_addr_configuration = 0b01;
    constexpr static uint8_t reg_addr_t_low         = 0b10;
    constexpr static uint8_t reg_addr_t_high        = 0b11;
    constexpr static float temperature_resolution   = 0.0625; // Degrees Celsius per LSB
};

#include "Input.h"

#include "StringUtil.h"

#include <exception>
#include <iostream>
#include <fstream>
#include <string_view>


INDEXED_ENUM(InputCommand,
    GpioChip,
    HV,
    LV,
    Sipm,
    Temperature
);

INDEXED_ENUM(InputCommandValue,
    Name
);

template <typename Indexer>
using InputTable = EnumTable<Indexer, InputCommandValueIndexer, std::string_view>;

constexpr static auto InputCommandTable = InputTable<InputCommandIndexer>::make_table(
    std::pair(InputCommand::GpioChip,       std::tuple("GpioChip"       )),
    std::pair(InputCommand::HV,             std::tuple("HV"             )),
    std::pair(InputCommand::LV,             std::tuple("LV"             )),
    std::pair(InputCommand::Sipm,           std::tuple("SIPM"           )),
    std::pair(InputCommand::Temperature,    std::tuple("Temperature"    ))
);

INDEXED_ENUM(I2cCommand, BusId, DevId)
constexpr static auto I2cCommandTable = InputTable<I2cCommandIndexer>::make_table(
    std::pair(I2cCommand::BusId,    std::tuple("BusId")),
    std::pair(I2cCommand::DevId,    std::tuple("DevId"))
);

INDEXED_ENUM(DacCommand, VMin, VMax, DacMin, DacMax);
constexpr static auto DacCommandTable = InputTable<DacCommandIndexer>::make_table(
    std::pair(DacCommand::VMin,     std::tuple("VMin"   )),
    std::pair(DacCommand::VMax,     std::tuple("VMax"   )),
    std::pair(DacCommand::DacMin,   std::tuple("DacMin" )),
    std::pair(DacCommand::DacMax,   std::tuple("DacMax" ))
);

INDEXED_ENUM(SipmCommand, Impl, UartPath);
constexpr static auto SipmImplTable = InputTable<SipmCaenImplIndexer>::make_table(
    std::pair(SipmCaenImpl::DISABLE,    std::tuple("Disable")),
    std::pair(SipmCaenImpl::UART,       std::tuple("UART"   )),
    std::pair(SipmCaenImpl::I2C,        std::tuple("I2C"    ))
);
constexpr static auto SipmCommandTable = InputTable<SipmCommandIndexer>::make_table(
    std::pair(SipmCommand::Impl,        std::tuple("Impl"       )),
    std::pair(SipmCommand::UartPath,    std::tuple("UartPath"   ))
);

INDEXED_ENUM(GpioCommand, Path);
constexpr static auto GpioCommandTable = InputTable<GpioCommandIndexer>::make_table(
    std::pair(GpioCommand::Path, std::tuple("Path"))
);

bool parse_i2c(I2cInput& input, const std::vector<std::string>& tokens) {
    if (tokens.size() != 3) return false;
    if (auto lookup_result = I2cCommandTable.lookup<InputCommandValue::Name>(tokens[1])) {
        switch (lookup_result->first) {
            case I2cCommand::BusId: 
                input.bus_id = std::stol(tokens[2], nullptr, 0);
                break;
            case I2cCommand::DevId:
                input.dev_id = std::stol(tokens[2], nullptr, 0);
                break;
        }
        return true;
    } else {
        return false;
    }
}

bool parse_dac(DacInput& input, const std::vector<std::string>& tokens) {
    if (tokens.size() != 3) return false;
    if (auto lookup_result = DacCommandTable.lookup<InputCommandValue::Name>(tokens[1])) {
        switch (lookup_result->first) {
            case DacCommand::VMin: 
                input.v_min = std::stof(tokens[2]);
                break;
            case DacCommand::VMax:
                input.v_max = std::stof(tokens[2]);
                break;
            case DacCommand::DacMin:
                input.dac_min = std::stol(tokens[2], nullptr, 0);
                break;
            case DacCommand::DacMax:
                input.dac_max = std::stol(tokens[2], nullptr, 0);
                break;
        }
        return true;
    } else {
        return false;
    }
}

bool parse_sipm(SipmCaenInput& input, const std::vector<std::string>& tokens) {
    if (tokens.size() != 3) return false;
    if (auto lookup_result = SipmCommandTable.lookup<InputCommandValue::Name>(tokens[1])) {
        switch (lookup_result->first) {
            case SipmCommand::Impl: {
                if (auto impl_lookup = SipmImplTable.lookup<InputCommandValue::Name>(tokens[2])) {
                    input.impl = impl_lookup->first;
                    if (input.impl == SipmCaenImpl::UART) {
                        input.uart_path = "";
                    } else if (input.impl == SipmCaenImpl::I2C) {
                        input.i2c = I2cInput{};
                    }
                } else {
                    return false;
                }
                } break;
            case SipmCommand::UartPath: {
                if (input.uart_path) {
                    input.uart_path = tokens[2];
                }
                } break;
        }
        return true;
    } else {
        return false;
    }
}

bool parse_gpio(GpioInput& input, const std::vector<std::string>& tokens) {
    if (tokens.size() != 3) return false;
    if (auto lookup_result = GpioCommandTable.lookup<InputCommandValue::Name>(tokens[1])) {
        switch (lookup_result->first) {
            case GpioCommand::Path:
                input.file = tokens[2];
                break;
        }
        return true;
    } else {
        return false;
    }
}

bool parse_temperature(TemperatureInput&, const std::vector<std::string>&) {
    return true;
}

ContextInput::ContextInput(const std::string& filename) {
    std::ifstream input(filename);
    std::string line;
    while (std::getline(input, line)) {
        std::vector<std::string> tokens = tokenize(line);

        if (tokens.empty()) continue;

        if (auto lookup_result = InputCommandTable.lookup<InputCommandValue::Name>(tokens[0])) {
            try {
                bool success = false;
                switch (lookup_result->first) {
                    case InputCommand::GpioChip: {
                        success = parse_gpio(gpio_input, tokens);
                        } break;
                    case InputCommand::HV: {
                        success =  parse_i2c(hv_input.i2c, tokens)
                                || parse_dac(hv_input.dac, tokens);
                        } break;
                    case InputCommand::LV: {
                        success =  parse_i2c(lv_input.i2c, tokens)
                                || parse_dac(lv_input.dac, tokens);
                        } break;
                    case InputCommand::Sipm: {
                        // Use temp to ensure parsing is OK, but don't actually store the data
                        I2cInput temp{};
                        success =  parse_sipm(sipm_input, tokens)
                                || (sipm_input.i2c ? parse_i2c(*sipm_input.i2c, tokens) : parse_i2c(temp, tokens));
                        } break;
                    case InputCommand::Temperature: {
                        success = parse_temperature(temperature_input, tokens);
                        } break;
                }

                if (!success) {
                    std::cerr << "[ERROR] Failed to parse config command: " << line << "\n";
                }
            } catch(std::runtime_error& e) {
                std::cerr << "[ERROR] Error while parsing command: " << line << "\n";
            }
        } else {
            std::cerr << "[ERROR] Didn't recognize config command: " << line << "\n";
        }
    }
}

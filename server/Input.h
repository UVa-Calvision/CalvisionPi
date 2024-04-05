#pragma once

#include <string>
#include <optional>

#include "CppUtils/c_util/Enum.h"



struct I2cInput {
    uint8_t bus_id, dev_id;
};



struct DacInput {
    float v_min, v_max;
    uint16_t dac_min, dac_max;
};

struct VoltageControlInput {
    I2cInput i2c;
    DacInput dac;
};



INDEXED_ENUM(SipmCaenImpl,
    DISABLE,
    UART,
    I2C
);

struct SipmCaenInput {
    SipmCaenImpl impl = SipmCaenImpl::DISABLE;
    std::optional<std::string> uart_path = std::nullopt;
    std::optional<I2cInput> i2c = std::nullopt;
};



struct GpioInput {
    std::string file = "/dev/gpiochip0";
};



struct TemperatureInput {
    I2cInput i2c;
};




struct ContextInput {
    GpioInput gpio_input;
    VoltageControlInput lv_input, hv_input;
    SipmCaenInput sipm_input;
    TemperatureInput temperature_input;

    ContextInput(const std::string& filename);
};



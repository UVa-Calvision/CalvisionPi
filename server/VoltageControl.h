#pragma once

#include "CppUtils/io/I2cHandle.h"

#include "DAC.h"

class mcp4725 : private I2cReaderWriter, public DAC {
public:
    mcp4725(uint8_t bus_id, uint8_t dev_id,
            float v_min, float v_max,
            uint16_t dac_min, uint16_t dac_max);

    void execute(float value);

    using I2cReaderWriter::good;

private:
    std::array<uint8_t, 3> buffer_;
};

class LowVoltageControl : public mcp4725 {
public:
    LowVoltageControl();
};

class HighVoltageControl : public mcp4725 {
public:
    HighVoltageControl();
};

#pragma once

#include "CppUtils/io/I2cHandle.h"

#include "DAC.h"
#include "Input.h"

class mcp4725 : private I2cReaderWriter, public DAC {
public:
    mcp4725(const VoltageControlInput& input);

    void execute(float value);

    using I2cReaderWriter::good;

private:
    std::array<uint8_t, 3> buffer_;
};

// class LowVoltageControl : public mcp4725 {
// public:
//     LowVoltageControl();
// };
// 
// class HighVoltageControl : public mcp4725 {
// public:
//     HighVoltageControl();
// };

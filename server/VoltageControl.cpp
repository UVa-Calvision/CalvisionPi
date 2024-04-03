#include "VoltageControl.h"

#include "CppUtils/c_util/BitArray.h"

mcp4725::mcp4725(uint8_t bus_id, uint8_t dev_id,
                 float v_min, float v_max,
                 uint16_t dac_min, uint16_t dac_max)
    : I2cReaderWriter(bus_id, dev_id, OpenMode::ReadWrite),
      DAC(ArrayView<uint8_t, 2>(&buffer_[1]), v_min, v_max, dac_min, dac_max)
{}

void mcp4725::execute(float value) {
    buffer_[0] = 0b0100'0000;    // write to DAC command

    set_voltage(value);

    write_buffer(buffer_);
}

constexpr static uint8_t I2C_BUS_ID = 1;
constexpr static uint8_t LOW_VOLTAGE_DEV_ID = 0x64;
constexpr static uint8_t HIGH_VOLTAGE_DEV_ID = 0x65;

LowVoltageControl::LowVoltageControl()
    : mcp4725(I2C_BUS_ID, LOW_VOLTAGE_DEV_ID,
              0, 4.8e3, 0, 0xfff)
{}

HighVoltageControl::HighVoltageControl()
    : mcp4725(I2C_BUS_ID, HIGH_VOLTAGE_DEV_ID,
              59.03e3, 4.459e3, 0x1c3, 0x7d0)
{}


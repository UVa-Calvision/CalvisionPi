#include "VoltageControl.h"

#include <fmt/format.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <array>

float linear_interpolate(float x, float x_min, float x_max, float y_min, float y_max) {
    float slope = (y_max - y_min) / (x_max - x_min);
    return slope * (x - x_min) + y_min;
}


mcp4725::mcp4725(uint8_t bus_id, uint8_t dev_id)
    : I2cReaderWriter(bus_id, dev_id, OpenMode::ReadWrite),
      v_min_(0), v_max_(0),
      dac_min_(0), dac_max_(0)
{
}

void mcp4725::set_int(uint16_t value) {
    std::array<uint8_t, 3> buffer;
    buffer[0] = 0b0100'0000;    // write to DAC command
    buffer[1] = ((value & 0b1111'1111'0000) >> 4);
    buffer[2] = ((value & 0b0000'0000'0000) << 4);
    write_buffer(buffer);
}

void mcp4725::set_voltage(float voltage) {
    set_int(voltage_to_dac(voltage));
}

float mcp4725::dac_to_voltage(uint16_t value) const {
    return linear_interpolate((float) value, (float) dac_min_, (float) dac_max_, v_min_, v_max_);
}

uint16_t mcp4725::voltage_to_dac(float voltage) const {
    return (uint16_t) linear_interpolate(voltage, v_min_, v_max_, (float) dac_min_, (float) dac_max_);
}

bool mcp4725::voltage_in_range(float voltage) const {
    return voltage <= std::max(v_min_, v_max_) && voltage >= std::min(v_min_, v_max_);
}

constexpr static uint8_t I2C_BUS_ID = 1;
constexpr static uint8_t LOW_VOLTAGE_DEV_ID = 0x64;
constexpr static uint8_t HIGH_VOLTAGE_DEV_ID = 0x65;

LowVoltageControl::LowVoltageControl()
    : mcp4725(I2C_BUS_ID, LOW_VOLTAGE_DEV_ID)
{
    // Default calibration
    v_min_ = 0;
    v_max_ = 4.8e3;
    dac_min_ = 0;
    dac_max_ = 0xfff;
}

HighVoltageControl::HighVoltageControl()
    : mcp4725(I2C_BUS_ID, HIGH_VOLTAGE_DEV_ID)
{
    // Default calibration
    v_min_ = 59.03e3;
    v_max_ = 4.459e3;
    dac_min_ = 0x1c2;
    dac_max_ = 0x7d0;
}


#include <fmt/core.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <iostream>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <array>
#include <stdexcept>
#include <cstdint>

#include "binary_io.h"

class FileHandle : public BinaryWriter {
public:
    FileHandle(const std::string& name, const std::string& filename, int mode);
    ~FileHandle();

    bool good() const;
    void check_valid() const;
    void close();

protected:

    void write_impl(const uint8_t* buffer, size_t N) override;

    std::string name_;
    int fd_;
};

float linear_interpolate(float x, float x_min, float x_max, float y_min, float y_max);


class mcp4725 : private FileHandle {
public:
    mcp4725(uint8_t bus_id, uint8_t dev_id);

    void set_int(uint16_t value);
    void set_voltage(float voltage);

    void set_voltage_min(float v_min);
    void set_voltage_max(float v_max);
    void set_dac_min(uint16_t dac_min);
    void set_dac_max(int16_t dac_max);

    float dac_to_voltage(uint16_t value) const;
    uint16_t voltage_to_dac(float voltage) const;

    bool voltage_in_range(float voltage) const;

protected:
    float v_min_, v_max_;
    uint16_t dac_min_, dac_max_;
};

constexpr static uint8_t I2C_BUS_ID = 1;
constexpr static uint8_t LOW_VOLTAGE_DEV_ID = 0x64;
constexpr static uint8_t HIGH_VOLTAGE_DEV_ID = 0x65;

class LowVoltageControl : public mcp4725 {
public:
    LowVoltageControl()
        : mcp4725(I2C_BUS_ID, LOW_VOLTAGE_DEV_ID)
    {
        // Default calibration
        v_min_ = 0;
        v_max_ = 4.8e3;
        dac_min_ = 0;
        dac_max_ = 0xfff;
    }
};

class HighVoltageControl : public mcp4725 {
public:
    HighVoltageControl()
        : mcp4725(I2C_BUS_ID, HIGH_VOLTAGE_DEV_ID)
    {
        // Default calibration
        v_min_ = 59.03e3;
        v_max_ = 4.459e3;
        dac_min_ = 0x1c2;
        dac_max_ = 0x7d0;
    }
};

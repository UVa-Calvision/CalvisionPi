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

class FileHandle {
public:
    FileHandle(const std::string& name, const std::string& filename, int mode);
    ~FileHandle();

    bool good() const;
    void check_valid() const;
    void close();

    template <size_t N>
    void write(const std::array<uint8_t, N>& data) const {
        size_t n_written = ::write(fd_, data.data(), N);
        if (n_written != N) {
            throw std::runtime_error(fmt::format("Attempted to write {:d} bytes but actually wrote {:d} bytes", N, n_written));
        }
    }

protected:
    int fd_;
    std::string name_;
};

float linear_interpolate(float x, float x_min, float x_max, float y_min, float y_max);


class mcp4725 : private FileHandle {
public:
    mcp4725(uint8_t bus_id, uint8_t dev_id);

    void set_int(uint16_t value) const;

    void set_voltage_min(float v_min);
    void set_voltage_max(float v_max);
    void set_dac_min(uint16_t dac_min);
    void set_dac_max(int16_t dac_max);

    float dac_to_voltage(uint16_t value) const;
    uint16_t voltage_to_dac(float voltage) const;

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
        v_max_ = 0;
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

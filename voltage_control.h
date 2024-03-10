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


class fd_handle {
public:
    fd_handle(const std::string& name, const std::string& filename, int mode)
        : name_(name), fd_(open(filename.c_str(), mode))
    {
        check_valid();
    }

    ~fd_handle() {
        close();
    }

    operator bool() const {
        return fd_ != -1;
    }

    void check_valid() const {
        if (!*this) {
            throw std::runtime_error(fmt::format("Failed to open {0:s}", name_));
        }
    }

    void close() {
        if (*this) {
            ::close(fd_);
            fd_ = -1;
        }
    }

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

inline
float linear_interpolate(float x, float x_min, float x_max, float y_min, float y_max) {
    float slope = (y_max - y_min) / (x_max - x_min);
    return slope * (x - x_min) + y_min;
}



class mcp4725 : private fd_handle {
public:
    mcp4725(uint8_t bus_id, uint8_t dev_id)
        : fd_handle(fmt::format("mcp7425@{0:#x}:{1:#x}", bus_id, dev_id),
                    fmt::format("/dev/i2c-{0:d}", bus_id),
                    O_RDWR),
          v_min_(0), v_max_(0)
    {
        if (ioctl(fd_, I2C_SLAVE, dev_id) == -1) {
            throw std::runtime_error(fmt::format("Couldn't access i2c {}", name_));
        }
    }

    void set_int(uint16_t value) const {
        std::array<uint8_t, 3> buffer;
        buffer[0] = 0b0100'0000;    // write to DAC command
        buffer[1] = ((value & 0b1111'1111'0000) >> 4);
        buffer[2] = ((value & 0b0000'0000'0000) << 4);
        write(buffer);
    }

    void set_voltage_min(float v_min) {
        v_min_ = v_min;
    }

    void set_voltage_max(float v_max) {
        v_max_ = v_max;
    }

    void set_dac_min(uint16_t dac_min) {
        dac_min_ = dac_min;
    }

    void set_dac_max(int16_t dac_max) {
        dac_max_ = dac_max;
    }

    float dac_to_voltage(uint16_t value) const {
        return linear_interpolate((float) value, (float) dac_min_, (float) dac_max_, v_min_, v_max_);
    }

    uint16_t voltage_to_dac(float voltage) const {
        return (uint16_t) linear_interpolate(voltage, v_min_, v_max_, (float) dac_min_, (float) dac_max_);
    }

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
        v_min_ = 59.03e3;
        v_max_ = 4.459e3;
        dac_min_ = 0x1c2;
        dac_max_ = 0x7d0;
    }
};

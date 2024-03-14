#include "VoltageControl.h"

FileHandle::FileHandle(const std::string& name, const std::string& filename, int mode)
    : name_(name), fd_(open(filename.c_str(), mode))
{
    check_valid();
}

FileHandle::~FileHandle() {
    close();
}

bool FileHandle::good() const {
    return fd_ != -1;
}

void FileHandle::check_valid() const {
    if (!good()) {
        throw std::runtime_error(fmt::format("Failed to open {0:s}", name_));
    }
}

void FileHandle::close() {
    if (good()) {
        ::close(fd_);
        fd_ = -1;
    }
}

void FileHandle::write_impl(const uint8_t* buffer, size_t N) {
    int n_written = ::write(fd_, buffer, N);
    if (n_written != N)
        throw std::runtime_error(fmt::format("Attempted to write {:d} bytes but actually wrote {:d} bytes", N, n_written));
}

float linear_interpolate(float x, float x_min, float x_max, float y_min, float y_max) {
    float slope = (y_max - y_min) / (x_max - x_min);
    return slope * (x - x_min) + y_min;
}


mcp4725::mcp4725(uint8_t bus_id, uint8_t dev_id)
    : FileHandle(fmt::format("mcp7425@{0:#x}:{1:#x}", bus_id, dev_id),
                fmt::format("/dev/i2c-{0:d}", bus_id),
                O_RDWR),
      v_min_(0), v_max_(0)
{
    if (ioctl(fd_, I2C_SLAVE, dev_id) == -1) {
        throw std::runtime_error(fmt::format("Couldn't access i2c {}", name_));
    }
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

void mcp4725::set_voltage_min(float v_min) {
    v_min_ = v_min;
}

void mcp4725::set_voltage_max(float v_max) {
    v_max_ = v_max;
}

void mcp4725::set_dac_min(uint16_t dac_min) {
    dac_min_ = dac_min;
}

void mcp4725::set_dac_max(int16_t dac_max) {
    dac_max_ = dac_max;
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

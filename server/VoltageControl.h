#include "CppUtils/io/I2cHandle.h"

float linear_interpolate(float x, float x_min, float x_max, float y_min, float y_max);

class mcp4725 : private I2cReaderWriter {
public:
    mcp4725(uint8_t bus_id, uint8_t dev_id);

    void set_int(uint16_t value);
    void set_voltage(float voltage);

    float dac_to_voltage(uint16_t value) const;
    uint16_t voltage_to_dac(float voltage) const;

    bool voltage_in_range(float voltage) const;

    using I2cReaderWriter::good;

protected:
    float v_min_, v_max_;
    uint16_t dac_min_, dac_max_;
};

class LowVoltageControl : public mcp4725 {
public:
    LowVoltageControl();
};

class HighVoltageControl : public mcp4725 {
public:
    HighVoltageControl();
};

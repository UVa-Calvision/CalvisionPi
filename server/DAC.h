#pragma once

#include <cstddef>
#include <cstdint>

#include "CppUtils/container/ArrayView.h"

float linear_interpolation(float x, float x_min, float x_max, float y_min, float y_max);

class DAC {
public:
    DAC(ArrayView<uint8_t, 2> buffer, float v_min, float v_max, uint16_t dac_min, uint16_t dac_max);

    void set_v_min(float v_min);
    void set_v_max(float v_max);
    void set_dac_min(float dac_min);
    void set_dac_max(float dac_max);

    float dac_to_voltage(uint16_t value) const;
    uint16_t voltage_to_dac(float value) const;

    bool voltage_in_range(float voltage) const;
    bool dac_in_range(uint16_t dac) const;

    void set_voltage(float voltage);
    void set_int(uint16_t value);

protected:
    ArrayView<uint8_t, 2> buffer_;
    float v_min_, v_max_;
    uint16_t dac_min_, dac_max_;
};

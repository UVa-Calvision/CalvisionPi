#include "DAC.h"

#include "CppUtils/c_util/BitArray.h"

#include <algorithm>

template <typename T>
bool in_range(T x, T a, T b) {
    return x <= std::max(a,b) && x >= std::min(a,b);
}

float linear_interpolate(float x, float x_min, float x_max, float y_min, float y_max) {
    float slope = (y_max - y_min) / (x_max - x_min);
    return slope * (x - x_min) + y_min;
}

DAC::DAC(ArrayView<uint8_t, 2> buffer, const DacInput& input)
    : buffer_(buffer)
    , v_min_(input.v_min), v_max_(input.v_max)
    , dac_min_(input.dac_min), dac_max_(input.dac_max)
{}


void DAC::set_v_min(float v_min) { v_min_ = v_min; }
void DAC::set_v_max(float v_max) { v_max_ = v_max; }
void DAC::set_dac_min(float dac_min) { dac_min_ = dac_min; }
void DAC::set_dac_max(float dac_max) { dac_max_ = dac_max; }

float DAC::dac_to_voltage(uint16_t value) const {
    return linear_interpolate((float) value, (float) dac_min_, (float) dac_max_, v_min_, v_max_);
}

uint16_t DAC::voltage_to_dac(float voltage) const {
    return (uint16_t) linear_interpolate(voltage, v_min_, v_max_, (float) dac_min_, (float) dac_max_);
}

bool DAC::voltage_in_range(float voltage) const {
    return in_range(voltage, v_min_, v_max_);
}

bool DAC::dac_in_range(uint16_t dac) const {
    return in_range(dac, dac_min_, dac_max_);
}

void DAC::set_voltage(float value) {
    set_int(voltage_to_dac(value));
}

void DAC::set_int(uint16_t value) {
    auto bits = make_bit_array<Endianness::Big, 12>(buffer_, value);
    left_justify(bits);
}

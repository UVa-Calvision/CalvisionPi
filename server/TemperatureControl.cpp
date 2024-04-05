#include "TemperatureControl.h"

#include "CppUtils/c_util/BitArray.h"

tmp112::tmp112(const TemperatureInput& /*input*/)
    : I2cReaderWriter()
{}

float tmp112::read_temperature() {
        write(reg_addr_temperature);
        std::array<uint8_t, 2> buffer;
        read_buffer(buffer);

        bool is_extended_mode = buffer[1] & 0b1;
        int16_t value = 0;
        if (is_extended_mode) {
            BitArray<Endianness::Big, 13> bits = right_justify<13>(ArrayView<uint8_t, 2>(buffer));
            value = bits.convert<int16_t>();
        } else {
            BitArray<Endianness::Big, 12> bits = right_justify<12>(ArrayView<uint8_t, 2>(buffer));
            value = bits.convert<int16_t>();
        }

        return value * temperature_resolution;
}

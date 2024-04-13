#include "HumidityControl.h"

#include <chrono>
#include <thread>

aht10::aht10(const HumidityInput& input)
    : I2cReaderWriter(input.i2c.bus_id, input.i2c.dev_id, OpenMode::ReadWrite)
    , num_retries_(input.num_retries)
{
    // send initialization command
    std::array<uint8_t, 3> init_command = {0xe1, 0x28, 0x00};
    write_buffer(init_command);
}

void aht10::soft_reset() {
    write<uint8_t>(0b1011'1010);
}

bool aht10::request_read() {
    std::array<uint8_t, 3> command = {0xac, 0x33, 0x00};
    write_buffer(command);

    // Attempt to wait 75ms for reading humidity / temperature data
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(75ms);

    // Retry the reads if busy
    for (size_t i = 0; i < num_retries_; i++) {
        std::array<uint8_t, 6> result;
        read_buffer(result);

        BitArray<Endianness::Big, 8> state(&result[0]);
        bool busy = state.get_bit(7);
        if (busy) {
            std::this_thread::sleep_for(5ms);
        }

        BitArray<Endianness::Big, 20> temperature_bits(&result[3]);
        uint32_t raw_temperature = temperature_bits.convert<uint32_t>();
        temperature_ = 200.0 * static_cast<float>(raw_temperature) / static_cast<float>(uint32_t{1} << 20) - 50.0;

        BitArray<Endianness::Big, 20> humidity_bits = right_justify<20>(ArrayView<uint8_t, 3>(&result[1]));
        uint32_t raw_humidity = humidity_bits.convert<uint32_t>();
        humidity_ = 100.0 * static_cast<float>(raw_humidity) / static_cast<float>(uint32_t{1} << 20);

        return true;
    }

    return false;
}

float aht10::temperature() const { return temperature_; }
float aht10::humidity() const { return humidity_; }

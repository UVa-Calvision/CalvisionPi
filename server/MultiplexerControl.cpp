#include "MultiplexerControl.h"

tca9548::tca9548(const MultiplexerInput& input)
    : I2cReaderWriter(input.i2c.bus_id, input.i2c.dev_id, OpenMode::ReadWrite)
{}

void tca9548::enable(TCA9548_Channel channel) {
    auto code = TCA9548_ChannelTable.get<TCA9548_Value::Code>(channel);
    if (!code) {
        throw std::runtime_error("Invalide TCA9548 multiplexer channel code");
    }

    write<uint8_t>(*code);
}

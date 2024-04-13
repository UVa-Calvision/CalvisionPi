#include "SiPMDac.h"
#include "CppUtils/c_util/BitArray.h"

ltc2635::ltc2635(const VoltageControlInput& input)
    : I2cReaderWriter(input.i2c.bus_id, input.i2c.dev_id, OpenMode::ReadWrite),
    message_buffer_({0,0,0}),
    dacs_{DAC(&message_buffer_[1], input.dac),
          DAC(&message_buffer_[1], input.dac),
          DAC(&message_buffer_[1], input.dac),
          DAC(&message_buffer_[1], input.dac)}
{
    std::cout << "ltc2635 dev id: " << (unsigned) input.i2c.dev_id << "\n";
}

DAC& ltc2635::dac(LtcDacCode i) {
    if (i == LtcDacCode::ALL_DACs) return dacs_[0];
    if (auto code = LtcDacCodeTable.get<LtcDacValue::Code>(i))
        return dacs_[*code];
    else
        throw std::runtime_error("Invalid DAC Code");
}

const DAC& ltc2635::dac(LtcDacCode i) const {
    if (i == LtcDacCode::ALL_DACs) return dacs_[0];
    if (auto code = LtcDacCodeTable.get<LtcDacValue::Code>(i))
        return dacs_[*code];
    else
        throw std::runtime_error("Invalid DAC Code");
}

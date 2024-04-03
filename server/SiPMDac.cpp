#include "SiPMDac.h"
#include "CppUtils/c_util/BitArray.h"

ltc2635::ltc2635(uint8_t bus_id, uint8_t dev_id,
                 float v_min, float v_max,
                 uint16_t dac_min, uint16_t dac_max)
    : I2cReaderWriter(bus_id, dev_id, OpenMode::ReadWrite),
    message_buffer_({0,0,0}),
    dacs_{DAC(&message_buffer_[1], v_min, v_max, dac_min, dac_max),
          DAC(&message_buffer_[1], v_min, v_max, dac_min, dac_max),
          DAC(&message_buffer_[1], v_min, v_max, dac_min, dac_max),
          DAC(&message_buffer_[1], v_min, v_max, dac_min, dac_max)}
{
}


bool ltc2635::good() const { return handle_.good(); }

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

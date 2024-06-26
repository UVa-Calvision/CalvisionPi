#pragma once

#include "CppUtils/io/I2cHandle.h"
#include "CppUtils/c_util/Enum.h"
#include <string_view>

#include "CppUtils/c_util/BitArray.h"

#include "Input.h"
#include "DataFormat.h"

INDEXED_ENUM(TCA9548_Channel,
    Left,
    Right
);

INDEXED_ENUM(TCA9548_Value,
    Code
);

constexpr inline uint8_t humidity_channel = 0x40;

constexpr inline auto TCA9548_ChannelTable = EnumTable<TCA9548_ChannelIndexer, TCA9548_ValueIndexer, uint8_t>::make_table(
    std::pair(TCA9548_Channel::Left,        std::tuple(humidity_channel | 0x00)),
    std::pair(TCA9548_Channel::Right,       std::tuple(humidity_channel | 0x00))
);



class tca9548 : private I2cReaderWriter {
public:
    tca9548(const MultiplexerInput& input);

    using I2cReaderWriter::good;

    void enable(TCA9548_Channel channel);
};

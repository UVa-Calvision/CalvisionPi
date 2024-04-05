#pragma once

#include "CppUtils/io/I2cHandle.h"
#include "CppUtils/container/ArrayView.h"
#include "CppUtils/c_util/Enum.h"
#include "DAC.h"

#include <iostream>
#include <exception>


INDEXED_ENUM(LtcCommandCode,
    WriteRegister,
    UpdateRegister,
    WriteRegisterUpdateAll,
    WriteUpdateRegister,
    PowerDownRegister,
    PowderDownChip,
    SelectInternalReference,
    SelectExternalReference
)

INDEXED_ENUM(LtcCommandValue,
    Code,
    HasParameter
)

constexpr static auto LtcCommandCodeTable = EnumTable<LtcCommandCodeIndexer, LtcCommandValueIndexer, uint8_t, bool>::make_table(
    std::pair(LtcCommandCode::WriteRegister,                 std::tuple(0b0000,  true)),
    std::pair(LtcCommandCode::UpdateRegister,                std::tuple(0b0001, false)),
    std::pair(LtcCommandCode::WriteRegisterUpdateAll,        std::tuple(0b0010,  true)),
    std::pair(LtcCommandCode::WriteUpdateRegister,           std::tuple(0b0011,  true)),
    std::pair(LtcCommandCode::PowerDownRegister,             std::tuple(0b0100, false)),
    std::pair(LtcCommandCode::PowderDownChip,                std::tuple(0b0101, false)),
    std::pair(LtcCommandCode::SelectInternalReference,       std::tuple(0b0110, false)),
    std::pair(LtcCommandCode::SelectExternalReference,       std::tuple(0b0111, false))
);

INDEXED_ENUM(LtcDacCode,
    DAC_A,
    DAC_B,
    DAC_C,
    DAC_D,
    ALL_DACs
)

INDEXED_ENUM(LtcDacValue, Code);

constexpr static auto LtcDacCodeTable = EnumTable<LtcDacCodeIndexer, LtcDacValueIndexer, uint8_t>::make_table(
    std::pair(LtcDacCode::DAC_A,        std::tuple(0b0000)),
    std::pair(LtcDacCode::DAC_B,        std::tuple(0b0001)),
    std::pair(LtcDacCode::DAC_C,        std::tuple(0b0010)),
    std::pair(LtcDacCode::DAC_D,        std::tuple(0b0011)),
    std::pair(LtcDacCode::ALL_DACs,     std::tuple(0b1111)) 
);


class ltc2635 : private I2cReaderWriter {
public:
    ltc2635(const VoltageControlInput& input);

    bool good() const;

    DAC& dac(LtcDacCode i);
    const DAC& dac(LtcDacCode i) const;

    template <LtcCommandCode command,
              std::enable_if_t<LtcCommandCodeTable.get<command, LtcCommandValue::HasParameter>(), bool> = true>
    void execute(LtcDacCode dac, float voltage) {
        pack_first_message_byte<command>(dac);

        if (dac == LtcDacCode::ALL_DACs) {
            std::cout << "WARNING: Setting voltage for all LTC DACs (may have different calibrations). Using DAC A calibration to convert voltage to unsigned integer.\n";
        }

        this->dac(dac).set_voltage(voltage);

        write_buffer(message_buffer_);
    }

    template <LtcCommandCode command,
              std::enable_if_t<!LtcCommandCodeTable.get<command, LtcCommandValue::HasParameter>(), bool> = true>
    void execute(LtcDacCode dac) {
        pack_first_message_byte<command>(dac);

        message_buffer_[1] = 0;
        message_buffer_[2] = 0;

        write_buffer(message_buffer_);
    }

private:
    template <LtcCommandCode command>
    void pack_first_message_byte(LtcDacCode dac) {
        auto opt_dac_code = LtcDacCodeTable.get<LtcDacValue::Code>(dac);
        if (!opt_dac_code)
            throw std::runtime_error("Invalid LTC dac code?");
        
        uint8_t com_code = LtcCommandCodeTable.get<command, LtcCommandValue::Code>();
        uint8_t dac_code = *opt_dac_code;

        message_buffer_[0] = (com_code << 4) | dac_code;
        
        // using Bit4Code = BitArray<Endianness::Big, 4>;
        // pack_into(&message_buffer_[0], Bit4Code(&com_code), Bit4Code(&dac_code));
    }

    I2cReaderWriter handle_;
    std::array<uint8_t, 3> message_buffer_;
    std::array<DAC, 4> dacs_;
};

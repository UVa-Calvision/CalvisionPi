#include "CommandSipmDacControl.h"

std::optional<LtcDacCode> acquire_dac(Context& context, uint32_t dac_number) {
    if (context.mux_control_unavailable())
        return std::nullopt;

    if (dac_number < 4) {
        context.multiplexer->enable(TCA9548_Channel::Left);
        switch (dac_number) {
            case 0: return LtcDacCode::DAC_A;
            case 1: return LtcDacCode::DAC_B;
            case 2: return LtcDacCode::DAC_C;
            case 3: return LtcDacCode::DAC_D;
        }
    } else if (dac_number < 8) {
        context.multiplexer->enable(TCA9548_Channel::Right);
        switch (dac_number) {
            case 4: return LtcDacCode::DAC_A;
            case 5: return LtcDacCode::DAC_B;
            case 6: return LtcDacCode::DAC_C;
            case 7: return LtcDacCode::DAC_D;
        }
    } 

    return std::nullopt;
}



INDEXED_ENUM(RegMap, Register);

constexpr inline auto ControlToRegTable = EnumTable<SipmDacControlCommandIndexer, RegMapIndexer, LtcCommandCode>::make_table(
    std::pair(SipmDacControlCommand::UpdateRegister,          std::tuple(LtcCommandCode::UpdateRegister         )),
    std::pair(SipmDacControlCommand::PowerDownRegister,       std::tuple(LtcCommandCode::PowerDownRegister      )),
    std::pair(SipmDacControlCommand::PowerDownChip,           std::tuple(LtcCommandCode::PowerDownChip          )),
    std::pair(SipmDacControlCommand::SelectInternalReference, std::tuple(LtcCommandCode::SelectInternalReference)),
    std::pair(SipmDacControlCommand::SelectExternalReference, std::tuple(LtcCommandCode::SelectExternalReference))
);

template <SipmDacControlCommand c>
struct ControlRegisterFunctor {
    constexpr static LtcCommandCode control_reg = ControlToRegTable.get<c, RegMap::Register>();

    ReturnData operator()(Context& context, LtcDacCode dac) const {
        context.sipm_dac_control->execute<control_reg>(dac);
        return ReturnData(ErrorCode::Success);
    }
};


ReturnData CommandSipmDacControl::execute(Context& context, SipmDacControlCommand c) {
    if (raw_data_.size() != 2)
        return ReturnData(ErrorCode::PoorlyStructuredCommand);

    auto dac_code = acquire_dac(context, format_to_type<DataFormat::Uint>::convert_raw(raw_data_[1]));
    if (!dac_code)
        return ReturnData(ErrorCode::ResourceUnavailable);

    if (context.sipm_dac_control_unavailable())
        return ReturnData(ErrorCode::ResourceUnavailable);

    return *SipmDacControlCommandIndexer::dispatch<ControlRegisterFunctor>(c, context, *dac_code);
}

constexpr inline auto WriteToRegTable = EnumTable<SipmDacWriteCommandIndexer, RegMapIndexer, LtcCommandCode>::make_table(
    std::pair(SipmDacWriteCommand::Write,           std::tuple(LtcCommandCode::WriteRegister)),
    std::pair(SipmDacWriteCommand::WriteUpdate,     std::tuple(LtcCommandCode::WriteUpdateRegister)),
    std::pair(SipmDacWriteCommand::WriteUpdateAll,  std::tuple(LtcCommandCode::WriteRegisterUpdateAll))
);

template <SipmDacWriteCommand c>
struct WriteRegisterFunctor {
    constexpr static LtcCommandCode control_reg = WriteToRegTable.get<c, RegMap::Register>();

    ReturnData operator()(Context& context, LtcDacCode dac, raw_type raw_voltage) const {
        float voltage;
        try {
            voltage = format_to_type<DataFormat::Float>::convert_raw(raw_voltage);
        } catch (const std::runtime_error& e) {
            std::cerr << "[ERROR]: Error during type conversion " << e.what() << "\n";
            return ReturnData(ErrorCode::InvalidCommand);
        }

        context.sipm_dac_control->execute<control_reg>(dac, voltage);

        return ReturnData(ErrorCode::Success);
    }
};

ReturnData CommandSipmDacWrite::execute(Context& context, SipmDacWriteCommand c) {
    if (raw_data_.size() != 3)
        return ReturnData(ErrorCode::PoorlyStructuredCommand);

    auto dac_code = acquire_dac(context, format_to_type<DataFormat::Uint>::convert_raw(raw_data_[1]));
    if (!dac_code)
        return ReturnData(ErrorCode::ResourceUnavailable);

    if (context.sipm_dac_control_unavailable())
        return ReturnData(ErrorCode::ResourceUnavailable);

    return *SipmDacWriteCommandIndexer::dispatch<WriteRegisterFunctor>(c, context, *dac_code, raw_data_[2]);
}

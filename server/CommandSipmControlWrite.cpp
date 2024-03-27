#include "CommandSipmControlWrite.h"

INDEXED_ENUM(RegMap, Register);

constexpr static auto CommandToRegTable = EnumTable<SipmControlWriteCommandIndexer, RegMapIndexer, SipmControlRegister>::make_table(
    std::tuple(SipmControlWriteCommand::HV_ENABLE,                SipmControlRegister::HV_ENABLE),
    std::tuple(SipmControlWriteCommand::MODE,                     SipmControlRegister::MODE),
    std::tuple(SipmControlWriteCommand::V_TARGET_WRITE,           SipmControlRegister::V_TARGET_WRITE),
    std::tuple(SipmControlWriteCommand::RAMP_SPEED,               SipmControlRegister::RAMP_SPEED),
    std::tuple(SipmControlWriteCommand::MAX_V,                    SipmControlRegister::MAX_V),
    std::tuple(SipmControlWriteCommand::MAX_I,                    SipmControlRegister::MAX_I),
    std::tuple(SipmControlWriteCommand::C_TEMP_M2,                SipmControlRegister::C_TEMP_M2),
    std::tuple(SipmControlWriteCommand::C_TEMP_M,                 SipmControlRegister::C_TEMP_M),
    std::tuple(SipmControlWriteCommand::C_TEMP_Q,                 SipmControlRegister::C_TEMP_Q),
    std::tuple(SipmControlWriteCommand::ALFA_VOUT,	              SipmControlRegister::ALFA_VOUT),
    std::tuple(SipmControlWriteCommand::ALFA_IOUT,	              SipmControlRegister::ALFA_IOUT),
    std::tuple(SipmControlWriteCommand::ALFA_VREF,	              SipmControlRegister::ALFA_VREF),
    std::tuple(SipmControlWriteCommand::ALFA_TREF,	              SipmControlRegister::ALFA_TREF),
    std::tuple(SipmControlWriteCommand::TCOEF,                    SipmControlRegister::TCOEF),
    std::tuple(SipmControlWriteCommand::LUT_ENABLE,               SipmControlRegister::LUT_ENABLE),
    std::tuple(SipmControlWriteCommand::ENABLE_PI,                SipmControlRegister::ENABLE_PI),
    std::tuple(SipmControlWriteCommand::EMERGENCY_STOP,           SipmControlRegister::EMERGENCY_STOP),
    std::tuple(SipmControlWriteCommand::IZERO,                    SipmControlRegister::IZERO),
    std::tuple(SipmControlWriteCommand::LUT_ADDRESS,              SipmControlRegister::LUT_ADDRESS),
    std::tuple(SipmControlWriteCommand::LUT_PROGRAM_TEMPERATURE,  SipmControlRegister::LUT_PROGRAM_TEMPERATURE),
    std::tuple(SipmControlWriteCommand::LUT_PROGRAM_OUTPUT_VALUE, SipmControlRegister::LUT_PROGRAM_OUTPUT_VALUE),
    std::tuple(SipmControlWriteCommand::LUT_LENGTH,               SipmControlRegister::LUT_LENGTH),
    std::tuple(SipmControlWriteCommand::I2C_BASE_ADDRESS,         SipmControlRegister::I2C_BASE_ADDRESS),
    std::tuple(SipmControlWriteCommand::CURRENT_RANGE,            SipmControlRegister::CURRENT_RANGE),
    std::tuple(SipmControlWriteCommand::STORE_ON_FLASH,           SipmControlRegister::STORE_ON_FLASH)
);

template <SipmControlWriteCommand reg>
struct ControlRegisterFunctor {
    constexpr static SipmControlRegister control_reg = CommandToRegTable.get<reg, RegMap::Register>();
    constexpr static DataFormat data_type = SipmControlWriteTable.c_get<reg, CommandValues::Types>()[0];
    using value_type = typename format_to_type<data_type>::type;
    
    ErrorCode operator()(Context& context, raw_type raw_data) const {
        value_type value;
        try {
            value = format_to_type<data_type>::convert_raw(raw_data);
        } catch(const std::runtime_error& e) {
            std::cerr << "[ERROR]: Error during type conversion " << e.what() << "\n";
            return ErrorCode::InvalidCommand;
        }

        context.sipm_control.write_register<control_reg, value_type>(value);

        return ErrorCode::Success;
    }
};

ErrorCode CommandSipmControlWrite::execute(Context& context, SipmControlWriteCommand reg) {
    if (raw_data_.size() != 2)
        return ErrorCode::PoorlyStructuredCommand;

    return *SipmControlWriteCommandIndexer::dispatch<ControlRegisterFunctor>(reg, context, raw_data_[1]);
}

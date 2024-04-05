#include "CommandSipmControlWrite.h"

INDEXED_ENUM(RegMap, Register);

constexpr static auto CommandToRegTable = EnumTable<SipmControlWriteCommandIndexer, RegMapIndexer, SipmControlRegister>::make_table(
    std::pair(SipmControlWriteCommand::HV_ENABLE,                std::tuple(SipmControlRegister::HV_ENABLE)),
    std::pair(SipmControlWriteCommand::MODE,                     std::tuple(SipmControlRegister::MODE)),
    std::pair(SipmControlWriteCommand::V_TARGET_WRITE,           std::tuple(SipmControlRegister::V_TARGET_WRITE)),
    std::pair(SipmControlWriteCommand::RAMP_SPEED,               std::tuple(SipmControlRegister::RAMP_SPEED)),
    std::pair(SipmControlWriteCommand::MAX_V,                    std::tuple(SipmControlRegister::MAX_V)),
    std::pair(SipmControlWriteCommand::MAX_I,                    std::tuple(SipmControlRegister::MAX_I)),
    std::pair(SipmControlWriteCommand::C_TEMP_M2,                std::tuple(SipmControlRegister::C_TEMP_M2)),
    std::pair(SipmControlWriteCommand::C_TEMP_M,                 std::tuple(SipmControlRegister::C_TEMP_M)),
    std::pair(SipmControlWriteCommand::C_TEMP_Q,                 std::tuple(SipmControlRegister::C_TEMP_Q)),
    std::pair(SipmControlWriteCommand::ALFA_VOUT,	             std::tuple(SipmControlRegister::ALFA_VOUT)),
    std::pair(SipmControlWriteCommand::ALFA_IOUT,	             std::tuple(SipmControlRegister::ALFA_IOUT)),
    std::pair(SipmControlWriteCommand::ALFA_VREF,	             std::tuple(SipmControlRegister::ALFA_VREF)),
    std::pair(SipmControlWriteCommand::ALFA_TREF,	             std::tuple(SipmControlRegister::ALFA_TREF)),
    std::pair(SipmControlWriteCommand::TCOEF,                    std::tuple(SipmControlRegister::TCOEF)),
    std::pair(SipmControlWriteCommand::LUT_ENABLE,               std::tuple(SipmControlRegister::LUT_ENABLE)),
    std::pair(SipmControlWriteCommand::ENABLE_PI,                std::tuple(SipmControlRegister::ENABLE_PI)),
    std::pair(SipmControlWriteCommand::EMERGENCY_STOP,           std::tuple(SipmControlRegister::EMERGENCY_STOP)),
    std::pair(SipmControlWriteCommand::IZERO,                    std::tuple(SipmControlRegister::IZERO)),
    std::pair(SipmControlWriteCommand::LUT_ADDRESS,              std::tuple(SipmControlRegister::LUT_ADDRESS)),
    std::pair(SipmControlWriteCommand::LUT_PROGRAM_TEMPERATURE,  std::tuple(SipmControlRegister::LUT_PROGRAM_TEMPERATURE)),
    std::pair(SipmControlWriteCommand::LUT_PROGRAM_OUTPUT_VALUE, std::tuple(SipmControlRegister::LUT_PROGRAM_OUTPUT_VALUE)),
    std::pair(SipmControlWriteCommand::LUT_LENGTH,               std::tuple(SipmControlRegister::LUT_LENGTH)),
    std::pair(SipmControlWriteCommand::I2C_BASE_ADDRESS,         std::tuple(SipmControlRegister::I2C_BASE_ADDRESS)),
    std::pair(SipmControlWriteCommand::CURRENT_RANGE,            std::tuple(SipmControlRegister::CURRENT_RANGE)),
    std::pair(SipmControlWriteCommand::STORE_ON_FLASH,           std::tuple(SipmControlRegister::STORE_ON_FLASH))
);

template <SipmControlWriteCommand reg>
struct ControlRegisterFunctor {
    constexpr static SipmControlRegister control_reg = CommandToRegTable.get<reg, RegMap::Register>();
    constexpr static DataFormat data_type = SipmControlWriteTable.c_get<reg, CommandValues::ParameterTypes>()[0];
    using value_type = typename format_to_type<data_type>::type;
    
    ReturnData operator()(Context& context, raw_type raw_data) const {
        value_type value;
        try {
            value = format_to_type<data_type>::convert_raw(raw_data);
        } catch(const std::runtime_error& e) {
            std::cerr << "[ERROR]: Error during type conversion " << e.what() << "\n";
            return ReturnData(ErrorCode::InvalidCommand);
        }

        context.sipm_control->write_register<control_reg, value_type>(value);

        return ReturnData(ErrorCode::Success);
    }
};

ReturnData CommandSipmControlWrite::execute(Context& context, SipmControlWriteCommand reg) {
    if (raw_data_.size() != 2)
        return ReturnData(ErrorCode::PoorlyStructuredCommand);

    if (context.sipm_control_unavailable())
        return ReturnData(ErrorCode::ResourceUnavailable);

    return *SipmControlWriteCommandIndexer::dispatch<ControlRegisterFunctor>(reg, context, raw_data_[1]);
}

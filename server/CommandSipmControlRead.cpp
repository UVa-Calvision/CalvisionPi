#include "CommandSipmControlRead.h"

INDEXED_ENUM(RegMap, Register);

constexpr static auto CommandToRegTable = EnumTable<SipmControlReadCommandIndexer, RegMapIndexer, SipmControlRegister>::make_table(
    std::pair(SipmControlReadCommand::HV_ENABLE,                  std::tuple(SipmControlRegister::HV_ENABLE)),                 
    std::pair(SipmControlReadCommand::MODE,                       std::tuple(SipmControlRegister::MODE)),                      
    std::pair(SipmControlReadCommand::V_TARGET_WRITE,             std::tuple(SipmControlRegister::V_TARGET_WRITE)),            
    std::pair(SipmControlReadCommand::RAMP_SPEED,                 std::tuple(SipmControlRegister::RAMP_SPEED)),                
    std::pair(SipmControlReadCommand::MAX_V,                      std::tuple(SipmControlRegister::MAX_V)),                     
    std::pair(SipmControlReadCommand::MAX_I,                      std::tuple(SipmControlRegister::MAX_I)),                     
    std::pair(SipmControlReadCommand::C_TEMP_M2,                  std::tuple(SipmControlRegister::C_TEMP_M2)),                 
    std::pair(SipmControlReadCommand::C_TEMP_M,                   std::tuple(SipmControlRegister::C_TEMP_M)),                  
    std::pair(SipmControlReadCommand::C_TEMP_Q,                   std::tuple(SipmControlRegister::C_TEMP_Q)),                  
    std::pair(SipmControlReadCommand::ALFA_VOUT,	              std::tuple(SipmControlRegister::ALFA_VOUT)),	                
    std::pair(SipmControlReadCommand::ALFA_IOUT,	              std::tuple(SipmControlRegister::ALFA_IOUT)),	                
    std::pair(SipmControlReadCommand::ALFA_VREF,	              std::tuple(SipmControlRegister::ALFA_VREF)),	                
    std::pair(SipmControlReadCommand::ALFA_TREF,	              std::tuple(SipmControlRegister::ALFA_TREF)),	                
    std::pair(SipmControlReadCommand::TCOEF,                      std::tuple(SipmControlRegister::TCOEF)),                     
    std::pair(SipmControlReadCommand::LUT_ENABLE,                 std::tuple(SipmControlRegister::LUT_ENABLE)),                
    std::pair(SipmControlReadCommand::ENABLE_PI,                  std::tuple(SipmControlRegister::ENABLE_PI)),                 
    std::pair(SipmControlReadCommand::LUT_ADDRESS,                std::tuple(SipmControlRegister::LUT_ADDRESS)),               
    std::pair(SipmControlReadCommand::LUT_PROGRAM_TEMPERATURE,    std::tuple(SipmControlRegister::LUT_PROGRAM_TEMPERATURE)),   
    std::pair(SipmControlReadCommand::LUT_PROGRAM_OUTPUT_VALUE,   std::tuple(SipmControlRegister::LUT_PROGRAM_OUTPUT_VALUE)),  
    std::pair(SipmControlReadCommand::LUT_LENGTH,                 std::tuple(SipmControlRegister::LUT_LENGTH)),                
    std::pair(SipmControlReadCommand::I2C_BASE_ADDRESS,           std::tuple(SipmControlRegister::I2C_BASE_ADDRESS)),          
    std::pair(SipmControlReadCommand::CURRENT_RANGE,              std::tuple(SipmControlRegister::CURRENT_RANGE)),             
    std::pair(SipmControlReadCommand::PIN_STATUS,                 std::tuple(SipmControlRegister::PIN_STATUS)),                
    std::pair(SipmControlReadCommand::VIN,                        std::tuple(SipmControlRegister::VIN)),                       
    std::pair(SipmControlReadCommand::VOUT,	                      std::tuple(SipmControlRegister::VOUT)),	                    
    std::pair(SipmControlReadCommand::IOUT,	                      std::tuple(SipmControlRegister::IOUT)),	                    
    std::pair(SipmControlReadCommand::VREF,	                      std::tuple(SipmControlRegister::VREF)),	                    
    std::pair(SipmControlReadCommand::TREF,	                      std::tuple(SipmControlRegister::TREF)),	                    
    std::pair(SipmControlReadCommand::V_TARGET_READ,              std::tuple(SipmControlRegister::V_TARGET_READ)),             
    std::pair(SipmControlReadCommand::R_TARGET,                   std::tuple(SipmControlRegister::R_TARGET)),                  
    std::pair(SipmControlReadCommand::CVT,                        std::tuple(SipmControlRegister::CVT)),                       
    std::pair(SipmControlReadCommand::COMPLIANCE_V,               std::tuple(SipmControlRegister::COMPLIANCE_V)),              
    std::pair(SipmControlReadCommand::COMPLIANCE_I,               std::tuple(SipmControlRegister::COMPLIANCE_I)),              
    std::pair(SipmControlReadCommand::PRODUCT_CODE,               std::tuple(SipmControlRegister::PRODUCT_CODE)),              
    std::pair(SipmControlReadCommand::FW_VERSION,                 std::tuple(SipmControlRegister::FW_VERSION)),                
    std::pair(SipmControlReadCommand::HW_VERSION,                 std::tuple(SipmControlRegister::HW_VERSION)),                
    std::pair(SipmControlReadCommand::SERIAL_NUMBER,              std::tuple(SipmControlRegister::SERIAL_NUMBER))             
);

template <SipmControlReadCommand reg>
struct ControlRegisterFunctor {
    constexpr static SipmControlRegister control_reg = CommandToRegTable.get<reg, RegMap::Register>();
    constexpr static DataFormat data_type = *SipmControlReadTable.c_get<reg, CommandValues::ReturnType>();
    using value_type = typename format_to_type<data_type>::type;
    
    ReturnData operator()(Context& context) const {
        try {
            value_type value = context.sipm_control.read_register<control_reg, value_type>();
            std::cout << "[SipmControlRead] Read value: " << value << "\n";
            return ReturnData(ErrorCode::Success, binary_cast<raw_type>(value));
        } catch(const std::runtime_error& e) {
            std::cerr << "[ERROR]: Error during type conversion " << e.what() << "\n";
            return ReturnData(ErrorCode::InvalidCommand);
        }
    }
};

ReturnData CommandSipmControlRead::execute(Context& context, SipmControlReadCommand reg) {
    if (raw_data_.size() < 1)
        return ReturnData(ErrorCode::PoorlyStructuredCommand);

    return *SipmControlReadCommandIndexer::dispatch<ControlRegisterFunctor>(reg, context);
}

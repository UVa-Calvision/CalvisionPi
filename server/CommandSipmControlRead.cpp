#include "CommandSipmControlRead.h"

INDEXED_ENUM(RegMap, Register);

constexpr static auto CommandToRegTable = EnumTable<SipmControlReadCommandIndexer, RegMapIndexer, SipmControlRegister>::make_table(
    std::tuple(SipmControlReadCommand::HV_ENABLE,                  SipmControlRegister::HV_ENABLE),                 
    std::tuple(SipmControlReadCommand::MODE,                       SipmControlRegister::MODE),                      
    std::tuple(SipmControlReadCommand::V_TARGET_WRITE,             SipmControlRegister::V_TARGET_WRITE),            
    std::tuple(SipmControlReadCommand::RAMP_SPEED,                 SipmControlRegister::RAMP_SPEED),                
    std::tuple(SipmControlReadCommand::MAX_V,                      SipmControlRegister::MAX_V),                     
    std::tuple(SipmControlReadCommand::MAX_I,                      SipmControlRegister::MAX_I),                     
    std::tuple(SipmControlReadCommand::C_TEMP_M2,                  SipmControlRegister::C_TEMP_M2),                 
    std::tuple(SipmControlReadCommand::C_TEMP_M,                   SipmControlRegister::C_TEMP_M),                  
    std::tuple(SipmControlReadCommand::C_TEMP_Q,                   SipmControlRegister::C_TEMP_Q),                  
    std::tuple(SipmControlReadCommand::ALFA_VOUT,	               SipmControlRegister::ALFA_VOUT),	                
    std::tuple(SipmControlReadCommand::ALFA_IOUT,	               SipmControlRegister::ALFA_IOUT),	                
    std::tuple(SipmControlReadCommand::ALFA_VREF,	               SipmControlRegister::ALFA_VREF),	                
    std::tuple(SipmControlReadCommand::ALFA_TREF,	               SipmControlRegister::ALFA_TREF),	                
    std::tuple(SipmControlReadCommand::TCOEF,                      SipmControlRegister::TCOEF),                     
    std::tuple(SipmControlReadCommand::LUT_ENABLE,                 SipmControlRegister::LUT_ENABLE),                
    std::tuple(SipmControlReadCommand::ENABLE_PI,                  SipmControlRegister::ENABLE_PI),                 
    std::tuple(SipmControlReadCommand::LUT_ADDRESS,                SipmControlRegister::LUT_ADDRESS),               
    std::tuple(SipmControlReadCommand::LUT_PROGRAM_TEMPERATURE,    SipmControlRegister::LUT_PROGRAM_TEMPERATURE),   
    std::tuple(SipmControlReadCommand::LUT_PROGRAM_OUTPUT_VALUE,   SipmControlRegister::LUT_PROGRAM_OUTPUT_VALUE),  
    std::tuple(SipmControlReadCommand::LUT_LENGTH,                 SipmControlRegister::LUT_LENGTH),                
    std::tuple(SipmControlReadCommand::I2C_BASE_ADDRESS,           SipmControlRegister::I2C_BASE_ADDRESS),          
    std::tuple(SipmControlReadCommand::CURRENT_RANGE,              SipmControlRegister::CURRENT_RANGE),             
    std::tuple(SipmControlReadCommand::PIN_STATUS,                 SipmControlRegister::PIN_STATUS),                
    std::tuple(SipmControlReadCommand::VIN,                        SipmControlRegister::VIN),                       
    std::tuple(SipmControlReadCommand::VOUT,	                   SipmControlRegister::VOUT),	                    
    std::tuple(SipmControlReadCommand::IOUT,	                   SipmControlRegister::IOUT),	                    
    std::tuple(SipmControlReadCommand::VREF,	                   SipmControlRegister::VREF),	                    
    std::tuple(SipmControlReadCommand::TREF,	                   SipmControlRegister::TREF),	                    
    std::tuple(SipmControlReadCommand::V_TARGET_READ,              SipmControlRegister::V_TARGET_READ),             
    std::tuple(SipmControlReadCommand::R_TARGET,                   SipmControlRegister::R_TARGET),                  
    std::tuple(SipmControlReadCommand::CVT,                        SipmControlRegister::CVT),                       
    std::tuple(SipmControlReadCommand::COMPLIANCE_V,               SipmControlRegister::COMPLIANCE_V),              
    std::tuple(SipmControlReadCommand::COMPLIANCE_I,               SipmControlRegister::COMPLIANCE_I),              
    std::tuple(SipmControlReadCommand::PRODUCT_CODE,               SipmControlRegister::PRODUCT_CODE),              
    std::tuple(SipmControlReadCommand::FW_VERSION,                 SipmControlRegister::FW_VERSION),                
    std::tuple(SipmControlReadCommand::HW_VERSION,                 SipmControlRegister::HW_VERSION),                
    std::tuple(SipmControlReadCommand::SERIAL_NUMBER,              SipmControlRegister::SERIAL_NUMBER)             
);

template <SipmControlReadCommand reg>
struct ControlRegisterFunctor {
    constexpr static SipmControlRegister control_reg = CommandToRegTable.get<reg, RegMap::Register>();
    constexpr static DataFormat data_type = SipmControlReadTable.c_get<reg, CommandValues::Types>()[0];
    using value_type = typename format_to_type<data_type>::type;
    
    ErrorCode operator()(Context& context) const {
        try {
            value_type value = context.sipm_control.read_register<control_reg, value_type>();
            std::cout << "[SipmControlRead] Read value: " << value << "\n";
        } catch(const std::runtime_error& e) {
            std::cerr << "[ERROR]: Error during type conversion " << e.what() << "\n";
            return ErrorCode::InvalidCommand;
        }

        return ErrorCode::Success;
    }
};

ErrorCode CommandSipmControlRead::execute(Context& context, SipmControlReadCommand reg) {
    if (raw_data_.size() < 1)
        return ErrorCode::PoorlyStructuredCommand;

    return *SipmControlReadCommandIndexer::dispatch<ControlRegisterFunctor>(reg, context);
}

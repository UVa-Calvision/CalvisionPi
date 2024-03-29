#pragma once

#include "CppUtils/c_util/Enum.h"
#include <string_view>
#include "DataFormat.h"

INDEXED_ENUM(SipmControlRegister,
    HV_ENABLE,
    MODE,
    V_TARGET_WRITE,
    RAMP_SPEED,
    MAX_V,
    MAX_I,
    C_TEMP_M2,
    C_TEMP_M,
    C_TEMP_Q,
    ALFA_VOUT,
    ALFA_IOUT,
    ALFA_VREF,
    ALFA_TREF,
    TCOEF,
    LUT_ENABLE,
    ENABLE_PI,
    EMERGENCY_STOP,
    IZERO,
    LUT_ADDRESS,
    LUT_PROGRAM_TEMPERATURE,
    LUT_PROGRAM_OUTPUT_VALUE,
    LUT_LENGTH,
    I2C_BASE_ADDRESS,
    CURRENT_RANGE,
    PIN_STATUS,
    VIN,
    VOUT,
    IOUT,
    VREF,
    TREF,
    V_TARGET_READ,
    R_TARGET,
    CVT,
    COMPLIANCE_V,
    COMPLIANCE_I,
    PRODUCT_CODE,
    FW_VERSION,
    HW_VERSION,
    SERIAL_NUMBER,
    STORE_ON_FLASH
);


INDEXED_ENUM(SipmRegisterValue,
    Name,
    Register,
    Type,
    Can_Read,
    Can_Write
);

constexpr static auto SipmRegisterTable = EnumTable<SipmControlRegisterIndexer, SipmRegisterValueIndexer, std::string_view, uint8_t, DataFormat, bool, bool>::make_table(
    std::make_pair(SipmControlRegister::HV_ENABLE,                  std::tuple("HV_ENABLE",                  0, DataFormat::Bool ,  true,  true)),
    std::make_pair(SipmControlRegister::MODE,                       std::tuple("MODE",                       1, DataFormat::Int  ,  true,  true)),
    std::make_pair(SipmControlRegister::V_TARGET_WRITE,             std::tuple("V_TARGET_WRITE",             2, DataFormat::Float,  true,  true)),
    std::make_pair(SipmControlRegister::RAMP_SPEED,                 std::tuple("RAMP_SPEED",                 3, DataFormat::Float,  true,  true)),
    std::make_pair(SipmControlRegister::MAX_V,                      std::tuple("MAX_V",                      4, DataFormat::Float,  true,  true)),
    std::make_pair(SipmControlRegister::MAX_I,                      std::tuple("MAX_I",                      5, DataFormat::Float,  true,  true)),
    std::make_pair(SipmControlRegister::C_TEMP_M2,                  std::tuple("C_TEMP_M2",                  7, DataFormat::Float,  true,  true)),
    std::make_pair(SipmControlRegister::C_TEMP_M,                   std::tuple("C_TEMP_M",                   8, DataFormat::Float,  true,  true)),
    std::make_pair(SipmControlRegister::C_TEMP_Q,                   std::tuple("C_TEMP_Q",                   9, DataFormat::Float,  true,  true)),
    std::make_pair(SipmControlRegister::ALFA_VOUT,	                std::tuple("ALFA_VOUT",	                10, DataFormat::Float,  true,  true)),
    std::make_pair(SipmControlRegister::ALFA_IOUT,	                std::tuple("ALFA_IOUT",	                11, DataFormat::Float,  true,  true)),
    std::make_pair(SipmControlRegister::ALFA_VREF,	                std::tuple("ALFA_VREF",	                12, DataFormat::Float,  true,  true)),
    std::make_pair(SipmControlRegister::ALFA_TREF,	                std::tuple("ALFA_TREF",	                13, DataFormat::Float,  true,  true)),
    std::make_pair(SipmControlRegister::TCOEF,                      std::tuple("TCOEF",                     28, DataFormat::Float,  true,  true)),
    std::make_pair(SipmControlRegister::LUT_ENABLE,                 std::tuple("LUT_ENABLE",                29, DataFormat::Bool ,  true,  true)),
    std::make_pair(SipmControlRegister::ENABLE_PI,                  std::tuple("ENABLE_PI",                 30, DataFormat::Bool ,  true,  true)),
    std::make_pair(SipmControlRegister::EMERGENCY_STOP,             std::tuple("EMERGENCY_STOP",            31, DataFormat::Bool , false,  true)),
    std::make_pair(SipmControlRegister::IZERO,                      std::tuple("IZERO",                     32, DataFormat::Bool , false,  true)),
    std::make_pair(SipmControlRegister::LUT_ADDRESS,                std::tuple("LUT_ADDRESS",               36, DataFormat::Int  ,  true,  true)),
    std::make_pair(SipmControlRegister::LUT_PROGRAM_TEMPERATURE,    std::tuple("LUT_PROGRAM_TEMPERATURE",   37, DataFormat::Float,  true,  true)),
    std::make_pair(SipmControlRegister::LUT_PROGRAM_OUTPUT_VALUE,   std::tuple("LUT_PROGRAM_OUTPUT_VALUE",  38, DataFormat::Float,  true,  true)),
    std::make_pair(SipmControlRegister::LUT_LENGTH,                 std::tuple("LUT_LENGTH",                39, DataFormat::Int  ,  true,  true)),
    std::make_pair(SipmControlRegister::I2C_BASE_ADDRESS,           std::tuple("I2C_BASE_ADDRESS",          40, DataFormat::Int  ,  true,  true)),
    std::make_pair(SipmControlRegister::CURRENT_RANGE,              std::tuple("CURRENT_RANGE",             81, DataFormat::Int  ,  true,  true)),
    std::make_pair(SipmControlRegister::PIN_STATUS,                 std::tuple("PIN_STATUS",               229, DataFormat::Int  ,  true, false)),
    std::make_pair(SipmControlRegister::VIN,                        std::tuple("VIN",                      230, DataFormat::Float,  true, false)),
    std::make_pair(SipmControlRegister::VOUT,	                    std::tuple("VOUT",	                   231, DataFormat::Float,  true, false)),
    std::make_pair(SipmControlRegister::IOUT,	                    std::tuple("IOUT",	                   232, DataFormat::Float,  true, false)),
    std::make_pair(SipmControlRegister::VREF,	                    std::tuple("VREF",	                   233, DataFormat::Float,  true, false)),
    std::make_pair(SipmControlRegister::TREF,	                    std::tuple("TREF",	                   234, DataFormat::Float,  true, false)),
    std::make_pair(SipmControlRegister::V_TARGET_READ,              std::tuple("V_TARGET_READ",            235, DataFormat::Float,  true, false)),
    std::make_pair(SipmControlRegister::R_TARGET,                   std::tuple("R_TARGET",                 236, DataFormat::Float,  true, false)),
    std::make_pair(SipmControlRegister::CVT,                        std::tuple("CVT",                      237, DataFormat::Float,  true, false)),
    std::make_pair(SipmControlRegister::COMPLIANCE_V,               std::tuple("COMPLIANCE_V",             249, DataFormat::Bool ,  true, false)),
    std::make_pair(SipmControlRegister::COMPLIANCE_I,               std::tuple("COMPLIANCE_I",             250, DataFormat::Bool ,  true, false)),
    std::make_pair(SipmControlRegister::PRODUCT_CODE,               std::tuple("PRODUCT_CODE",             251, DataFormat::Int  ,  true, false)),
    std::make_pair(SipmControlRegister::FW_VERSION,                 std::tuple("FW_VERSION",               252, DataFormat::Float,  true, false)),
    std::make_pair(SipmControlRegister::HW_VERSION,                 std::tuple("HW_VERSION",               253, DataFormat::Float,  true, false)),
    std::make_pair(SipmControlRegister::SERIAL_NUMBER,              std::tuple("SERIAL_NUMBER",            254, DataFormat::Int  ,  true, false)),
    std::make_pair(SipmControlRegister::STORE_ON_FLASH,             std::tuple("STORE_ON_FLASH",           255, DataFormat::Bool , false,  true))
);

template <typename T, SipmControlRegister reg>
constexpr static bool valid_register_type() {
    switch (SipmRegisterTable.get<reg, SipmRegisterValue::Type>()) {
        case DataFormat::Bool:
            return std::is_same_v<T,bool>;
        case DataFormat::Int:
            return std::is_same_v<T,int32_t> || std::is_same_v<T,uint32_t>;
        case DataFormat::Float:
            return std::is_same_v<T,float>;
        default:
            return false;
    }
}



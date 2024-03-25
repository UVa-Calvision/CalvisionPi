#pragma once

#include "CppUtils/c_util/Enum.h"

enum class SipmControlRegister {
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
    STORE_ON_FLASH,
};

enum class SipmRegisterType {
    Bool,
    Int,
    Float
};

using SipmRegisterIndexer = EnumIndexer<SipmControlRegister,
    SipmControlRegister::HV_ENABLE,
    SipmControlRegister::MODE,
    SipmControlRegister::V_TARGET_WRITE,
    SipmControlRegister::RAMP_SPEED,
    SipmControlRegister::MAX_V,
    SipmControlRegister::MAX_I,
    SipmControlRegister::C_TEMP_M2,
    SipmControlRegister::C_TEMP_M,
    SipmControlRegister::C_TEMP_Q,
    SipmControlRegister::ALFA_VOUT,
    SipmControlRegister::ALFA_IOUT,
    SipmControlRegister::ALFA_VREF,
    SipmControlRegister::ALFA_TREF,
    SipmControlRegister::TCOEF,
    SipmControlRegister::LUT_ENABLE,
    SipmControlRegister::ENABLE_PI,
    SipmControlRegister::EMERGENCY_STOP,
    SipmControlRegister::IZERO,
    SipmControlRegister::LUT_ADDRESS,
    SipmControlRegister::LUT_PROGRAM_TEMPERATURE,
    SipmControlRegister::LUT_PROGRAM_OUTPUT_VALUE,
    SipmControlRegister::LUT_LENGTH,
    SipmControlRegister::I2C_BASE_ADDRESS,
    SipmControlRegister::CURRENT_RANGE,
    SipmControlRegister::PIN_STATUS,
    SipmControlRegister::VIN,
    SipmControlRegister::VOUT,
    SipmControlRegister::IOUT,
    SipmControlRegister::VREF,
    SipmControlRegister::TREF,
    SipmControlRegister::V_TARGET_READ,
    SipmControlRegister::R_TARGET,
    SipmControlRegister::CVT,
    SipmControlRegister::COMPLIANCE_V,
    SipmControlRegister::COMPLIANCE_I,
    SipmControlRegister::PRODUCT_CODE,
    SipmControlRegister::FW_VERSION,
    SipmControlRegister::HW_VERSION,
    SipmControlRegister::SERIAL_NUMBER,
    SipmControlRegister::STORE_ON_FLASH
>;

enum class SipmRegisterValue : size_t {
    Name,
    Register,
    Type,
    Can_Read,
    Can_Write,
};

using SipmRegisterValueIndexer = EnumIndexer<SipmRegisterValue,
    SipmRegisterValue::Name,
    SipmRegisterValue::Register,
    SipmRegisterValue::Type,
    SipmRegisterValue::Can_Read,
    SipmRegisterValue::Can_Write
>;

constexpr static auto SipmRegisterTable = EnumTable<SipmRegisterIndexer, SipmRegisterValueIndexer, const char*, uint8_t, SipmRegisterType, bool, bool>::make_table(
    std::tuple(SipmControlRegister::HV_ENABLE,                  "HV_ENABLE",                  0, SipmRegisterType::Bool ,  true,  true),
    std::tuple(SipmControlRegister::MODE,                       "MODE",                       1, SipmRegisterType::Int  ,  true,  true),
    std::tuple(SipmControlRegister::V_TARGET_WRITE,             "V_TARGET_WRITE",             2, SipmRegisterType::Float,  true,  true),
    std::tuple(SipmControlRegister::RAMP_SPEED,                 "RAMP_SPEED",                 3, SipmRegisterType::Float,  true,  true),
    std::tuple(SipmControlRegister::MAX_V,                      "MAX_V",                      4, SipmRegisterType::Float,  true,  true),
    std::tuple(SipmControlRegister::MAX_I,                      "MAX_I",                      5, SipmRegisterType::Float,  true,  true),
    std::tuple(SipmControlRegister::C_TEMP_M2,                  "C_TEMP_M2",                  7, SipmRegisterType::Float,  true,  true),
    std::tuple(SipmControlRegister::C_TEMP_M,                   "C_TEMP_M",                   8, SipmRegisterType::Float,  true,  true),
    std::tuple(SipmControlRegister::C_TEMP_Q,                   "C_TEMP_Q",                   9, SipmRegisterType::Float,  true,  true),
    std::tuple(SipmControlRegister::ALFA_VOUT,	                "ALFA_VOUT",	             10, SipmRegisterType::Float,  true,  true),
    std::tuple(SipmControlRegister::ALFA_IOUT,	                "ALFA_IOUT",	             11, SipmRegisterType::Float,  true,  true),
    std::tuple(SipmControlRegister::ALFA_VREF,	                "ALFA_VREF",	             12, SipmRegisterType::Float,  true,  true),
    std::tuple(SipmControlRegister::ALFA_TREF,	                "ALFA_TREF",	             13, SipmRegisterType::Float,  true,  true),
    std::tuple(SipmControlRegister::TCOEF,                      "TCOEF",                     28, SipmRegisterType::Float,  true,  true),
    std::tuple(SipmControlRegister::LUT_ENABLE,                 "LUT_ENABLE",                29, SipmRegisterType::Bool ,  true,  true),
    std::tuple(SipmControlRegister::ENABLE_PI,                  "ENABLE_PI",                 30, SipmRegisterType::Bool ,  true,  true),
    std::tuple(SipmControlRegister::EMERGENCY_STOP,             "EMERGENCY_STOP",            31, SipmRegisterType::Bool , false,  true),
    std::tuple(SipmControlRegister::IZERO,                      "IZERO",                     32, SipmRegisterType::Bool , false,  true),
    std::tuple(SipmControlRegister::LUT_ADDRESS,                "LUT_ADDRESS",               36, SipmRegisterType::Int  ,  true,  true),
    std::tuple(SipmControlRegister::LUT_PROGRAM_TEMPERATURE,    "LUT_PROGRAM_TEMPERATURE",   37, SipmRegisterType::Float,  true,  true),
    std::tuple(SipmControlRegister::LUT_PROGRAM_OUTPUT_VALUE,   "LUT_PROGRAM_OUTPUT_VALUE",  38, SipmRegisterType::Float,  true,  true),
    std::tuple(SipmControlRegister::LUT_LENGTH,                 "LUT_LENGTH",                39, SipmRegisterType::Int  ,  true,  true),
    std::tuple(SipmControlRegister::I2C_BASE_ADDRESS,           "I2C_BASE_ADDRESS",          40, SipmRegisterType::Int  ,  true,  true),
    std::tuple(SipmControlRegister::CURRENT_RANGE,              "CURRENT_RANGE",             81, SipmRegisterType::Int  ,  true,  true),
    std::tuple(SipmControlRegister::PIN_STATUS,                 "PIN_STATUS",               229, SipmRegisterType::Int  ,  true, false),
    std::tuple(SipmControlRegister::VIN,                        "VIN",                      230, SipmRegisterType::Float,  true, false),
    std::tuple(SipmControlRegister::VOUT,	                    "VOUT",	                    231, SipmRegisterType::Float,  true, false),
    std::tuple(SipmControlRegister::IOUT,	                    "IOUT",	                    232, SipmRegisterType::Float,  true, false),
    std::tuple(SipmControlRegister::VREF,	                    "VREF",	                    233, SipmRegisterType::Float,  true, false),
    std::tuple(SipmControlRegister::TREF,	                    "TREF",	                    234, SipmRegisterType::Float,  true, false),
    std::tuple(SipmControlRegister::V_TARGET_READ,              "V_TARGET_READ",            235, SipmRegisterType::Float,  true, false),
    std::tuple(SipmControlRegister::R_TARGET,                   "R_TARGET",                 236, SipmRegisterType::Float,  true, false),
    std::tuple(SipmControlRegister::CVT,                        "CVT",                      237, SipmRegisterType::Float,  true, false),
    std::tuple(SipmControlRegister::COMPLIANCE_V,               "COMPLIANCE_V",             249, SipmRegisterType::Bool ,  true, false),
    std::tuple(SipmControlRegister::COMPLIANCE_I,               "COMPLIANCE_I",             250, SipmRegisterType::Bool ,  true, false),
    std::tuple(SipmControlRegister::PRODUCT_CODE,               "PRODUCT_CODE",             251, SipmRegisterType::Int  ,  true, false),
    std::tuple(SipmControlRegister::FW_VERSION,                 "FW_VERSION",               252, SipmRegisterType::Float,  true, false),
    std::tuple(SipmControlRegister::HW_VERSION,                 "HW_VERSION",               253, SipmRegisterType::Float,  true, false),
    std::tuple(SipmControlRegister::SERIAL_NUMBER,              "SERIAL_NUMBER",            254, SipmRegisterType::Int  ,  true, false),
    std::tuple(SipmControlRegister::STORE_ON_FLASH,             "STORE_ON_FLASH",           255, SipmRegisterType::Bool , false,  true)
);

template <typename T, SipmControlRegister reg>
constexpr static bool valid_register_type() {
    if (sizeof(T) != 4)
        return false;

    switch (SipmRegisterTable.get<SipmRegisterValue::Type>().get<reg>()) {
        case SipmRegisterType::Bool:
            return std::is_same_v<T,bool>;
        case SipmRegisterType::Int:
            return std::is_same_v<T,int32_t> || std::is_same_v<T,uint32_t>;
        case SipmRegisterType::Float:
            return std::is_same_v<T,float>;
        default:
            return false;
    }
}



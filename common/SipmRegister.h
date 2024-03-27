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
    std::tuple(SipmControlRegister::HV_ENABLE,                  "HV_ENABLE",                  0, DataFormat::Bool ,  true,  true),
    std::tuple(SipmControlRegister::MODE,                       "MODE",                       1, DataFormat::Int  ,  true,  true),
    std::tuple(SipmControlRegister::V_TARGET_WRITE,             "V_TARGET_WRITE",             2, DataFormat::Float,  true,  true),
    std::tuple(SipmControlRegister::RAMP_SPEED,                 "RAMP_SPEED",                 3, DataFormat::Float,  true,  true),
    std::tuple(SipmControlRegister::MAX_V,                      "MAX_V",                      4, DataFormat::Float,  true,  true),
    std::tuple(SipmControlRegister::MAX_I,                      "MAX_I",                      5, DataFormat::Float,  true,  true),
    std::tuple(SipmControlRegister::C_TEMP_M2,                  "C_TEMP_M2",                  7, DataFormat::Float,  true,  true),
    std::tuple(SipmControlRegister::C_TEMP_M,                   "C_TEMP_M",                   8, DataFormat::Float,  true,  true),
    std::tuple(SipmControlRegister::C_TEMP_Q,                   "C_TEMP_Q",                   9, DataFormat::Float,  true,  true),
    std::tuple(SipmControlRegister::ALFA_VOUT,	                "ALFA_VOUT",	             10, DataFormat::Float,  true,  true),
    std::tuple(SipmControlRegister::ALFA_IOUT,	                "ALFA_IOUT",	             11, DataFormat::Float,  true,  true),
    std::tuple(SipmControlRegister::ALFA_VREF,	                "ALFA_VREF",	             12, DataFormat::Float,  true,  true),
    std::tuple(SipmControlRegister::ALFA_TREF,	                "ALFA_TREF",	             13, DataFormat::Float,  true,  true),
    std::tuple(SipmControlRegister::TCOEF,                      "TCOEF",                     28, DataFormat::Float,  true,  true),
    std::tuple(SipmControlRegister::LUT_ENABLE,                 "LUT_ENABLE",                29, DataFormat::Bool ,  true,  true),
    std::tuple(SipmControlRegister::ENABLE_PI,                  "ENABLE_PI",                 30, DataFormat::Bool ,  true,  true),
    std::tuple(SipmControlRegister::EMERGENCY_STOP,             "EMERGENCY_STOP",            31, DataFormat::Bool , false,  true),
    std::tuple(SipmControlRegister::IZERO,                      "IZERO",                     32, DataFormat::Bool , false,  true),
    std::tuple(SipmControlRegister::LUT_ADDRESS,                "LUT_ADDRESS",               36, DataFormat::Int  ,  true,  true),
    std::tuple(SipmControlRegister::LUT_PROGRAM_TEMPERATURE,    "LUT_PROGRAM_TEMPERATURE",   37, DataFormat::Float,  true,  true),
    std::tuple(SipmControlRegister::LUT_PROGRAM_OUTPUT_VALUE,   "LUT_PROGRAM_OUTPUT_VALUE",  38, DataFormat::Float,  true,  true),
    std::tuple(SipmControlRegister::LUT_LENGTH,                 "LUT_LENGTH",                39, DataFormat::Int  ,  true,  true),
    std::tuple(SipmControlRegister::I2C_BASE_ADDRESS,           "I2C_BASE_ADDRESS",          40, DataFormat::Int  ,  true,  true),
    std::tuple(SipmControlRegister::CURRENT_RANGE,              "CURRENT_RANGE",             81, DataFormat::Int  ,  true,  true),
    std::tuple(SipmControlRegister::PIN_STATUS,                 "PIN_STATUS",               229, DataFormat::Int  ,  true, false),
    std::tuple(SipmControlRegister::VIN,                        "VIN",                      230, DataFormat::Float,  true, false),
    std::tuple(SipmControlRegister::VOUT,	                    "VOUT",	                    231, DataFormat::Float,  true, false),
    std::tuple(SipmControlRegister::IOUT,	                    "IOUT",	                    232, DataFormat::Float,  true, false),
    std::tuple(SipmControlRegister::VREF,	                    "VREF",	                    233, DataFormat::Float,  true, false),
    std::tuple(SipmControlRegister::TREF,	                    "TREF",	                    234, DataFormat::Float,  true, false),
    std::tuple(SipmControlRegister::V_TARGET_READ,              "V_TARGET_READ",            235, DataFormat::Float,  true, false),
    std::tuple(SipmControlRegister::R_TARGET,                   "R_TARGET",                 236, DataFormat::Float,  true, false),
    std::tuple(SipmControlRegister::CVT,                        "CVT",                      237, DataFormat::Float,  true, false),
    std::tuple(SipmControlRegister::COMPLIANCE_V,               "COMPLIANCE_V",             249, DataFormat::Bool ,  true, false),
    std::tuple(SipmControlRegister::COMPLIANCE_I,               "COMPLIANCE_I",             250, DataFormat::Bool ,  true, false),
    std::tuple(SipmControlRegister::PRODUCT_CODE,               "PRODUCT_CODE",             251, DataFormat::Int  ,  true, false),
    std::tuple(SipmControlRegister::FW_VERSION,                 "FW_VERSION",               252, DataFormat::Float,  true, false),
    std::tuple(SipmControlRegister::HW_VERSION,                 "HW_VERSION",               253, DataFormat::Float,  true, false),
    std::tuple(SipmControlRegister::SERIAL_NUMBER,              "SERIAL_NUMBER",            254, DataFormat::Int  ,  true, false),
    std::tuple(SipmControlRegister::STORE_ON_FLASH,             "STORE_ON_FLASH",           255, DataFormat::Bool , false,  true)
);

template <typename T, SipmControlRegister reg>
constexpr static bool valid_register_type() {
    switch (SipmRegisterTable.get<SipmRegisterValue::Type>().get<reg>()) {
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



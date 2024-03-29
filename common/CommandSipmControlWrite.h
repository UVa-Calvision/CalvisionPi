#pragma once

#include "Command.h"
#include "SipmRegister.h"

INDEXED_ENUM(SipmControlWriteCommand,
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
    STORE_ON_FLASH          
);

constexpr static auto SipmControlWriteTable = CommandEnumTable<SipmControlWriteCommandIndexer, 1>::make_table(
    std::pair(SipmControlWriteCommand::HV_ENABLE,                  std::tuple("HV_ENABLE",                DataFormatTypes<1>{DataFormat::Bool }, std::nullopt)),
    std::pair(SipmControlWriteCommand::MODE,                       std::tuple("MODE",                     DataFormatTypes<1>{DataFormat::Int  }, std::nullopt)),
    std::pair(SipmControlWriteCommand::V_TARGET_WRITE,             std::tuple("V_TARGET_WRITE",           DataFormatTypes<1>{DataFormat::Float}, std::nullopt)),
    std::pair(SipmControlWriteCommand::RAMP_SPEED,                 std::tuple("RAMP_SPEED",               DataFormatTypes<1>{DataFormat::Float}, std::nullopt)),
    std::pair(SipmControlWriteCommand::MAX_V,                      std::tuple("MAX_V",                    DataFormatTypes<1>{DataFormat::Float}, std::nullopt)),
    std::pair(SipmControlWriteCommand::MAX_I,                      std::tuple("MAX_I",                    DataFormatTypes<1>{DataFormat::Float}, std::nullopt)),
    std::pair(SipmControlWriteCommand::C_TEMP_M2,                  std::tuple("C_TEMP_M2",                DataFormatTypes<1>{DataFormat::Float}, std::nullopt)),
    std::pair(SipmControlWriteCommand::C_TEMP_M,                   std::tuple("C_TEMP_M",                 DataFormatTypes<1>{DataFormat::Float}, std::nullopt)),
    std::pair(SipmControlWriteCommand::C_TEMP_Q,                   std::tuple("C_TEMP_Q",                 DataFormatTypes<1>{DataFormat::Float}, std::nullopt)),
    std::pair(SipmControlWriteCommand::ALFA_VOUT,	               std::tuple("ALFA_VOUT",	              DataFormatTypes<1>{DataFormat::Float}, std::nullopt)),
    std::pair(SipmControlWriteCommand::ALFA_IOUT,	               std::tuple("ALFA_IOUT",	              DataFormatTypes<1>{DataFormat::Float}, std::nullopt)),
    std::pair(SipmControlWriteCommand::ALFA_VREF,	               std::tuple("ALFA_VREF",	              DataFormatTypes<1>{DataFormat::Float}, std::nullopt)),
    std::pair(SipmControlWriteCommand::ALFA_TREF,	               std::tuple("ALFA_TREF",	              DataFormatTypes<1>{DataFormat::Float}, std::nullopt)),
    std::pair(SipmControlWriteCommand::TCOEF,                      std::tuple("TCOEF",                    DataFormatTypes<1>{DataFormat::Float}, std::nullopt)),
    std::pair(SipmControlWriteCommand::LUT_ENABLE,                 std::tuple("LUT_ENABLE",               DataFormatTypes<1>{DataFormat::Bool }, std::nullopt)),
    std::pair(SipmControlWriteCommand::ENABLE_PI,                  std::tuple("ENABLE_PI",                DataFormatTypes<1>{DataFormat::Bool }, std::nullopt)),
    std::pair(SipmControlWriteCommand::EMERGENCY_STOP,             std::tuple("EMERGENCY_STOP",           DataFormatTypes<1>{DataFormat::Bool }, std::nullopt)),
    std::pair(SipmControlWriteCommand::IZERO,                      std::tuple("IZERO",                    DataFormatTypes<1>{DataFormat::Bool }, std::nullopt)),
    std::pair(SipmControlWriteCommand::LUT_ADDRESS,                std::tuple("LUT_ADDRESS",              DataFormatTypes<1>{DataFormat::Int  }, std::nullopt)),
    std::pair(SipmControlWriteCommand::LUT_PROGRAM_TEMPERATURE,    std::tuple("LUT_PROGRAM_TEMPERATURE",  DataFormatTypes<1>{DataFormat::Float}, std::nullopt)),
    std::pair(SipmControlWriteCommand::LUT_PROGRAM_OUTPUT_VALUE,   std::tuple("LUT_PROGRAM_OUTPUT_VALUE", DataFormatTypes<1>{DataFormat::Float}, std::nullopt)),
    std::pair(SipmControlWriteCommand::LUT_LENGTH,                 std::tuple("LUT_LENGTH",               DataFormatTypes<1>{DataFormat::Int  }, std::nullopt)),
    std::pair(SipmControlWriteCommand::I2C_BASE_ADDRESS,           std::tuple("I2C_BASE_ADDRESS",         DataFormatTypes<1>{DataFormat::Int  }, std::nullopt)),
    std::pair(SipmControlWriteCommand::CURRENT_RANGE,              std::tuple("CURRENT_RANGE",            DataFormatTypes<1>{DataFormat::Int  }, std::nullopt)),
    std::pair(SipmControlWriteCommand::STORE_ON_FLASH,             std::tuple("STORE_ON_FLASH",           DataFormatTypes<1>{DataFormat::Bool }, std::nullopt))
);

CommandClass(SipmControlWrite);

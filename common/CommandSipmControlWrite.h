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

constexpr static auto SipmControlWriteTable = CommandEnumTable<SipmControlWriteCommandIndexer>::make_table(
    std::tuple(SipmControlWriteCommand::HV_ENABLE,                  "HV_ENABLE",                DataFormatTypes{DataFormat::Bool }),
    std::tuple(SipmControlWriteCommand::MODE,                       "MODE",                     DataFormatTypes{DataFormat::Int  }),
    std::tuple(SipmControlWriteCommand::V_TARGET_WRITE,             "V_TARGET_WRITE",           DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlWriteCommand::RAMP_SPEED,                 "RAMP_SPEED",               DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlWriteCommand::MAX_V,                      "MAX_V",                    DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlWriteCommand::MAX_I,                      "MAX_I",                    DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlWriteCommand::C_TEMP_M2,                  "C_TEMP_M2",                DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlWriteCommand::C_TEMP_M,                   "C_TEMP_M",                 DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlWriteCommand::C_TEMP_Q,                   "C_TEMP_Q",                 DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlWriteCommand::ALFA_VOUT,	                "ALFA_VOUT",	            DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlWriteCommand::ALFA_IOUT,	                "ALFA_IOUT",	            DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlWriteCommand::ALFA_VREF,	                "ALFA_VREF",	            DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlWriteCommand::ALFA_TREF,	                "ALFA_TREF",	            DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlWriteCommand::TCOEF,                      "TCOEF",                    DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlWriteCommand::LUT_ENABLE,                 "LUT_ENABLE",               DataFormatTypes{DataFormat::Bool }),
    std::tuple(SipmControlWriteCommand::ENABLE_PI,                  "ENABLE_PI",                DataFormatTypes{DataFormat::Bool }),
    std::tuple(SipmControlWriteCommand::EMERGENCY_STOP,             "EMERGENCY_STOP",           DataFormatTypes{DataFormat::Bool }),
    std::tuple(SipmControlWriteCommand::IZERO,                      "IZERO",                    DataFormatTypes{DataFormat::Bool }),
    std::tuple(SipmControlWriteCommand::LUT_ADDRESS,                "LUT_ADDRESS",              DataFormatTypes{DataFormat::Int  }),
    std::tuple(SipmControlWriteCommand::LUT_PROGRAM_TEMPERATURE,    "LUT_PROGRAM_TEMPERATURE",  DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlWriteCommand::LUT_PROGRAM_OUTPUT_VALUE,   "LUT_PROGRAM_OUTPUT_VALUE", DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlWriteCommand::LUT_LENGTH,                 "LUT_LENGTH",               DataFormatTypes{DataFormat::Int  }),
    std::tuple(SipmControlWriteCommand::I2C_BASE_ADDRESS,           "I2C_BASE_ADDRESS",         DataFormatTypes{DataFormat::Int  }),
    std::tuple(SipmControlWriteCommand::CURRENT_RANGE,              "CURRENT_RANGE",            DataFormatTypes{DataFormat::Int  }),
    std::tuple(SipmControlWriteCommand::STORE_ON_FLASH,             "STORE_ON_FLASH",           DataFormatTypes{DataFormat::Bool })
);

CommandClass(SipmControlWrite);

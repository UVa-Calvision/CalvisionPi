#pragma once

#include "Command.h"
#include "SipmRegister.h"

constexpr static auto SipmControlTable = CommandEnumTable<SipmControlRegisterIndexer>::make_table(
    std::tuple(SipmControlRegister::HV_ENABLE,                  "HV_ENABLE",                DataFormatTypes{DataFormat::Bool }),
    std::tuple(SipmControlRegister::MODE,                       "MODE",                     DataFormatTypes{DataFormat::Int  }),
    std::tuple(SipmControlRegister::V_TARGET_WRITE,             "V_TARGET_WRITE",           DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::RAMP_SPEED,                 "RAMP_SPEED",               DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::MAX_V,                      "MAX_V",                    DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::MAX_I,                      "MAX_I",                    DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::C_TEMP_M2,                  "C_TEMP_M2",                DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::C_TEMP_M,                   "C_TEMP_M",                 DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::C_TEMP_Q,                   "C_TEMP_Q",                 DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::ALFA_VOUT,	                "ALFA_VOUT",	            DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::ALFA_IOUT,	                "ALFA_IOUT",	            DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::ALFA_VREF,	                "ALFA_VREF",	            DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::ALFA_TREF,	                "ALFA_TREF",	            DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::TCOEF,                      "TCOEF",                    DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::LUT_ENABLE,                 "LUT_ENABLE",               DataFormatTypes{DataFormat::Bool }),
    std::tuple(SipmControlRegister::ENABLE_PI,                  "ENABLE_PI",                DataFormatTypes{DataFormat::Bool }),
    std::tuple(SipmControlRegister::EMERGENCY_STOP,             "EMERGENCY_STOP",           DataFormatTypes{DataFormat::Bool }),
    std::tuple(SipmControlRegister::IZERO,                      "IZERO",                    DataFormatTypes{DataFormat::Bool }),
    std::tuple(SipmControlRegister::LUT_ADDRESS,                "LUT_ADDRESS",              DataFormatTypes{DataFormat::Int  }),
    std::tuple(SipmControlRegister::LUT_PROGRAM_TEMPERATURE,    "LUT_PROGRAM_TEMPERATURE",  DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::LUT_PROGRAM_OUTPUT_VALUE,   "LUT_PROGRAM_OUTPUT_VALUE", DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::LUT_LENGTH,                 "LUT_LENGTH",               DataFormatTypes{DataFormat::Int  }),
    std::tuple(SipmControlRegister::I2C_BASE_ADDRESS,           "I2C_BASE_ADDRESS",         DataFormatTypes{DataFormat::Int  }),
    std::tuple(SipmControlRegister::CURRENT_RANGE,              "CURRENT_RANGE",            DataFormatTypes{DataFormat::Int  }),
    std::tuple(SipmControlRegister::PIN_STATUS,                 "PIN_STATUS",               DataFormatTypes{DataFormat::Int  }),
    std::tuple(SipmControlRegister::VIN,                        "VIN",                      DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::VOUT,	                    "VOUT",	                    DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::IOUT,	                    "IOUT",	                    DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::VREF,	                    "VREF",	                    DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::TREF,	                    "TREF",	                    DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::V_TARGET_READ,              "V_TARGET_READ",            DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::R_TARGET,                   "R_TARGET",                 DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::CVT,                        "CVT",                      DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::COMPLIANCE_V,               "COMPLIANCE_V",             DataFormatTypes{DataFormat::Bool }),
    std::tuple(SipmControlRegister::COMPLIANCE_I,               "COMPLIANCE_I",             DataFormatTypes{DataFormat::Bool }),
    std::tuple(SipmControlRegister::PRODUCT_CODE,               "PRODUCT_CODE",             DataFormatTypes{DataFormat::Int  }),
    std::tuple(SipmControlRegister::FW_VERSION,                 "FW_VERSION",               DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::HW_VERSION,                 "HW_VERSION",               DataFormatTypes{DataFormat::Float}),
    std::tuple(SipmControlRegister::SERIAL_NUMBER,              "SERIAL_NUMBER",            DataFormatTypes{DataFormat::Int  }),
    std::tuple(SipmControlRegister::STORE_ON_FLASH,             "STORE_ON_FLASH",           DataFormatTypes{DataFormat::Bool })
);

class CommandSipmControl : public Command<CommandSipmControl, SipmControlRegister> {
public:
    constexpr static CommandCode code = CommandCode::SipmVoltageControl;

    constexpr static const CommandEnumTable<SipmControlRegisterIndexer>& command_table() {
        return SipmControlTable;
    }

    CommandSipmControl(std::vector<raw_type> raw)
        : Command<CommandSipmControl, SipmControlRegister>(CommandCode::SipmVoltageControl, std::move(raw))
    {}

#ifdef BUILD_SERVER
    virtual ErrorCode execute(Context&, SipmControlRegister) override {
        return ErrorCode::Success;
    }
#endif
};

template<>
struct CommandMapping<CommandCode::SipmVoltageControl> { using type = CommandSipmControl; };


// class CommandSipmVoltageControl : public Command {
// public:
//     ClassFields(SipmVoltageControl)
// 
//     CommandSipmVoltageControl(SipmControlRegister reg, uint32_t raw_value)
//         : reg_(reg), raw_value_(raw_value)
//     {}
// 
//     CommandSipmVoltageControl(const std::string_view& reg_name, const std::string& raw)
//     {
//         if (auto reg = SipmRegisterTable.lookup<SipmRegisterValue::Name>(reg_name)) {
//             reg_ = *reg;
//             raw_value_ = read_to_raw(*SipmRegisterTable.get<SipmRegisterValue::Type>(*reg_), raw);
//         } else {
//             throw std::runtime_error("Invalid sipm control register name: " + std::string(reg_name));
//         }
//     }
// 
//     virtual void write(Socket& socket) override {
//         if (!reg_)
//             throw std::runtime_error("Invalid sipm control register");
// 
//         const uint8_t reg = *SipmRegisterTable.get<SipmRegisterValue::Register>(*reg_);
//         socket.write<uint8_t>(reg);
//         socket.write<uint32_t>(raw_value_);
//     }
// 
//     virtual void read(Socket& socket) override {
//         uint8_t reg = 0;
//         socket.read<uint8_t>(reg);
//         if (auto opt_reg = SipmRegisterTable.lookup<SipmRegisterValue::Register>(reg)) {
//             reg_ = *opt_reg;
//         } else {
//             throw std::runtime_error("Invalid sipm control register: " + std::to_string(reg));
//         }
//         socket.read<uint32_t>(raw_value_);
//     }
// 
//     static size_t size() { return 2; }
// 
// private:
//     std::optional<SipmControlRegister> reg_;
//     uint32_t raw_value_;
// };

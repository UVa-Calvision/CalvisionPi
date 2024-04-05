#include "CommandTemperatureControl.h"

INDEXED_ENUM(RegMap, Register);

constexpr inline auto CommandToRegTable = EnumTable<TemperatureControlCommandIndexer, RegMapIndexer, TMP112_Configuration>::make_table(
    std::pair(TemperatureControlCommand::ShutdownMode,        std::tuple(TMP112_Configuration::ShutdownMode  )),
    std::pair(TemperatureControlCommand::ThermostatMode,      std::tuple(TMP112_Configuration::ThermostatMode)),
    std::pair(TemperatureControlCommand::Polarity,            std::tuple(TMP112_Configuration::Polarity      )),
    std::pair(TemperatureControlCommand::FaultQueue,          std::tuple(TMP112_Configuration::FaultQueue    )),
    std::pair(TemperatureControlCommand::OneShot,             std::tuple(TMP112_Configuration::OneShot       )),
    std::pair(TemperatureControlCommand::ExtendedMode,        std::tuple(TMP112_Configuration::ExtendedMode  )),
    std::pair(TemperatureControlCommand::ConversionRate,      std::tuple(TMP112_Configuration::ConversionRate))
);

template <TemperatureControlCommand reg>
struct ControlRegisterFunctor {
    constexpr static TMP112_Configuration control_reg = CommandToRegTable.get<reg, RegMap::Register>();
    constexpr static DataFormat data_type = TemperatureControlTable.c_get<reg, CommandValues::ParameterTypes>()[0];
    using value_type = typename format_to_type<data_type>::type;

    ReturnData operator()(Context& context, raw_type raw_data) const {
        value_type value;
        try {
            value = format_to_type<data_type>::convert_raw(raw_data);
        } catch(const std::runtime_error& e) {
            std::cerr << "[ERROR]: Error during type conversion " << e.what() << "\n";
            return ReturnData(ErrorCode::InvalidCommand);
        }

        context.temperature_control->write_configuration<control_reg, value_type>(value);

        return ReturnData(ErrorCode::Success);
    }
};

ReturnData CommandTemperatureControl::execute(Context& context, TemperatureControlCommand reg) {
    if (raw_data_.size() != 2)
        return ReturnData(ErrorCode::PoorlyStructuredCommand);

    if (context.temperature_control_unavailable())
        return ReturnData(ErrorCode::ResourceUnavailable);

    return *TemperatureControlCommandIndexer::dispatch<ControlRegisterFunctor>(reg, context, raw_data_[1]);
}



ReturnData CommandTemperatureRead::execute(Context& context, TemperatureReadCommand /* reg */) {
    if (raw_data_.size() != 1)
        return ReturnData(ErrorCode::PoorlyStructuredCommand);

    if (context.temperature_control_unavailable())
        return ReturnData(ErrorCode::ResourceUnavailable);

    return ReturnData(ErrorCode::Success, make_raw(context.temperature_control->read_temperature()));
}

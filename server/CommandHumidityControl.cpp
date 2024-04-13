#include "CommandHumidityControl.h"
#include "HumidityControl.h"


ReturnData CommandHumidityControl::execute(Context& context, HumidityControlCommand command) {
    if (raw_data_.size() != 1)
        return ReturnData(ErrorCode::PoorlyStructuredCommand);

    if (context.humidity_control_unavailable())
        return ReturnData(ErrorCode::ResourceUnavailable);

    switch (command) {
        case HumidityControlCommand::RequestRead:
            return ReturnData(context.humidity_control->request_read() ? ErrorCode::Success : ErrorCode::ResourceBusy);
        case HumidityControlCommand::ReadHumidity:
            return ReturnData(ErrorCode::Success, make_raw<float>(context.humidity_control->humidity()));
        case HumidityControlCommand::ReadTemperature:
            return ReturnData(ErrorCode::Success, make_raw<float>(context.humidity_control->temperature()));
        case HumidityControlCommand::SoftReset:
            context.humidity_control->soft_reset();
            return ReturnData(ErrorCode::Success);
    }

    throw std::runtime_error("Unhandled humidity control command");
}

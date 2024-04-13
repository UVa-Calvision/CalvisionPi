#include "CommandMuxControl.h"

INDEXED_ENUM(RegMap, Register);

constexpr inline auto CommandToRegTable = EnumTable<MuxControlCommandIndexer, RegMapIndexer, TCA9548_Channel>::make_table(
    std::pair(MuxControlCommand::Left,      std::tuple(TCA9548_Channel::Left    )),
    std::pair(MuxControlCommand::Right,     std::tuple(TCA9548_Channel::Right   ))
);

ReturnData CommandMuxControl::execute(Context& context, MuxControlCommand c) {
    if (raw_data_.size() != 1)
        return ReturnData(ErrorCode::PoorlyStructuredCommand);

    if (context.mux_control_unavailable())
        return ReturnData(ErrorCode::ResourceUnavailable);

    context.multiplexer->enable(*CommandToRegTable.get<RegMap::Register>(c));

    return ReturnData(ErrorCode::Success);
}

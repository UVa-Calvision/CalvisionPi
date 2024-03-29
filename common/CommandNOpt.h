#pragma once

#include "Command.h"

class CommandNOpt : public BaseCommand {
public:
    CommandNOpt()
        : BaseCommand(CommandCode::NOpt, {})
    {}

    static std::unique_ptr<CommandNOpt> build_command(detail::token_iter begin, detail::token_iter end) {
        if (begin == end)
            return std::make_unique<CommandNOpt>();
        else
            return nullptr;
    }

    static std::unique_ptr<CommandNOpt> read_raw_from_socket(Socket&) {
        return std::make_unique<CommandNOpt>();
    }

    virtual void dump_command(std::ostream& out) const override {
        out << "NOpt";
    }

    void read_return_value(Socket&) override {}

#ifdef BUILD_SERVER
    virtual ReturnData execute(Context&) override {
        return ReturnData(ErrorCode::Success);
    }
#endif
};

template <>
struct CommandMapping<CommandCode::NOpt> { using type = CommandNOpt; };

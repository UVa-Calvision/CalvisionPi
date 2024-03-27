#pragma once

#include "Command.h"

class CommandQuit : public BaseCommand {
public:

    CommandQuit()
        : BaseCommand(CommandCode::Quit, {})
    {}

    static std::unique_ptr<CommandQuit> build_command(detail::token_iter begin, detail::token_iter end) {
        if (begin == end)
            return std::make_unique<CommandQuit>();
        else
            return nullptr;
    }

    static std::unique_ptr<CommandQuit> read_raw_from_socket(Socket&) {
        return std::make_unique<CommandQuit>();
    }

    virtual void dump_command(std::ostream& out) const override {
        out << "Quit";
    }

#ifdef BUILD_SERVER
    virtual ErrorCode execute(Context&) override {
        return ErrorCode::Success;
    }
#endif
};

template <>
struct CommandMapping<CommandCode::Quit> { using type = CommandQuit; };

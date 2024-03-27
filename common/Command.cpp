#include "Command.h"
#include "CommandNOpt.h"
#include "CommandQuit.h"
#include "CommandVoltageControl.h"
#include "CommandSipmControl.h"



template <CommandCode code>
struct CommandCodeFunctor {
    using CommandType = typename CommandMapping<code>::type;

    std::unique_ptr<BaseCommand> operator()(detail::token_iter begin, detail::token_iter end) {
        try {
            if (begin == end)
                return nullptr;

            const auto e = CommandType::command_table().template lookup<CommandValues::Name>(*begin++);
            if (!e)
                return nullptr;

            const auto& data = CommandType::command_table().template get<CommandValues::Types>(*e);
            std::vector<raw_type> result{static_cast<raw_type>(*CommandType::command_table().to_index(*e))};

            for (DataFormat f : *data) {
                // too few parameters
                if (begin == end)
                    return nullptr;

                if (auto value = read_to_raw(f, *begin++)) {
                    result.push_back(*value);
                } else {
                    // incorrect parameter type
                    return nullptr;
                }
            }

            // too many parameters
            if (begin != end)
                return nullptr;

            return std::make_unique<CommandType>(std::move(result));
        } catch (const std::runtime_error& e) {
            std::cerr << "Error while building command (maybe ok?): " << e.what() << "\n";
            return nullptr;
        }
    }
};

template <>
struct CommandCodeFunctor<CommandCode::NOpt> {
    std::unique_ptr<BaseCommand> operator()(detail::token_iter begin, detail::token_iter end) {
        if (begin == end)
            return std::make_unique<CommandNOpt>();
        else
            return nullptr;
    }
};

template <>
struct CommandCodeFunctor<CommandCode::Quit> {
    std::unique_ptr<BaseCommand> operator()(detail::token_iter begin, detail::token_iter end) {
        if (begin == end)
            return std::make_unique<CommandQuit>();
        else
            return nullptr;
    }
};

std::unique_ptr<BaseCommand> make_command(const std::vector<std::string>& tokens) {
    auto begin = tokens.begin();
    auto end = tokens.end();

    if (begin == end) return nullptr;

    if (auto code = CommandCodeTable.lookup<CommandCodeValues::Name>(std::string_view(*begin++))) {
        // returns optional<unique_ptr<BaseCommand>> so we need to unwrap it
        if (auto command = CommandCodeIndexer::dispatch<CommandCodeFunctor>(*code, begin, end)) {
            return std::move(*command);
        }
    }

    return nullptr;
}

template <CommandCode code>
struct ReadCodeFunctor {
    using CommandType = typename CommandMapping<code>::type;

    std::unique_ptr<BaseCommand> operator()(Socket& socket) {
        try {
            uint32_t raw_enum;
            socket.read<uint32_t>(raw_enum);
            const auto e = CommandType::command_table().from_index(raw_enum);
            if (!e)
                return nullptr;
            std::vector<uint32_t> result(CommandType::command_table().template get<CommandValues::Types>(*e)->size(), 0);
            socket.read_buffer(result);
            result.insert(result.begin(), raw_enum);
            return std::make_unique<CommandType>(std::move(result));
        } catch (const std::runtime_error& err) {
            std::cerr << "Error while reading raw command data from socket: " << err.what() << "\n";
            return nullptr;
        }
    }
};

template <>
struct ReadCodeFunctor<CommandCode::NOpt> {
    std::unique_ptr<BaseCommand> operator()(Socket&) {
        return std::make_unique<CommandNOpt>();
    }
};

template <>
struct ReadCodeFunctor<CommandCode::Quit> {
    std::unique_ptr<BaseCommand> operator()(Socket&) {
        return std::make_unique<CommandQuit>();
    }
};

std::unique_ptr<BaseCommand> read_command(Socket& socket) {
    try {
        raw_type command_code = 0;
        socket.read<raw_type>(command_code);

        if (auto code = CommandCodeTable.from_index(command_code)) {
            if (auto command = CommandCodeIndexer::dispatch<ReadCodeFunctor>(*code, socket)) {
                return std::move(*command);
            }
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "[ERROR] Reading command code: " << e.what() << "\n";
    }

    return nullptr;
}

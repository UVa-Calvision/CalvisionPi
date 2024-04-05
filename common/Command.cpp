#include "Command.h"
#include "CommandNOpt.h"
#include "CommandQuit.h"
#include "CommandVoltageControl.h"
#include "CommandSipmControlWrite.h"
#include "CommandSipmControlRead.h"
#include "CommandTemperatureControl.h"

#include <iomanip>


template <CommandCode code>
struct CommandCodeFunctor {
    using CommandType = typename CommandMapping<code>::type;

    std::unique_ptr<BaseCommand> operator()(detail::token_iter begin, detail::token_iter end) {
        try {
            if (begin == end)
                return nullptr;

            const auto lookup_result = CommandType::command_table().template lookup<CommandValues::Name>(*begin++);
            if (!lookup_result)
                return nullptr;

            const auto& data = lookup_result->second.get().template get<CommandValues::ParameterTypes>();
            std::vector<raw_type> result{static_cast<raw_type>(*CommandType::command_table().to_index(lookup_result->first))};

            for (DataFormat f : data) {
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

    if (begin == end) {
        std::cout << "No input tokens\n";
        return nullptr;
    }

    if (auto lookup_result = CommandCodeTable.lookup<CommandCodeValues::Name>(std::string_view(*begin++))) {
        // returns optional<unique_ptr<BaseCommand>> so we need to unwrap it
        if (auto command = CommandCodeIndexer::dispatch<CommandCodeFunctor>(lookup_result->first, begin, end)) {
            return std::move(*command);
        }
    }
    return nullptr;
}

// ---- ReadCode

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
            std::vector<uint32_t> result(CommandType::command_table().template get<CommandValues::ParameterTypes>(*e)->size(), 0);
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
    raw_type command_code = 0;
    socket.read<raw_type>(command_code);

    if (auto code = CommandCodeTable.from_index(command_code)) {
        if (auto command = CommandCodeIndexer::dispatch<ReadCodeFunctor>(*code, socket)) {
            return std::move(*command);
        }
    }

    return nullptr;
}

// ----- Help

INDEXED_ENUM(DataFormatValue, Name);

constexpr static auto DataFormatTable = EnumTable<DataFormatIndexer, DataFormatValueIndexer, std::string_view>::make_table(
    std::pair(DataFormat::Bool,     std::tuple("bool"   )),
    std::pair(DataFormat::Int,      std::tuple("int"    )),
    std::pair(DataFormat::Uint,     std::tuple("uint"   )),
    std::pair(DataFormat::Float,    std::tuple("float"  ))
);

template <CommandCode code>
struct HelpFunctor {
    using CommandType = typename CommandMapping<code>::type;

    void operator()() {
        constexpr auto ct_entry = CommandCodeTable.get<code>();
        std::cout << ct_entry.template get<CommandCodeValues::Name>() << ": " << ct_entry.template get<CommandCodeValues::Help>() << "\n";
        for (size_t i = 0; i < CommandType::command_table().num_entries(); i++) {
            auto entry = *CommandType::command_table().get(*CommandType::command_table().from_index(i));
            std::cout << ct_entry.template get<CommandCodeValues::Name>() << " " << std::setw(24) << entry.template get<CommandValues::Name>();
            const auto& param_types = entry.template get<CommandValues::ParameterTypes>();
            if (param_types.size() == 0) {
                std::cout << " [ void]";
            } else {
                for (DataFormat f : param_types) {
                    std::cout << " [" << std::setw(5) << *DataFormatTable.get<DataFormatValue::Name>(f) << "]";
                }
            }

            std::cout << " -> [" << std::setw(5);
            auto ret_type = entry.template get<CommandValues::ReturnType>();
            if (ret_type) {
                std::cout << *DataFormatTable.get<DataFormatValue::Name>(*ret_type);
            } else {
                std::cout << "void";
            }
            std::cout << "]\t\t" << entry.template get<CommandValues::Help>() << "\n";
        }
        std::cout << "\n";
    }
};

template <>
struct HelpFunctor<CommandCode::NOpt> {
    void operator()() {
        constexpr auto ct_entry = CommandCodeTable.get<CommandCode::NOpt>();
        std::cout << ct_entry.get<CommandCodeValues::Name>() << ": " << ct_entry.get<CommandCodeValues::Help>() << "\n"
            << "NOpt [ void] -> [ void]\n\n";
    }
};

template <>
struct HelpFunctor<CommandCode::Quit> {
    void operator()() {
        constexpr auto ct_entry = CommandCodeTable.get<CommandCode::Quit>();
        std::cout << ct_entry.get<CommandCodeValues::Name>() << ": " << ct_entry.get<CommandCodeValues::Help>() << "\n"
            << "Quit [ void] -> [ void]\n\n";
    }
};

void print_command_help() {
    std::cout << "==========================================================================================================================\n"
              << "  Help\n"
              << "==========================================================================================================================\n"
              << "Usage:\n"
              << "\tclient address port [flags]\n"
              << "\n"
              << "Commands are parsed line-by-line and delimited by whitespace.\n"
              << "Commands are case-sensitive, and have the general format of:\n"
              << "\tDeviceName Action [Paramaters] -> [Return Type]\n"
              << "void indicates either no parameters should be supplied, or that there is no returned value.\n"
              << "The server always returns an error code. Values are returned only when the error code is a success.\n"
              << "\n"
              << "Command Line Arguments:\n"
              << "    address                     IP address of server to connect to\n"
              << "    port                        Port number of server to connect to\n"
              << "    -h                          Print this help message (address and port may be omitted)\n"
              << "    -c \"Command String\"         Run single provided command.\n"
              << "    -b \"Command String\"         Run single provided command in batch mode. The only output printed to cout is\n"
              << "                                the return value (if present). The exit code of the program is set to the \n"
              << "                                error code returned by the server.\n"
              << "    -f [script file]            File containing line by line commands to execute\n"
              << "If -c, -b, and -f are omitted, then input is read from stdin.\n"
              << "\n"
              << "Error Codes:\n";
 
    for (ErrorCode code : ErrorCodeIndexer::values) {
        std::cout << "\t" << *ErrorCodeTable.to_index(code) << "\t" << *ErrorCodeTable.get<ErrorCodeValue::Name>(code) << "\n";
    }

    std::cout << "\n"
              << "Commands:\n\n";

    for (CommandCode command : CommandCodeIndexer::values) {
        CommandCodeIndexer::dispatch<HelpFunctor>(command);
    }
}

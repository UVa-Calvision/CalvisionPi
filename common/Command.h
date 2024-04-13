#pragma once

#include "CppUtils/networking/Socket.h"
#include "CppUtils/c_util/CUtil.h"
#include <memory>
#include <iostream>
#include <vector>
#include "SipmRegister.h"
#include "DataFormat.h"

#ifdef BUILD_SERVER
#include "Context.h"
#endif

namespace detail {
using token_iter = std::vector<std::string>::const_iterator;
}

INDEXED_ENUM(CommandCode, 
    NOpt,
    Quit,
    VoltageControl,
    SipmControlWrite,
    SipmControlRead,
    TemperatureControl,
    TemperatureRead,
    HumidityControl,
    MuxControl,
    SipmDacControl,
    SipmDacWrite
);

INDEXED_ENUM(CommandCodeValues,
    Name,
    Help
);

constexpr inline auto CommandCodeTable = EnumTable<CommandCodeIndexer, CommandCodeValuesIndexer, std::string_view, std::string_view>::make_table(
    std::pair(CommandCode::NOpt,               std::tuple("NOpt",               "Do nothing (just for debugging)"   )),
    std::pair(CommandCode::Quit,               std::tuple("Quit",               "Tell the server to quit"           )),
    std::pair(CommandCode::VoltageControl,     std::tuple("VoltageControl",     "LED Voltage Control"               )),
    std::pair(CommandCode::SipmControlWrite,   std::tuple("SipmControlWrite",   "CAEN SiPM Voltage write commands"  )),
    std::pair(CommandCode::SipmControlRead,    std::tuple("SipmControlRead",    "CAEN SiPM Voltage read commands"   )),
    std::pair(CommandCode::TemperatureControl, std::tuple("TemperatureControl", "Temperature readout configuration" )),
    std::pair(CommandCode::TemperatureRead,    std::tuple("TemperatureRead",    "Temperature readout"               )),
    std::pair(CommandCode::HumidityControl,    std::tuple("HumidityControl",    "Humidity control"                  )),
    std::pair(CommandCode::MuxControl,         std::tuple("MuxControl",         "Multiplexer control"               )),
    std::pair(CommandCode::SipmDacControl,     std::tuple("SipmDacControl",     "SIPM DAC configuration"            )),
    std::pair(CommandCode::SipmDacWrite,       std::tuple("SipmDacWrite",       "SiPM DAC write commands"           ))
);



INDEXED_ENUM(ErrorCode,
    Success,
    InvalidCommand,
    PoorlyStructuredCommand,
    ResourceUnavailable,
    VoltageOutOfRange,
    UnspecifiedFailure,
    CannotWrite,
    CannotRead,
    ResourceBusy
);

INDEXED_ENUM(ErrorCodeValue,
    Name
);

constexpr inline auto ErrorCodeTable = EnumTable<ErrorCodeIndexer, ErrorCodeValueIndexer, std::string_view>::make_table(
    std::pair(ErrorCode::Success,                  std::tuple("Success"                   )),
    std::pair(ErrorCode::InvalidCommand,           std::tuple("Invalid command"           )),
    std::pair(ErrorCode::PoorlyStructuredCommand,  std::tuple("Poorly structured command" )),
    std::pair(ErrorCode::ResourceUnavailable,      std::tuple("Resource unavailable"      )),
    std::pair(ErrorCode::VoltageOutOfRange,        std::tuple("Voltage out of range"      )),
    std::pair(ErrorCode::UnspecifiedFailure,       std::tuple("Unspecified failure"       )),
    std::pair(ErrorCode::CannotWrite,              std::tuple("Cannot write to register"  )),
    std::pair(ErrorCode::CannotRead,               std::tuple("Cannot read from register" )),
    std::pair(ErrorCode::ResourceBusy,             std::tuple("Resource is busy"          ))
);

struct ReturnData {

    explicit ReturnData(ErrorCode code)
        : error_code(code), return_value(std::nullopt)
    {}

    explicit ReturnData(ErrorCode code, raw_type value)
        : error_code(code), return_value(value)
    {}

    ErrorCode error_code;
    std::optional<raw_type> return_value;
};


class BaseCommand {
public:
    using raw_type = uint32_t;

    void write(Socket& socket) {
        socket.write<raw_type>(raw_enum_);
        socket.write_buffer(raw_data_);
    }

    void read(Socket& socket) {
        socket.read<raw_type>(raw_enum_);
        std::cout << "Raw Enum: " << raw_enum_ << "\n";
        socket.read_buffer(raw_data_);
    }

    virtual void dump_command(std::ostream& out) const = 0;

    CommandCode code() const {
        return *CommandCodeTable.from_index(raw_enum_);
    }

    virtual void read_return_value(Socket& socket) = 0;

#ifdef BUILD_SERVER
    virtual ReturnData execute(Context&) = 0;
#endif

protected:
    BaseCommand(CommandCode code, std::vector<raw_type> raw_data)
        : raw_enum_(static_cast<raw_type>(*CommandCodeIndexer::get(code))), raw_data_(std::move(raw_data))
    {}

    raw_type raw_enum_;
    std::vector<raw_type> raw_data_;
};

INDEXED_ENUM(CommandValues,
    Name,
    ParameterTypes,
    ReturnType,
    Help
);

template <typename Indexer, size_t N>
using CommandEnumTable = EnumTable<Indexer, CommandValuesIndexer,
                                   std::string_view, DataFormatTypes<N>, std::optional<DataFormat>, std::string_view>;

template <typename T, typename EnumType>
class Command : public BaseCommand {
    using Self = Command<T,EnumType>;
public:
    void dump_command(std::ostream& out) const override {
        auto name = command_value<CommandValues::Name>();
        if (!name) return;

        out << "[" << CommandCodeTable.get<T::code, CommandCodeValues::Name>() << "]: "
            << " " << *name;

        auto types = *command_value<CommandValues::ParameterTypes>();

        for (size_t i = 0; i < types.size(); i++) {
            out << " ";
            dump_dataformat(out, types[i], raw_data_[i+1]);
        }
        out << "\n";
    }

    virtual void read_return_value(Socket& socket) override {
        if (auto ret_type = command_value<CommandValues::ReturnType>()) {
            if (*ret_type) {
                raw_type raw_value;
                socket.read<raw_type>(raw_value);
                dump_dataformat(std::cout, **ret_type, raw_value);
                // this->handle_return_value(raw_value);
            } 
        } 
    }

#ifdef BUILD_SERVER
    virtual ReturnData execute(Context& context) override {
        auto action = lead_enum();
        if (!action) {
            return ReturnData(ErrorCode::InvalidCommand);
        }
        
        dump_command(std::cout);

        try {
            return execute(context, *action);
        } catch(const std::runtime_error& e) {
            std::cerr << "[ERROR] Execute: " << e.what() << "\n";
            return ReturnData(ErrorCode::ResourceUnavailable);
        }
    }

    virtual ReturnData execute(Context&, EnumType) = 0;
#endif

protected:
    Command(CommandCode code, std::vector<raw_type> raw)
        : BaseCommand(code, std::move(raw))
    {}

//    virtual void handle_return_value(raw_type raw_value) {
//        // Must have return type if this is called!
//        DataFormat f = **command_value<CommandValues::ReturnType>();
//        dump_dataformat(std::cout, f, raw_value);
//    }

    template <CommandValues c>
    constexpr auto command_value() const {
        using CommandTableType = std::remove_reference_t<std::invoke_result_t<decltype(&T::command_table)> >;
        using F = typename CommandTableType::template FieldType<c>;
        auto action = lead_enum();
        if (!action) return (const F*) nullptr;
        return T::command_table().template get<c>(*action);
    }

    template <EnumType e>
    constexpr static auto c_data_types() {
        return T::command_table().template c_get<e, CommandValues::ParameterTypes>();
    }

    std::optional<EnumType> lead_enum() const {
        if (raw_data_.empty()) return std::nullopt;
        return impl().command_table().from_index(raw_data_[0]);
    }

    template <EnumType e>
    auto unpack_raw() const {
        return unpack_raw_helper<e>(std::make_index_sequence<c_data_types<e>().size()>{});
    }

    template <EnumType e, size_t... Is>
    auto unpack_raw_helper(std::index_sequence<Is...>) const {
        return std::make_tuple(format_to_type<c_data_types<e>()[Is]>::convert_raw(raw_data_.at(1+Is))...);
    }

private:
    T& impl() {
        return *static_cast<T*>(this);
    }

    const T& impl() const {
        return *static_cast<const T*>(this);
    }
};

#ifdef BUILD_SERVER
#define EXECUTE_SIGNATURE(NAME) virtual ReturnData execute(Context&, NAME##Command) override;
#else
#define EXECUTE_SIGNATURE(NAME)
#endif

#define CommandClass(NAME) \
    class Command##NAME : public Command<Command##NAME, NAME##Command> { \
    public: \
        constexpr static CommandCode code = CommandCode::NAME; \
        constexpr static const auto& command_table() { return NAME##Table; } \
        Command##NAME(std::vector<raw_type> raw) : Command<Command##NAME, NAME##Command>(code, std::move(raw)) {} \
        EXECUTE_SIGNATURE(NAME) \
    }; \
    template <> struct CommandMapping<Command##NAME::code> { using type = Command##NAME; };


template <CommandCode code>
struct CommandMapping;

std::unique_ptr<BaseCommand> make_command(const std::vector<std::string>& tokens);
std::unique_ptr<BaseCommand> read_command(Socket& socket);
void print_command_help();

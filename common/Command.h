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
    SipmControlRead
);

INDEXED_ENUM(CommandCodeValues, Name);

constexpr static auto CommandCodeTable = EnumTable<CommandCodeIndexer, CommandCodeValuesIndexer, std::string_view>::make_table(
    std::tuple(CommandCode::NOpt, "NOpt"),
    std::tuple(CommandCode::Quit, "Quit"),
    std::tuple(CommandCode::VoltageControl, "VoltageControl"),
    std::tuple(CommandCode::SipmControlWrite, "SipmControlWrite"),
    std::tuple(CommandCode::SipmControlRead, "SipmControlRead")
);



enum class ErrorCode : uint16_t {
    Success = 0,
    InvalidCommand,
    PoorlyStructuredCommand,
    ResourceUnavailable,
    VoltageOutOfRange,
    UnspecifiedFailure,
    CannotWrite,
    CannotRead,
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
        socket.read_buffer(raw_data_);
    }

    virtual void dump_command(std::ostream& out) const = 0;

    CommandCode code() const {
        return *CommandCodeTable.from_index(raw_enum_);
    }

#ifdef BUILD_SERVER
    virtual ErrorCode execute(Context&) = 0;
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
    Types
);

template <typename Indexer>
using CommandEnumTable = EnumTable<Indexer, CommandValuesIndexer, std::string_view, DataFormatTypes>;

template <typename T, typename EnumType>
class Command : public BaseCommand {
public:
    void dump_command(std::ostream& out) const override {
        auto action = lead_enum();
        if (!action) return;

        out << "[" << CommandCodeTable.get<T::code, CommandCodeValues::Name>() << "]: "
            << " " << *T::command_table().template get<CommandValues::Name>(*action);

        const DataFormatTypes& types = data_types(*action);

        for (size_t i = 0; i < types.size(); i++) {
            out << " ";
            dump_dataformat(out, types[i], raw_data_[i+1]);
        }
        out << "\n";
    }

#ifdef BUILD_SERVER
    virtual ErrorCode execute(Context& context) override {
        auto action = lead_enum();
        if (!action) {
            return ErrorCode::InvalidCommand;
        }
        
        dump_command(std::cout);
        std::cout << "\n";

        try {
            return execute(context, *action);
        } catch(const std::runtime_error& e) {
            std::cerr << "[ERROR] Execute: " << e.what() << "\n";
            return ErrorCode::ResourceUnavailable;
        }
    }

    virtual ErrorCode execute(Context&, EnumType) = 0;
#endif

protected:
    Command(CommandCode code, std::vector<raw_type> raw)
        : BaseCommand(code, std::move(raw))
    {}

    template <EnumType e>
    constexpr static DataFormatTypes c_data_types() {
        return T::command_table().template c_get<e, CommandValues::Types>();
    }

    constexpr const DataFormatTypes& data_types(EnumType e) const {
        return *T::command_table().template get<CommandValues::Types>(e);
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
#define CommandClass(NAME) \
    class Command##NAME : public Command<Command##NAME, NAME##Command> { \
    public: \
        constexpr static CommandCode code = CommandCode::NAME; \
        constexpr static const CommandEnumTable<NAME##CommandIndexer>& command_table() { return NAME##Table; } \
        Command##NAME(std::vector<raw_type> raw) : Command<Command##NAME, NAME##Command>(code, std::move(raw)) {} \
        virtual ErrorCode execute(Context&, NAME##Command) override; \
    }; \
    template <> struct CommandMapping<Command##NAME::code> { using type = Command##NAME; };
#else
#define CommandClass(NAME) \
    class Command##NAME : public Command<Command##NAME, NAME##Command> { \
    public: \
        constexpr static CommandCode code = CommandCode::NAME; \
        constexpr static const CommandEnumTable<NAME##CommandIndexer>& command_table() { return NAME##Table; } \
        Command##NAME(std::vector<raw_type> raw) : Command<Command##NAME, NAME##Command>(code, std::move(raw)) {} \
    }; \
    template <> struct CommandMapping<Command##NAME::code> { using type = Command##NAME; };
#endif

template <CommandCode code>
struct CommandMapping;

std::unique_ptr<BaseCommand> make_command(const std::vector<std::string>& tokens);
std::unique_ptr<BaseCommand> read_command(Socket& socket);

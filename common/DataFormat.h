#pragma once

#include "CppUtils/c_util/CUtil.h"
#include "CppUtils/c_util/Enum.h"
#include <cstdint>
#include <string>
#include <optional>
#include <iostream>
#include <array>

using raw_type = uint32_t;

INDEXED_ENUM(DataFormat,
    Bool,
    Int,
    Uint,
    Float
);

using DataFormatTypes = std::array<DataFormat, 1>;

template <typename T> constexpr DataFormat type_to_format() = delete;
template <> constexpr DataFormat type_to_format<bool>() { return DataFormat::Bool; }
template <> constexpr DataFormat type_to_format<int32_t>() { return DataFormat::Int; }
template <> constexpr DataFormat type_to_format<uint32_t>() { return DataFormat::Uint; }
template <> constexpr DataFormat type_to_format<float>() { return DataFormat::Float; }

template <DataFormat f> struct format_to_type;

template <>
struct format_to_type<DataFormat::Bool> {
    using type = bool;
    static type read(const std::string& s) { return static_cast<bool>(std::stoi(s)); }
    static type convert_raw(raw_type x) { return static_cast<bool>(x); }

    constexpr static uint8_t SipmCode = 0x02;
};

template <>
struct format_to_type<DataFormat::Int> {
    using type = int32_t;
    static type read(const std::string& s) { return std::stoi(s); }
    static type convert_raw(raw_type x) { return binary_cast<type>(x); }

    constexpr static uint8_t SipmCode = 0x00;
};

template <>
struct format_to_type<DataFormat::Uint> {
    using type = uint32_t;
    static type read(const std::string& s) { return std::stoul(s); }
    static type convert_raw(raw_type x) { return binary_cast<type>(x); }

    constexpr static uint8_t SipmCode = 0x02;
};

template <>
struct format_to_type<DataFormat::Float> {
    using type = float;
    static type read(const std::string& s) { return std::stof(s); }
    static type convert_raw(raw_type x) { return binary_cast<type>(x); }

    constexpr static uint8_t SipmCode = 0x03;
};

template <DataFormat f> using format_to_type_t = typename format_to_type<f>::type;

std::optional<raw_type> read_to_raw(DataFormat f, const std::string& s);

void dump_dataformat(std::ostream& out, DataFormat f, raw_type x);

template <typename T>
raw_type make_raw(T t) {
    static_assert(sizeof(T) == 4);
    return binary_cast<raw_type>(t);
}

template <> raw_type make_raw<bool>(bool t);


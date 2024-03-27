#pragma once

#include <array>
#include <span>
#include <cstdint>
#include <type_traits>

#include "SipmRegister.h"

template <typename ControlImpl>
class SipmControl {
public:
    virtual ~SipmControl() {}

    template <SipmControlRegister reg, typename T>
    void write_register(T value) {
        static_assert(SipmRegisterTable.get<SipmRegisterValue::Can_Write>().get<reg>());
        static_assert(valid_register_type<T, reg>());

        impl().template i2c_write<T>(SipmRegisterTable.get<SipmRegisterValue::Register>().get<reg>(), value);
    }

    template <SipmControlRegister reg, typename T>
    T read_register() {
        static_assert(SipmRegisterTable.get<SipmRegisterValue::Can_Read>().get<reg>());
        static_assert(valid_register_type<T, reg>());

        return impl().template i2c_read<T>(SipmRegisterTable.get<SipmRegisterValue::Register>().get<reg>());
    }

    virtual bool good() const = 0;

protected:
    SipmControl()
    {}

private:
    ControlImpl& impl() {
        return *static_cast<ControlImpl*>(this);
    }

    const ControlImpl& impl() const {
        return *static_cast<const ControlImpl*>(this);
    }
};


#include "CppUtils/io/I2cHandle.h"


class SipmI2cControl : public SipmControl<SipmI2cControl>, private I2cReaderWriter {
public:
    SipmI2cControl();
    virtual ~SipmI2cControl();

    virtual bool good() const { return I2cReaderWriter::good(); }

protected:
    friend class SipmControl<SipmI2cControl>;

    template <typename T>
    using format = format_to_type<type_to_format<T>()>;

    template <size_t N>
    void print_command(const std::array<uint8_t, N>& message) {
        std::cout << "I2C Command: " << std::hex;
        for (uint8_t i : message) {
            std::cout << "0x" << static_cast<unsigned int>(i) << "  ";
        }
        std::cout << std::dec << "\n";
    }

    template <typename T>
    void i2c_write(uint8_t address, T value) {
        static std::array<uint8_t, 6> write_message;
        write_message[0] = address;
        write_message[1] = static_cast<uint8_t>(format<T>::SipmCode);
        raw_type raw_value = make_raw<T>(value);
        for (size_t i = 0; i < sizeof(raw_type); i++) {
            write_message[2+i] = ((uint8_t*) &raw_value)[i];
        }
        write_buffer(write_message);
    }

    template <typename T>
    T i2c_read(uint8_t address) {
        static std::array<uint8_t, 2> write_message;
        write_message[0] = address;
        write_message[1] = static_cast<uint8_t>(format<T>::SipmCode);
        write_buffer(write_message);

        static std::array<uint8_t, 4> read_message;
        read_buffer(read_message);
        raw_type raw_value = 0;
        for (size_t i = 0; i < sizeof(raw_type); i++) {
            ((uint8_t*) &raw_value)[i] = read_message[i];
        }
        return format<T>::convert_raw(raw_value);
    }

private:

    constexpr static uint8_t I2C_BUS_ID = 1;
    constexpr static uint8_t I2C_DEV_ID = 0x70;
};

// UART control

#include <termios.h>
#include <sstream>
#include <vector>

#include "CppUtils/io/DeviceHandle.h"

namespace {

#include <type_traits>

template <typename... Args, std::enable_if_t<sizeof...(Args) == 0, bool> = true>
void concat_commands(std::ostream&, const Args&...) {
}

template <typename Arg, typename... Args>
void concat_commands(std::ostream& out, const Arg& a, const Args&... args) {
    out << ',' << a;
    concat_commands<Args...>(out, args...);
}

};

class SipmUartControl : public SipmControl<SipmUartControl>, private DeviceReaderWriter {
public:
    SipmUartControl(const std::string& tty_name);
    virtual ~SipmUartControl();

    virtual bool good() const { return DeviceReaderWriter::good(); }

protected:
    friend class SipmControl<SipmUartControl>;

    template <typename T>
    void i2c_write(uint8_t address, T value) {
        send_command("SET", static_cast<unsigned int>(address), value);
        std::cout << "[RESPONSE] " << read_response() << "\n";
    }

    template <typename T>
    T i2c_read(uint8_t address) {
        send_command("GET", static_cast<unsigned int>(address));
        std::string response = read_response();
        std::cout << "[RESPONSE] " << response << "\n";
        if (response.length() < 3) std::runtime_error("Invalid Uart prefix: " + response);
        return format_to_type<type_to_format<T>()>::read(response.substr(3));
    }

private:
    template <typename... Args>
    void send_command(const std::string& command_name, const Args&... args) {
        std::stringstream s;
        s << "AT+" << command_name;
        concat_commands(s, args...);
        s << CRLF;
        write_string(s.str());
    }

    std::string read_response();

    constexpr static const char* CRLF = "\n";

    struct termios old_termios_;
};

#pragma once

#include <array>
#include <span>
#include <cstdint>
#include <type_traits>
#include <memory>

#include "SipmRegister.h"
#include "Input.h"

class SipmControl {
public:
    virtual ~SipmControl() {}

    template <SipmControlRegister reg, typename T>
    void write_register(T value) {
        static_assert(SipmRegisterTable.get<reg, SipmRegisterValue::Can_Write>());
        static_assert(valid_register_type<T, reg>());

        this->write_impl(SipmRegisterTable.get<reg, SipmRegisterValue::Register>(), type_to_format<T>(), make_raw<T>(value));
    }

    template <SipmControlRegister reg, typename T>
    T read_register() {
        static_assert(SipmRegisterTable.get<reg, SipmRegisterValue::Can_Read>());
        static_assert(valid_register_type<T, reg>());

        return format_to_type<type_to_format<T>()>::convert_raw(
            this->read_impl(SipmRegisterTable.get<reg, SipmRegisterValue::Register>(), type_to_format<T>()));
    }

    virtual bool good() const = 0;

    static std::unique_ptr<SipmControl> make_control(const SipmCaenInput& input);

protected:
    SipmControl()
    {}

    virtual void write_impl(uint8_t address, DataFormat format, raw_type raw_value) = 0;
    virtual raw_type read_impl(uint8_t address, DataFormat format) = 0;
};


#include "CppUtils/io/I2cHandle.h"


class SipmI2cControl : public SipmControl, private I2cReaderWriter {
public:
    SipmI2cControl(const I2cInput& input);
    virtual ~SipmI2cControl();

    virtual bool good() const { return I2cReaderWriter::good(); }

protected:
    template <size_t N>
    void print_command(const std::array<uint8_t, N>& message) {
        std::cout << "I2C Command: " << std::hex;
        for (uint8_t i : message) {
            std::cout << "0x" << static_cast<unsigned int>(i) << "  ";
        }
        std::cout << std::dec << "\n";
    }

    uint8_t format_code(DataFormat f) {
        switch (f) {
            case DataFormat::Bool: return 0x02;
            case DataFormat::Int: return 0x00;
            case DataFormat::Uint: return 0x02;
            case DataFormat::Float: return 0x03;
        }
        return 0x00;
    }

    void write_impl(uint8_t address, DataFormat f, raw_type raw_value) override {
        static std::array<uint8_t, 6> write_message;
        write_message[0] = address;
        write_message[1] = format_code(f);
        for (size_t i = 0; i < sizeof(raw_type); i++) {
            write_message[2+i] = ((uint8_t*) &raw_value)[i];
        }
        write_buffer(write_message);
    }

    raw_type read_impl(uint8_t address, DataFormat f) override {
        static std::array<uint8_t, 2> write_message;
        write_message[0] = address;
        write_message[1] = format_code(f);
        write_buffer(write_message);

        static std::array<uint8_t, 4> read_message;
        read_buffer(read_message);
        raw_type raw_value = 0;
        for (size_t i = 0; i < sizeof(raw_type); i++) {
            ((uint8_t*) &raw_value)[i] = read_message[i];
        }
        return raw_value;
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

class SipmUartControl : public SipmControl, private DeviceReaderWriter {
public:
    SipmUartControl(const std::string& input);
    virtual ~SipmUartControl();

    virtual bool good() const { return DeviceReaderWriter::good(); }

protected:
    void write_impl(uint8_t address, DataFormat f, raw_type value) {
        send_command("SET", address, f, value);
    }

    raw_type read_impl(uint8_t address, DataFormat f) {
        send_command("GET", address, f);
        std::string response = read_response();
        if (response.length() < 3) std::runtime_error("Invalid Uart prefix: " + response);
        return *read_to_raw(f, response.substr(3));
    }

private:
    void send_command(const std::string& command_name, uint8_t address,
                      DataFormat f, std::optional<raw_type> value = std::nullopt) {
        std::stringstream s;
        s << "AT+" << command_name << "," << static_cast<unsigned>(address);
        if (value) {
            s << ",";
            dump_dataformat(s, f, *value);
        }
        s << CRLF;
        write_string(s.str());
    }

    std::string read_response();

    constexpr static const char* CRLF = "\n";

    struct termios old_termios_;
};

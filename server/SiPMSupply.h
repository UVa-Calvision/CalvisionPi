#pragma once

#include <array>
#include <span>
#include <cstdint>
#include <type_traits>

#include "SipmRegister.h"

template <typename ControlImpl>
class SipmControl {
public:
    SipmControl(ControlImpl* impl)
        : impl_(impl)
    {}

    virtual ~SipmControl() {}

    template <SipmControlRegister reg, typename T>
    void write_register(T value) {
        static_assert(SipmRegisterTable.get<SipmRegisterValue::Can_Write>().get<reg>());
        static_assert(valid_register_type<T, reg>());

        impl_->template i2c_write<T>(SipmRegisterTable.get<SipmRegisterValue::Register>().get<reg>(), value);
    }

    template <SipmControlRegister reg, typename T>
    T read_register() {
        static_assert(SipmRegisterTable.get<SipmRegisterValue::Can_Read>().get<reg>());
        static_assert(valid_register_type<T, reg>());

        return impl_->template i2c_read<T>(SipmRegisterTable.get<SipmRegisterValue::Register>().get<reg>());
    }

private:

    ControlImpl* impl_;
};


#include "CppUtils/io/I2cHandle.h"


class SipmI2cControl : public SipmControl<SipmI2cControl>, private I2cReaderWriter {
public:
    SipmI2cControl();
    virtual ~SipmI2cControl();

    template <typename T>
    void i2c_write(uint8_t address, T value) {
        static std::array<uint8_t, 7> write_message;
        write_message[0] = I2C_DEV_ID;
        write_message[1] = address;
        write_message[2] = static_cast<uint8_t>(type_format<T>());
        for (size_t i = 0; i < sizeof(T); i++) {
            write_message[3+i] = ((uint8_t*) &value)[i];
        }
        write_buffer(write_message);
    }

    template <typename T, typename std::enable_if_t<std::is_same_v<T,bool>, bool> = true>
    void i2c_write(uint8_t address, T value) {
        i2c_write<uint32_t>(address, static_cast<uint32_t>(value));
    }

    template <typename T>
    T i2c_read(uint8_t address) {
        static std::array<uint8_t, 3> write_message;
        write_message[0] = I2C_DEV_ID;
        write_message[1] = address;
        write_message[2] = static_cast<uint8_t>(type_format<T>());
        write_buffer(write_message);

        static std::array<uint8_t, 4> read_message;
        read_buffer(read_message);
        return *((T*) read_message.data());
    }

    template <typename T, typename std::enable_if_t<std::is_same_v<T,bool>, bool> = true>
    T i2c_read(uint8_t address) {
        return i2c_read<uint32_t>(address) != 0;
    }

private:

    constexpr static uint8_t I2C_BUS_ID = 1;
    constexpr static uint8_t I2C_DEV_ID = 0x70;

    enum class DataFormat : uint8_t {
        Integer = 0x00,
        FixedPoint = 0x01,
        UnsignedInteger = 0x02,
        FloatingPoint = 0x03,
    };

    template <typename T>
    constexpr static DataFormat type_format() {
        if (std::is_same_v<T, int32_t>) return DataFormat::Integer;
        if (std::is_same_v<T, uint32_t>) return DataFormat::UnsignedInteger;
        if (std::is_same_v<T, float>) return DataFormat::FloatingPoint;
        static_assert(false);
    }
};

// UART control

#include <termios.h>

#include "CppUtils/io/DeviceHandle.h"

class SipmUartControl : private DeviceReaderWriter {
public:
    SipmUartControl(const std::string& tty_name);
    virtual ~SipmUartControl();
private:
    struct termios old_termios_;
};

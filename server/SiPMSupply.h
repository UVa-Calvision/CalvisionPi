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

protected:
    friend class SipmControl<SipmI2cControl>;

    template <typename T>
    using format = format_to_type<type_to_format<T>()>;

    template <typename T>
    void i2c_write(uint8_t address, T value) {
        static std::array<uint8_t, 7> write_message;
        write_message[0] = I2C_DEV_ID;
        write_message[1] = address;
        write_message[2] = static_cast<uint8_t>(format<T>::SipmCode);
        raw_type raw_value = make_raw<T>(value);
        for (size_t i = 0; i < sizeof(raw_type); i++) {
            write_message[3+i] = ((uint8_t*) &raw_value)[i];
        }
        write_buffer(write_message);
    }

    template <typename T>
    T i2c_read(uint8_t address) {
        static std::array<uint8_t, 3> write_message;
        write_message[0] = I2C_DEV_ID;
        write_message[1] = address;
        write_message[2] = static_cast<uint8_t>(format<T>::SipmCode);
        write_buffer(write_message);

        static std::array<uint8_t, 4> read_message;
        read_buffer(read_message);
        return format<T>::convert_raw(*((raw_type*) read_message.data()));
    }

private:

    constexpr static uint8_t I2C_BUS_ID = 1;
    constexpr static uint8_t I2C_DEV_ID = 0x70;
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

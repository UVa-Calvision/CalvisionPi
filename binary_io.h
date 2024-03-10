#pragma once

#include <cstdint>
#include <string>

class BinaryWriter {
public:
    template <typename T>
    void write(const T& t) {
        write<T, 1>(&t);
    }

    void write_string(const std::string& s) {
        write<char>((char*) s.data(), (size_t) s.length());
    }

    template <typename T>
    void write(const T* buffer, size_t N) {
        this->write_impl((const uint8_t*) buffer, N * sizeof(T));
    }

    template <typename T, size_t N>
    void write(const T* buffer) {
        write(buffer, N);
    }

    template <typename T>
    void write_buffer(const T& buffer) {
        write(buffer.data(), buffer.size());
    }

protected:
    virtual void write_impl(const uint8_t* buffer, size_t N) = 0;
};

class BinaryReader {
public:
    template <typename T>
    void read(T& t) {
        read<T, 1>(&t);
    }

    template <typename T, size_t N>
    void read(T* buffer) {
        read<T>(buffer, N);
    }

    template <typename T>
    void read(T* buffer, size_t N) {
        this->read_impl((uint8_t*) buffer, sizeof(T) * N);
    }

    template <typename T>
    void read_buffer(T& buffer) {
        read(buffer.data(), buffer.size());
    }

    template <typename T>
    std::string read_string(T& buffer) {
        read_buffer(buffer);
        return std::string(buffer.data(), buffer.size());
    }

protected:
    virtual void read_impl(uint8_t* buffer, size_t N) = 0;
};

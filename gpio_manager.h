#pragma once

#include <string>
#include <gpiod.h>
#include <stdexcept>

class GpioChip {
public:
    GpioChip(const std::string filename)
        : chip_(gpiod_chip_open(filename.c_str()))
    {
        if (!chip_)
            throw std::runtime_error("Couldn't open chip " + filename);
    }

    ~GpioChip() {
        gpiod_chip_close(chip_);
    }

    void set_line(const std::string& name, unsigned int offset, int value) {
        struct gpiod_line* line = request_line(name, offset, GPIOD_LINE_REQUEST_DIRECTION_OUTPUT);

        if (gpiod_line_set_value(line, value) < 0)
            throw std::runtime_error("Failed to set " + name + " line to " + std::to_string(value));

        gpiod_line_release(line);
    }

    int get_line(const std::string& name, unsigned int offset) {
        struct gpiod_line* line = request_line(name, offset, GPIOD_LINE_REQUEST_DIRECTION_INPUT);

        int value = gpiod_line_get_value(line);
        if (value < 0)
            throw std::runtime_error("Failed to get " + name + " line");

        gpiod_line_release(line);

        return value;
    }

private:
    struct gpiod_line* request_line(const std::string& name, unsigned int offset, int direction) {
        struct gpiod_line* line = gpiod_chip_get_line(chip_, offset);
        if (!line)
            throw std::runtime_error("Couldn't get " + name + " line");

        struct gpiod_line_request_config config;
        config.consumer = name.c_str();
        config.request_type = direction;
        config.flags = 0;

        if (gpiod_line_request(line, &config, 0) < 0)
            throw std::runtime_error("Failed to request " + name);

        return line;
    }

    struct gpiod_chip* chip_;
};

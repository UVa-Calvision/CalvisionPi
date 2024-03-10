#pragma once

#include <string>
#include <gpiod.h>
#include <stdexcept>

class GpioChip {
public:
    GpioChip(const std::string filename);
    ~GpioChip();

    void set_line(const std::string& name, unsigned int offset, int value);
    int get_line(const std::string& name, unsigned int offset);

private:
    struct gpiod_line* request_line(const std::string& name, unsigned int offset, int direction);

    struct gpiod_chip* chip_;
};

#pragma once

#include <string>
#include <gpiod.h>
#include <stdexcept>

class GpioChip {
public:
    GpioChip(const std::string filename);
    ~GpioChip();

    void enable_hv() {
        set_line("hv_enable", hv_enable_offset, 1);
    }

    void disable_hv() {
        set_line("hv_enable", hv_enable_offset, 0);
    }

private:
    const static int hv_enable_offset = 27;

    void set_line(const std::string& name, unsigned int offset, int value);
    int get_line(const std::string& name, unsigned int offset);

    struct gpiod_line* request_line(const std::string& name, unsigned int offset, int direction);

    struct gpiod_chip* chip_;
};

#include "GpioManager.h"

GpioChip::GpioChip(const GpioInput& input)
    : chip_(gpiod_chip_open(input.file.c_str()))
{
    if (!chip_)
        throw std::runtime_error("Couldn't open chip " + input.file);
}

GpioChip::~GpioChip() {
    gpiod_chip_close(chip_);
}

void GpioChip::set_line(const std::string& name, unsigned int offset, int value) {
    struct gpiod_line* line = request_line(name, offset, GPIOD_LINE_REQUEST_DIRECTION_OUTPUT);

    if (gpiod_line_set_value(line, value) < 0)
        throw std::runtime_error("Failed to set " + name + " line to " + std::to_string(value));

    gpiod_line_release(line);
}

int GpioChip::get_line(const std::string& name, unsigned int offset) {
    struct gpiod_line* line = request_line(name, offset, GPIOD_LINE_REQUEST_DIRECTION_INPUT);

    int value = gpiod_line_get_value(line);
    if (value < 0)
        throw std::runtime_error("Failed to get " + name + " line");

    gpiod_line_release(line);

    return value;
}

struct gpiod_line* GpioChip::request_line(const std::string& name, unsigned int offset, int direction) {
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

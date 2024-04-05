#include "Context.h"

Context::Context(const ContextInput& input)
{
    try {
        chip = std::make_unique<GpioChip>(input.gpio_input);
    } catch(const std::runtime_error& e) {
        std::cerr << "[ERROR] Failed to open gpio chip: " << e.what() << "\n";
        chip = nullptr;
    }

    try {
        lv_control = std::make_unique<mcp4725>(input.lv_input);
    } catch(const std::runtime_error& e) {
        std::cerr << "[ERROR] Failed to open low voltage control: " << e.what() << "\n";
        lv_control = nullptr;
    }

    try {
        hv_control = std::make_unique<mcp4725>(input.hv_input);
    } catch(const std::runtime_error& e) {
        std::cerr << "[ERROR] Failed to open high voltage control: " << e.what() << "\n";
        hv_control = nullptr;
    }

    try {
        sipm_control = SipmControl::make_control(input.sipm_input);
    } catch(const std::runtime_error& e) {
        std::cerr << "[ERROR] Failed to open SiPM control: " << e.what() << "\n";
        hv_control = nullptr;
    }

    try {
        temperature_control = std::make_unique<tmp112>(input.temperature_input);
    } catch(const std::runtime_error& e) {
        std::cerr << "[ERROR] Failed to open temperature control: " << e.what() << "\n";
        temperature_control = nullptr;
    }
}



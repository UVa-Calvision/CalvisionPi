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
        multiplexer = std::make_unique<tca9548>(input.multiplexer_input);
        multiplexer->enable(TCA9548_Channel::Left);

        try {
            humidity_control = std::make_unique<aht10>(input.humidity_input);
        } catch (const std::runtime_error& e) {
            std::cerr << "[ERROR] Failed to open humidity sensor: " << e.what() << "\n";
            humidity_control = nullptr;
        }

        try {
            temperature_control = std::make_unique<tmp112>(input.temperature_input);
        } catch (const std::runtime_error& e) {
            std::cerr << "[ERROR] Failed to open temperature sensor: " << e.what() << "\n";
            temperature_control = nullptr;
        }

        try {
            sipm_dac_control = std::make_unique<ltc2635>(input.sipm_dac_input);
            std::cout << "Sipm dac control: " << sipm_dac_control.get() << "\n";
            std::cout << "Sipm dac control good: " << (sipm_dac_control.get() ? sipm_dac_control->good() : false) << "\n";
        } catch (const std::runtime_error& e) {
            std::cerr << "[ERROR] Failed to open SiPM DAC controls: " << e.what() << "\n";
            sipm_dac_control = nullptr;
        }
    } catch(const std::runtime_error& e) {
        std::cerr << "[ERROR] Failed to open multiplexer: " << e.what() << "\n";
        multiplexer = nullptr;
    }
}



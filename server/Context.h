#pragma once

#include "GpioManager.h"
#include "VoltageControl.h"
#include "SiPMSupply.h"
#include "TemperatureControl.h"

#include <iostream>
#include <string>


struct Context {
    Context(const ContextInput& input);

    std::unique_ptr<GpioChip> chip;
    std::unique_ptr<mcp4725> lv_control;
    std::unique_ptr<mcp4725> hv_control;
    std::unique_ptr<SipmControl> sipm_control;
    std::unique_ptr<tmp112> temperature_control;

    bool quit = false;

    bool chip_unavailable() const { return !(chip && chip->good()); }
    bool lv_control_unavailable() const { return !(lv_control && lv_control->good()); }
    bool hv_control_unavailable() const { return !(hv_control && hv_control->good()); }
    bool sipm_control_unavailable() const { return !(sipm_control && sipm_control->good()); }
    bool temperature_control_unavailable() const { return !(temperature_control && temperature_control->good()); }
};



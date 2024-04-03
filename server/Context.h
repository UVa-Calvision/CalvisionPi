#pragma once

#include "GpioManager.h"
#include "VoltageControl.h"
#include "SiPMSupply.h"
#include "TemperatureControl.h"

#include <iostream>
#include <string>

#define USE_SIPM_UART


struct Context {
    
    Context()
        : chip("/dev/gpiochip0")
        , lv_control()
        , hv_control()
#ifdef USE_SIPM_UART
        , sipm_control("USB0")
#else
        , sipm_control()
#endif
        , temperature_control()
    {
        if (!lv_control.good()) {
            std::cerr << "[ERROR] LOW VOLTAGE CONTROL FAILED TO OPEN...\n";
        }

        if (!hv_control.good()) {
            std::cerr << "[ERROR] HIGH VOLTAGE CONTROL FAILED TO OPEN...\n";
        }

        if (!sipm_control.good()) {
            std::cerr << "[ERROR] SIPM CONTROL FAILED TO OPEN...\n";
        }


    }

    GpioChip chip;
    LowVoltageControl lv_control;
    HighVoltageControl hv_control;
#ifdef USE_SIPM_UART
    SipmUartControl sipm_control;
#else
    SipmI2cControl sipm_control;
#endif
    tmp112 temperature_control;

    bool quit = false;
};



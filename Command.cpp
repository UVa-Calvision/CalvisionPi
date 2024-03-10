#include "Command.h"

#ifdef BUILD_SERVER

int16_t CommandEnableHighVoltage::execute(Context& context) {
    std::cout << "Enabling high voltage.\n";
    return 0;
}

int16_t CommandDisableHighVoltate::execute(Context& context) {
    std::cout << "Disabling high voltage.\n";
    return 0;
}

int16_t CommandSetHighVoltage::execute(Context& context) {
    std::cout << "Setting high voltage to " << voltage_ << " mV\n";
    return 0;
}

int16_t CommandSetLowVoltage::execute(Context& context) {
    std::cout << "Setting low voltage to " << voltage_ << " mV\n";
    return 0;
}

#endif

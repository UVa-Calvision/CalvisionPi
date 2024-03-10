#include "Command.h"

int main(void) {

    Context context;

    ServerListener server(&context);
    server.listen();

//     // Enable GPIO
//     const unsigned int hv_enable_offset = 27;   // GPIO27 on the board
//     const int value = 0;
// 
//     GpioChip chip("/dev/gpiochip0");
//     chip.set_line("hv_enable", hv_enable_offset, value);

    
//     // Setup HV
//     try {
//         mcp4725 voltage_control(I2C_BUS_ID, HIGH_VOLTAGE_DEV_ID);
// 
//         uint16_t dac_min = 0x1c2;
//         float voltage_min = 59.03e3;
//         voltage_control.set_voltage_min(voltage_min);
//         voltage_control.set_dac_min(dac_min);
// 
//         uint16_t dac_max = 0x7d0;
//         float voltage_max = 4.459e3;
//         voltage_control.set_voltage_max(voltage_max);
//         voltage_control.set_dac_max(dac_max);
// 
//         // Calibrate max / min voltages
//         // std::cout << "Setting dac min value (" << std::hex << dac_min << std::dec << ")...\n";
//         // voltage_control.set_int(dac_min);
//         // std::cout << "Enter voltage (mV): ";
//         // voltage_min = 0;
//         // std::cin >> voltage_min;
//         // voltage_control.set_voltage_min(voltage_min);
// 
//         // std::cout << "Setting dac max value (" << std::hex << dac_max << std::dec << ")...\n";
//         // voltage_control.set_int(dac_max);
//         // std::cout << "Enter voltage (mV): ";
//         // voltage_max = 0;
//         // std::cin >> voltage_max;
//         // voltage_control.set_voltage_max(voltage_max);
// 
//         // Check DAC -> Voltage conversion
//         std::string temp;
//         for (int i = 0; i <= 100; i += 25) {
//             uint16_t value = (uint16_t) linear_interpolate(i, 0, 100, dac_min, dac_max);
//             std::cout << "Setting " << i << "\% of max voltage (" << std::hex << value << std::dec << "): "
//                 << voltage_control.dac_to_voltage(value) << " mV\n";
//             voltage_control.set_int(value);
//             std::cin >> temp;
//         }
// 
//         // Check voltage -> DAC conversion
//         for (int i = 0; i <= 100; i += 25) {
//             float voltage = linear_interpolate(i, 0, 100, voltage_max, voltage_min);
//             std::cout << "Setting " << i << "\% of max voltage: " << voltage << " mV\n";
//             voltage_control.set_int(voltage_control.voltage_to_dac(voltage));
//             std::cin >> temp;
// 
//         }
// 
//     } catch (const std::exception& e) {
//         std::cerr << "Exception: " << e.what() << "\n";
//     }
// 
//     std::cout << "Done.\n";

    return 0;
}

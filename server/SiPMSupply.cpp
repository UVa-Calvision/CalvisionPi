#include "SiPMSupply.h"

SipmI2cControl::SipmI2cControl()
    : SipmControl(),
    I2cReaderWriter(I2C_BUS_ID, I2C_DEV_ID, OpenMode::ReadWrite)
{}

SipmI2cControl::~SipmI2cControl()
{}

// UART control

#include <sys/types.h>
#include <fcntl.h>

#include "CppUtils/c_util/CUtil.h"

SipmUartControl::SipmUartControl(const std::string& tty_name)
    : DeviceReaderWriter("dev/tty" + tty_name, O_RDWR | O_NOCTTY)
{
    if (!good())
        throw std::runtime_error("Failed to open " + tty_name);

    tcgetattr(fd_, &old_termios_);


    struct termios new_termios;
    initialize_zero(new_termios);
    cfmakeraw(&new_termios);

    new_termios.c_cflag
        = CRTSCTS   // Output hardware flow control
        // set by raw | CS8       // 8 bit, no parity, 1 stopbit
        | CLOCAL    // local connection, no modem control
        | CREAD;    // enable receiving characters
    
    const speed_t baudrate = B115200;   // bps rate
    cfsetspeed(&new_termios, baudrate);

    new_termios.c_iflag
        = IGNPAR;   // ignore bytes with parity errors
        // unset by raw? | ICRNL;    // map CR to NL
        
    // Raw output
    // new_termios.c_oflag = 0;

    // Disable echo functionality
    // should we use canon? new_termios.c_lflag = ICANON;   // enable canonical output
    
    new_termios.c_cc[VTIME] = 0;    // inter-character timer unused
    new_termios.c_cc[VMIN] = 5;     // block reading until >= 5 chars received

    tcflush(fd_, TCIFLUSH);
    tcsetattr(fd_, TCSANOW, &new_termios);
}

SipmUartControl::~SipmUartControl() {
    tcsetattr(fd_, TCSANOW, &old_termios_);
}


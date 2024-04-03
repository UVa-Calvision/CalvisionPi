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
#include "CppUtils/io/IOUtils.h"

#include <unistd.h>

SipmUartControl::SipmUartControl(const std::string& tty_name)
    : DeviceReaderWriter("/dev/ttyUSB0" /* + tty_name */, O_RDWR | O_NOCTTY)
{
    if (!good())
        throw std::runtime_error("Failed to open /dev/tty" + tty_name);

    tcgetattr(fd_, &old_termios_);


    struct termios new_termios;
    initialize_zero(new_termios);
    // cfmakeraw(&new_termios);

    new_termios.c_cflag
        = CS8       // 8 bit, no parity, 1 stopbit
        | CLOCAL    // local connection, no modem control
        | CREAD;    // enable receiving characters
 
    new_termios.c_cflag &= ~CRTSCTS; // Disable hardware control of messages

    const speed_t baudrate = B115200;   // bps rate
    cfsetspeed(&new_termios, baudrate);

    new_termios.c_iflag
        = IGNPAR;    // ignore bytes with parity errors
        // | ICRNL;    // map CR to NL
        
    new_termios.c_oflag
        = OLCUC;     // Map lowercase to upper case
        // | ONLCR;    // Map NL to CR-NL

    // Disable echo functionality
    new_termios.c_lflag = ICANON;   // enable canonical output
    
    // Initialize control characters
    new_termios.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
    new_termios.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
    new_termios.c_cc[VERASE]   = 0;     /* del */
    new_termios.c_cc[VKILL]    = 0;     /* @ */
    new_termios.c_cc[VEOF]     = 4;     /* Ctrl-d */
    new_termios.c_cc[VTIME]    = 0;     /* inter-character timer unused */
    new_termios.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
    new_termios.c_cc[VSWTC]    = 0;     /* '\0' */
    new_termios.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
    new_termios.c_cc[VSTOP]    = 0;     /* Ctrl-s */
    new_termios.c_cc[VSUSP]    = 0;     /* Ctrl-z */
    new_termios.c_cc[VEOL]     = 0;     /* '\0' */
    new_termios.c_cc[VREPRINT] = 0;     /* Ctrl-r */
    new_termios.c_cc[VDISCARD] = 0;     /* Ctrl-u */
    new_termios.c_cc[VWERASE]  = 0;     /* Ctrl-w */
    new_termios.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
    new_termios.c_cc[VEOL2]    = 0;     /* '\0' */

    tcflush(fd_, TCIFLUSH);
    tcsetattr(fd_, TCSANOW, &new_termios);


    // Send code saying we're using UART in machine mode
    // send_string(CRLF);
    // send_string(CRLF);
    // usleep(100'000);
    send_command("MACHINE");
}

SipmUartControl::~SipmUartControl() {
    if (good()) {
        tcsetattr(fd_, TCSANOW, &old_termios_);
    }
}

std::string SipmUartControl::read_response() {
    static std::array<char, 256> buffer;

    detail::match_suffix<char, 2> match_crlf("\r\n");
    size_t total = detail::staggered_read(
                        [this] (char* xs, size_t n) {
                            return ::read(fd_, xs, n);
                        },
                        buffer.data(), buffer.size(), match_crlf);

    buffer[total] = 0;

    if (total < 4) throw std::runtime_error("Invalid Uart response: " + std::string(buffer.data()));
    
    // check first 3 chars match OK=
    constexpr static const char* response_prefix = "OK";
    for (size_t i = 0; i < 2; i++) {
        if (buffer[i] != response_prefix[i]) {
            throw std::runtime_error("Invalid response prefix: " + std::string(buffer.data()));
        }
    }

    // Strip trailing CRLF
    return std::string(buffer.begin(), buffer.begin()+total-2);
}


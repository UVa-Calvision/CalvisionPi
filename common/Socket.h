#pragma once

#include <string>
#include <netinet/in.h>

#include "binary_io.h"

class Socket : public BinaryWriter, public BinaryReader {
public:
    Socket();

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& s) = default;
    Socket& operator=(Socket&& s) = default;

    ~Socket();

    bool good() const;

    void open(int port);
    void connect(const std::string& hostname, int port);
    void accept(const Socket& server);

    void close();

protected:
    virtual void write_impl(const uint8_t* buffer, size_t N) override;
    virtual void read_impl(uint8_t* buffer, size_t N) override;

private:
    int socket_fd_;
    struct sockaddr_in address_;
};

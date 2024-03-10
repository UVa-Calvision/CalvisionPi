#pragma once

#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <cstring>
#include <array>
#include <unistd.h>

#include "binary_io.h"

class Socket : public BinaryWriter, public BinaryReader {
public:


    Socket()
        : socket_fd_(-1)
    {
        std::memset((void*) &address_, 0, sizeof(address_));
    }

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&& s) = default;
    Socket& operator=(Socket&& s) = default;

    ~Socket() {
        close();
    }

    void open(int port) {
        socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (!good())
            throw std::runtime_error("Error opening server socket");

        address_.sin_family = AF_INET;
        address_.sin_port = htons(port);
        address_.sin_addr.s_addr = INADDR_ANY;

        if (bind(socket_fd_, (struct sockaddr*) &address_, sizeof(address_)) < 0)
            throw std::runtime_error("Error on binding");

        listen(socket_fd_, 5);
    }

    bool good() const {
        return socket_fd_ > 0;
    }

    void accept(const Socket& server) {
        socklen_t client_len = sizeof(address_);
        socket_fd_ = ::accept(server.socket_fd_, (struct sockaddr*) &server.address_, &client_len);
        if (!good())
            throw std::runtime_error("Error opening client socket");
    }

//     template <size_t N>
//     void recieve(std::array<uint8_t, N>& buffer) const {
//         std::memset((void*) buffer.data(), 0, N * sizeof(uint8_t));
//         size_t n = ::recv(socket_fd_, buffer.data(), N, 0);
//         if (n < 0)
//             throw std::runtime_error("Error reading from socket");
//     }
// 
//     template <size_t N>
//     void send(const std::array<uint8_t, N>& buffer) const {
//         send(buffer.data(), N);
//     }
// 
//     void send(const uint8_t* buffer, size_t length) const {
//         size_t n = ::send(socket_fd_, buffer, length, 0);
//         if (n < 0)
//             throw std::runtime_error("Error sending from socket");
//     }

    void close() {
        if (good()) {
            ::close(socket_fd_);
            socket_fd_ = -1;
        }
    }
protected:
    virtual void write_impl(const uint8_t* buffer, size_t N) override {
        size_t n = ::send(socket_fd_, buffer, N, 0);
        if (n < 0)
            throw std::runtime_error("Error sending from socket");
    }

    virtual void read_impl(uint8_t* buffer, size_t N) override {
        std::memset((void*) buffer, 0, N * sizeof(uint8_t));
        size_t n = ::recv(socket_fd_, buffer, N, 0);
        if (n < 0)
            throw std::runtime_error("Error reading from socket");
    }

private:
    int socket_fd_;
    struct sockaddr_in address_;
};

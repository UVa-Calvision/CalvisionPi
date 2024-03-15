#include "Socket.h"

#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <array>
#include <unistd.h>

#include <iostream>


Socket::Socket()
    : socket_fd_(-1)
{
    std::memset((void*) &address_, 0, sizeof(address_));
}

Socket::~Socket() {
    close();
}

void Socket::open(int port) {
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

void Socket::connect(const std::string& hostname, int port) {
    socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (!good())
        throw std::runtime_error("Error opening client socket");
    
    struct hostent* hostentry = gethostbyname(hostname.c_str());
    if (!hostentry)
        throw std::runtime_error("Error finding hostname " + hostname);

    address_.sin_family = AF_INET;
    address_.sin_port = htons(port);
    std::memcpy((void*) &address_.sin_addr.s_addr, (void*) hostentry->h_addr, hostentry->h_length);

    if (::connect(socket_fd_, (struct sockaddr*) &address_, sizeof(address_)) < 0)
        throw std::runtime_error("Error on connecting");
}

bool Socket::good() const {
    return socket_fd_ > 0;
}

void Socket::accept(const Socket& server) {
    socklen_t client_len = sizeof(address_);
    socket_fd_ = ::accept(server.socket_fd_, (struct sockaddr*) &server.address_, &client_len);
    if (!good())
        throw std::runtime_error("Error opening client socket");
}

void Socket::close() {
    if (good()) {
        ::close(socket_fd_);
        socket_fd_ = -1;
    }
}

void Socket::write_impl(const uint8_t* buffer, size_t N) {
    size_t total = 0;
    while (total < N) {
        size_t n = ::send(socket_fd_, &buffer[total], N - total, 0);
        if (n == 0)
            throw std::runtime_error("Disconnect while writing socket");

        total += n;
    }
}

void Socket::read_impl(uint8_t* buffer, size_t N) {
    std::memset((void*) buffer, 0, N * sizeof(uint8_t));

    size_t total = 0;
    while (total < N) {
        size_t n = ::recv(socket_fd_, &buffer[total], N - total, 0);
        if (n == 0)
            throw std::runtime_error("Disconnect while reading socket");

        total += n;
    }
}

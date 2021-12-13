#pragma once

#include <exception>
#include <string>
#include <SFML/Network.hpp>

class NetworkException : std::exception {
private:
    std::string message;
public:
    NetworkException(std::string string);

    NetworkException(const char *c_string);

    ~NetworkException() noexcept override;

    const char *what() const noexcept override;

};

class ClientReceiveException : NetworkException {
private:
    sf::Socket::Status status;
public:
    ClientReceiveException(std::string message, sf::Socket::Status status);

    ~ClientReceiveException() noexcept override;

    sf::Socket::Status getStatus() const;
};
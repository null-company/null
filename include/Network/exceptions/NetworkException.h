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

class ReceiveException : public NetworkException {
private:
    sf::Socket::Status status;
public:
    ReceiveException(std::string message, sf::Socket::Status status);

    ~ReceiveException() noexcept override;

    sf::Socket::Status getStatus() const;
};

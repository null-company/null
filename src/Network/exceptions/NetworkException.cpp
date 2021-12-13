//
// Created by artemonchelus on 13.12.2021.
//

#include "exceptions/NetworkException.h"

#include <utility>

const char *NetworkException::what() const noexcept {
    return message.c_str();
}

NetworkException::NetworkException(std::string string) : message(std::move(string)) {}

NetworkException::NetworkException(const char *c_str) : NetworkException(std::string(c_str)) {}

NetworkException::~NetworkException() noexcept = default;

ClientReceiveException::ClientReceiveException(std::string message, sf::Socket::Status status) : NetworkException(
        std::move(message)), status(status) {}

sf::Socket::Status ClientReceiveException::getStatus() const {
    return status;
}

ClientReceiveException::~ClientReceiveException() noexcept = default;
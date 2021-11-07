#include "../../include/Network/Client.h"
#include "SFML/Network.hpp"
#include "iostream"

Client::Client(sf::IpAddress server_address, uint16_t state_port, uint16_t command_port)
        : server_state_socket(),
          client_command_socket(),
          client_idx(-1),
          server_address(server_address),
          state_port(state_port),
          command_port(command_port) {
    std::cout << "Client with server address " << server_address << ", state_port: " << state_port << ", "
              << command_port;

}


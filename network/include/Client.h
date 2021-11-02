//
// Created by artemonchelus on 02.11.2021.
//

#ifndef NULL_GAME_CLIENT_H
#define NULL_GAME_CLIENT_H

#include "SFML/Network.hpp"

class Client {
private:
    char client_idx;
    sf::TcpSocket server_state_socket;
    sf::TcpSocket client_command_socket;
    sf::IpAddress server_address;
    uint16_t state_port;
    uint16_t command_port;
public:
    Client(sf::IpAddress server_address, uint16_t state_port, uint16_t command_port);

    void connect();

    void send_message();

};

#endif //NULL_GAME_CLIENT_H

/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** client
*/

#include "../include/client.hpp"

Client::Client(sf::IpAddress address, unsigned short connexion_port, Mediator& med):
recipient(address), port(connexion_port) , _medi(med), _logic(_medi){

    has_start = true;
    game_started = false;
    lobby_started = false;
    has_start_error = false;
    in_replay = false;
    record = false;
    loadKeyBindingsFromFile();
    std::cout << "[Client]: New client created on " << address << " " << port << std::endl;
    replays_path = "replays/list.txt";
    _manager.Init();
}

Client::~Client()
{
    tcp_socket.disconnect();
}

void Client::connect_socket() {
    tcp_sckt_status = tcp_socket.connect(recipient, lobby_port);
}

void Client::send_tcp_message(std::vector<char> & message)
{
    if (tcp_socket.send(message.data(), sizeof(message)) != sf::Socket::Done) {
        std::cout << "(Tcp) message not sended" << std::endl;
    }
    //std::cout << "(Tcp) message sended" << std::endl;
}

void Client::send_udp_message(std::vector<char> & message) {
    if (udp_socket.send(message.data(), sizeof(message), recipient, port) != sf::Socket::Done) {
        std::cout << "(Udp) message not sended" << std::endl;
    }
    //std::cout << "(Udp) message sended" << std::endl;
}

void Client::receive_udp_message()
{
}

void Client::tcp_authenticate(std::string msg)
{
    std::vector<char> auth_msg = BinaryProtocol::GetAuthMessage(msg);
    send_tcp_message(auth_msg);
}

void Client::handle_score(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Score* msg_data = reinterpret_cast<BinaryProtocol::Score*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    _score.setString("SCORE: " + std::to_string(msg_data->score));
    r_score = 1;
}

void Client::send_close_message()
{
    std::string d_msg = "CLIENT_CLOSE";
    std::vector<char> close_msg = BinaryProtocol::GetClienClosetMessage(_id);

    send_tcp_message(close_msg);
}


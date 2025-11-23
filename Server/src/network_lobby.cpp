/*
** EPITECH PROJECT, 2024
** test_net
** File description:
** network_lobby
*/

#include "../include/network.hpp"

LobbyTCP::LobbyTCP(io_context& io_context, std::string &ip ,short port, const std::string& game_server_ip, unsigned short game_server_port)
: _acceptor(io_context, tcp::endpoint(boost::asio::ip::make_address(ip), port)),
          game_server_ip(game_server_ip), game_server_port(game_server_port)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    std::cout << "Looby created in " << ip  << "  on port " << port << std::endl;
    parse_file();

    game_id = Generate_UUID();

    _data.resize(5000);
}

LobbyTCP::~LobbyTCP()
{
    close();
    std::cout << "Lobby is close\n";
}

void LobbyTCP::start_lobby()
{
    start_accept();
}

void LobbyTCP::close()
{
    _is_running = false;
    _acceptor.close();
}

void LobbyTCP::parse_file(void)
{
    std::ifstream json_file("Config/server_config.json");

    if (!json_file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        exit(1);
    }
    
    std::string json_str((std::istreambuf_iterator<char>(json_file)), std::istreambuf_iterator<char>());
    json json_data;

    try {
        json_data = json::parse(json_str);
        json_file.close();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    //const auto& levels = json_data["levels"];
    auto levels = json_data["levels"].as<std::vector<std::string>>();

    // Afficher les niveaux
    std::cout << "Niveaux disponibles :" << std::endl;
    for (const auto& level : levels) {
        Level temp;
        temp.setLevelName(level);
        game_levels.push_back(temp);
        std::cout << "- " << level << std::endl;
    }
    // for (const auto& level : levels.array_range()) {
    //     Level temp;
    //     temp.setLevelName(level.);

    //     // const auto& level_data = level.value();
    //     // for (const auto& field : level_data.object_range()) {

    //     //     if (field.key() == "player_health") {
    //     //         temp.setPlayerHealth(field.value().as<int>());
    //     //     } else if (field.key() == "player_speed") {
    //     //         temp.setPlayerSpeed(field.value().as<float>());
    //     //     } else if (field.key() == "player_damage") {
    //     //         temp.setPlayerDamage(field.value().as<float>());
    //     //     } else if (field.key() == "mobs_health") {
    //     //         temp.setMobsHealth(field.value().as<int>());
    //     //     } else if (field.key() == "mobs_damage") {
    //     //         temp.setMobsDamage(field.value().as<float>());
    //     //     } else if (field.key() == "mobs_speed") {
    //     //         temp.setMobsSpeed(field.value().as<float>());
    //     //     } else if (field.key() == "boss_health") {
    //     //         temp.setBossHealth(field.value().as<int>());
    //     //     } else if (field.key() == "boss_damage") {
    //     //         temp.setBossDamage(field.value().as<float>());
    //     //     } else if (field.key() == "boss_speed") {
    //     //         temp.setBossSpeed(field.value().as<float>());
    //     //     } else if (field.key() == "type") {
    //     //         temp.setType(field.value().as<std::string>());
    //     //     } else if (field.key() == "score_goal") {
    //     //         temp.setLevelScore(field.value().as<int>());
    //     //     }
    //     //}
    //     game_levels.push_back(temp);
    // }
    //std::cout << "Loading Level configuration...\n";

    for (const auto& level : game_levels) {
        std::cout << "Level Name: " << level.getLevelName() << std::endl;
        // std::cout << "Player Health: " << level.getPlayerHealth() << std::endl;
        // std::cout << "Player Speed: " << level.getPlayerSpeed() << std::endl;
        // std::cout << "Player Damage: " << level.getPlayerDamage() << std::endl;
        // std::cout << "Mobs Health: " << level.getMobsHealth() << std::endl;
        // std::cout << "Mobs Damage: " << level.getMobsDamage() << std::endl;
        // std::cout << "Mobs Speed: " << level.getMobsSpeed() << std::endl;
        // std::cout << "Boss Health: " << level.getBossHealth() << std::endl;
        // std::cout << "Boss Damage: " << level.getBossDamage() << std::endl;
        // std::cout << "Boss Speed: " << level.getBossSpeed() << std::endl;
        // std::cout << "---------------------------" << std::endl; // Pour séparer les niveaux
    }
}

void LobbyTCP::start_accept()
{
    // create a new client when connection
    std::shared_ptr<tcp::socket> new_client = std::make_shared<tcp::socket>(_acceptor.get_executor());

    _acceptor.async_accept(*new_client, [this, new_client](const std::error_code& error) {
        if (!error) {
            //std::cout << "New client connected on " << new_client->remote_endpoint().address().to_string() << " "  << "on port " << new_client->remote_endpoint().port() << "\n";

            std::string client_address = new_client->remote_endpoint().address().to_string();
            unsigned short client_port = static_cast<unsigned short>(new_client->remote_endpoint().port());

            ClientInfo new_client_info(client_address, client_port, count);

            std::pair<int, int> pos = generateUniquePosition(existingPositions, 0, 960, 0, 540);

            int temp_x = pos.first;
            int temp_y = pos.second;

            // generate position for the new player
            new_client_info.set_PosX(pos.first);
            new_client_info.set_PosY(pos.second);

            //std::cout << "Client nb " << count << " is Connected with PosX == " <<  new_client_info.get_PosX() << " and PosY == " << new_client_info.get_PosY() << "\n";
            _clients.push_back(std::move(new_client_info));

            // pass to Anothher client

            int temp_id = count;

            if (!_client_sockets.empty()) {
                // send to all client the message of a new connected client
                std::string msg_client = "New client";
                std::vector<char> reponse_message = BinaryProtocol::GetNewClientMessage(msg_client, count, temp_x, temp_y);
                for (auto& client : _client_sockets) {
                    send_message(client, reponse_message);
                }
            }
            count++;

            /// send to the actual_client all the client that were there

            if (!_client_sockets.empty()) {
                std::vector<int> ids;
                std::vector<std::pair<int, int>> randoms_positions;
    
                for (const auto& client_info : _clients) {
                    // asign to the vector the id of all the client wihout the new conneced
                    if (client_info.getID() !=  temp_id) {
                        ids.push_back(client_info.getID());
                        //std::cout << "Client ID: " << client_info.getID() << std::endl;

                        std::pair<int, int> temp_pos;
                        temp_pos.first = client_info.get_PosX();
                        temp_pos.second = client_info.get_PosY();
                        
                        //std::cout << "Client ID:" << client_info.getID() << " is with PosX == " <<  temp_pos.first << " and PosY == " << temp_pos.second << "\n";
                        randoms_positions.push_back(temp_pos);
                    }
                }

                // create a message containing all the clients to the new client
                std::vector<char> client_list_message = BinaryProtocol::GetNewClientListMessage(ids.data(), ids.size(), randoms_positions);
                send_message(new_client, client_list_message);
            }

            // stock the client  in the vector of client
            _client_sockets.push_back(new_client);
            start_read(new_client);
        }
        // wait for another connection
        start_accept();
    });
}

void LobbyTCP::start_read(std::shared_ptr<tcp::socket> client_socket)
{
    memset(_data.data(), 0, sizeof(_data));

    client_socket->async_read_some(boost::asio::buffer(_data, sizeof(_data)),
        [this, client_socket](std::error_code ec, std::size_t length) {
            if (!ec) {
                handle_message(client_socket, length);
                start_read(client_socket);
            }
    });
}

const std::vector<ClientInfo>& LobbyTCP::get_clients() const
{
    return _clients;
}

void LobbyTCP::handle_message(std::shared_ptr<tcp::socket> client_socket, std::size_t length)
{

    std::cout << "Lobby length " << length << "\n";

    if (length < sizeof(BinaryProtocol::MessageHeader)) {
        std::cerr << "Invalid message type\n";
        return;
    }

    BinaryProtocol::MessageHeader* header = reinterpret_cast<BinaryProtocol::MessageHeader*>(_data.data());

    switch (header->type) {
        case BinaryProtocol::MessageType::AUTHENTICATION: {
            //std::cout << "Enter Auth\n";
            handle_authentification(client_socket, length);
            break;
        }
        case BinaryProtocol::MessageType::START: {
            //std::cout << "-------emplay start\n";
            handle_start(client_socket, length);
            break;
        }
        case BinaryProtocol::MessageType::CLIENT_QUIT: {
            std::cout << "client closed\n";
            remove_client(client_socket);
            break;
        } case BinaryProtocol::MessageType::CHOOSE_LEVEL: {
            // send to all the client that a level was choose
            //handle_level_choice(header, client_socket);
            break;
        } case BinaryProtocol::MessageType::START_LEVEL: {
            // prepare the init for a level
            //handle_level_start(header, client_socket);
            break;
        }
        default:
            break;
    }
}

void LobbyTCP::redirect_to_game_server(std::shared_ptr<tcp::socket> client_socket)
{

    // redirect all client to the server

    for (const auto& cl : _clients) {
        
    }
    // std::string redirect_message = "Redirecting to game server at " + game_server_ip_ + ":" + std::to_string(game_server_port_);
    //     asio::write(*client_socket, asio::buffer(redirect_message));
    //     client_socket->close();
    //     std::cout << "Redirecting client to game server at " << game_server_ip_ << ":" << game_server_port_ << std::endl;
}

bool LobbyTCP::is_game_server_ready() const
{
    return game_server_ready;
}

void LobbyTCP::send_start_success()
{
    std::string status = "SUCCESS";
    std::vector<char> success_msg = BinaryProtocol::GetStartSuccessMessage(status);

    // handle the send of the message
}

void LobbyTCP::send_start_failure()
{
    std::string status = "FAILURE";
    std::vector<char> success_msg = BinaryProtocol::GetStartFailureMessage(status);
}

// void LobbyTCP::handle_authentification(std::shared_ptr<tcp::socket> client_socket, uint32_t length)
// {
//     // if (length < sizeof(BinaryProtocol::AuthMessage)) {
//     //     std::cerr << "Authentication data is too small." << std::endl;
//     //     return;
//     // }
//     // handle authentification

//     // Get the actual data in memory that's beside the header
//     BinaryProtocol::AuthMessage* auth_data = reinterpret_cast<BinaryProtocol::AuthMessage*>(_data.data() + sizeof(BinaryProtocol::MessageHeader));

//     std::cout << "Name == " << auth_data->username << std::endl;
//     std::string status = "AUTHENTICATION_SUCCESS";

//     // when a client finish to authenticate, send him a message containing his id
//     for (int i = 0; i < _clients.size(); i++) {

//         // std::cout << "------------------------------------\n";

//         // std::cout << _clients[i].getIp();
//         // std::cout << _clients[i].getPort();
//         // std::cout << "------------------------------------\n";

//         // std::cout << "-------------SECONDPART-------------------\n";
//         // std::cout << client_socket->remote_endpoint().address().to_string();
//         // std::cout << client_socket->remote_endpoint().port();
//         // std::cout << "---------------FINISH-------------\n";

//         if (client_socket->remote_endpoint().address().to_string() == _clients[i].getIp()
//             && client_socket->remote_endpoint().port() ==  _clients[i].getPort()) {
//                 std::vector<char> response = BinaryProtocol::GetAuthSuccessMessage(status, _clients[i].getID(), _clients[i].get_PosX(), _clients[i].get_PosY());
//                 send_message(client_socket, response);
//                 //std::cout << "auth message successfuly sended\n";
//                 auth_nb++;
//                 break;
//         }
//     }
// }

void LobbyTCP::handle_authentification(std::shared_ptr<tcp::socket> client_socket, uint32_t length)
{
    if (length < sizeof(BinaryProtocol::MessageHeader) + sizeof(uint32_t)) {
        std::cerr << "Authentication data is too small." << std::endl;
        return;
    }

    // Extraire username_length
    BinaryProtocol::AuthMessage* auth_data = reinterpret_cast<BinaryProtocol::AuthMessage*>(_data.data() + sizeof(BinaryProtocol::MessageHeader));
    uint32_t username_length = auth_data->username_length;

    // //std::coit
    // if (username_length > 1024 || length < sizeof(BinaryProtocol::MessageHeader) + sizeof(uint32_t) + username_length) {
    //     std::cerr << "Invalid username length: " << username_length << std::endl;
    //     return;
    // }

    // Extraire le username
    std::string received_username(auth_data->username, username_length);

    std::cout << "Received username: " << received_username << std::endl;

    // Effectuer l'authentification...
    std::string status = "AUTHENTICATION_SUCCESS";

    for (int i = 0; i < _clients.size(); i++) {
        if (client_socket->remote_endpoint().address().to_string() == _clients[i].getIp()
            && client_socket->remote_endpoint().port() == _clients[i].getPort()) {

                _clients[i].setName(received_username);
                _players_uuids[_clients[i].getID()] = received_username;

            std::vector<char> response = BinaryProtocol::GetAuthSuccessMessage(status, _clients[i].getID(), _clients[i].get_PosX(), _clients[i].get_PosY());
            send_message(client_socket, response);
            auth_nb++;
            break;
        }
    }
}

void LobbyTCP::send_message(std::shared_ptr<tcp::socket> client_socket, std::vector<char> message)
{
    try {
        if (client_socket && client_socket->is_open()) {
            boost::asio::async_write(*client_socket, boost::asio::buffer(message),
                [this, client_socket, message](boost::system::error_code ec, std::size_t length) {
                    if (ec) {
                        std::cout << "Error while sending message: " << ec.message() << "\n";
                    } else {
                        std::cout << "Message sent successfully, length: " << length << ".\n";
                    }
            });

        } else {
            std::cout << "Message has not be sent due to client not connected\n";
        }
    } catch(const std::exception& e){
        std::cerr << e.what() << '\n';
    }
}

void  LobbyTCP::notify_all(std::vector<char> msg)
{
    for (const auto& cl : _client_sockets) {
        send_message(cl, msg);
    }
}

void LobbyTCP::handle_start(std::shared_ptr<tcp::socket> client_socket, uint32_t length)
{
    if (_clients.size() >= 1 && auth_nb == _client_sockets.size()) {
        std::cout << "enter start start\n";
        std::string status = "START_SUCCESS";
        std::vector<char> response = BinaryProtocol::GetStartSuccessMessage(game_id);
        notify_all(response);
        game_server_ready = true;
    } else {
        std::string status = "START_FAILURE NOT ENOUGH PLAYER";
        std::vector<char> reponse = BinaryProtocol::GetStartFailureMessage(status);
        std::cout << "SERVER: The start of the game failed\n";
        // notify all client that the start of the game as failed
        notify_all(reponse);
    }
}

void LobbyTCP::remove_client(std::shared_ptr<tcp::socket> client_socket)
{
    // Get the client's IP and port
    std::string client_ip = client_socket->remote_endpoint().address().to_string();
    unsigned short client_port = client_socket->remote_endpoint().port();
    int cl_ip = 0;

    for (int i = 0; i < _clients.size(); i++) {
        if (client_socket->remote_endpoint().address().to_string() == _clients[i].getIp()
        && client_socket->remote_endpoint().port() == _clients[i].getPort()) {
            cl_ip = _clients[i].getID();
        }
    }
    // Erase the client from the (client_info) vector
    _clients.erase(
        std::remove_if(_clients.begin(), _clients.end(),
            [&](const ClientInfo& client) {
                return client.getIp() == client_ip && client.getPort() == client_port;
            }),
        _clients.end());

    // Erase the client from the (client_sockets) vector
    _client_sockets.erase(
        std::remove_if(_client_sockets.begin(), _client_sockets.end(),
            [&](const std::shared_ptr<tcp::socket>& socket) {
                return socket->remote_endpoint().address().to_string() == client_ip &&
                       socket->remote_endpoint().port() == client_port;
            }),
        _client_sockets.end());

         std::cout << "Client "<< cl_ip << " is disconnected from the lobby\n";
         auth_nb--;

    int temp_id = 0;

    for (int i = 0; i < _clients.size(); i++) {
        if (_clients[i].getIp() == client_socket->remote_endpoint().address().to_string() && _clients[i].getPort() == client_socket->remote_endpoint().port()) {
            temp_id = _clients[i].getID();
        }
    }

    std::vector<char> msg = BinaryProtocol::GetClienClosetMessage(temp_id);

    for (const auto & cl: _client_sockets) {
        send_message(cl, msg);
    }
}

std::pair<int, int> LobbyTCP::generateUniquePosition(std::set<std::pair<int, int>>& existingPositions, int xMin, int xMax, int yMin, int yMax)
{
    std::pair<int, int> position;
    do {
        position.first = rand() % (xMax - xMin + 1) + xMin;
        position.second = rand() % (yMax - yMin + 1) + yMin;
    } while (existingPositions.find(position) != existingPositions.end());

    existingPositions.insert(position);
    return position;
}


void LobbyTCP::handle_level_choice(BinaryProtocol::MessageHeader* header, std::shared_ptr<tcp::socket> client_socket)
{
}

void LobbyTCP::handle_level_start(BinaryProtocol::MessageHeader* header, std::shared_ptr<tcp::socket> client_socket)
{
}

Level& LobbyTCP::getCurrentLevel(void)
{
    return choose_level;
}

std::string LobbyTCP::Generate_UUID(void)
{
    boost::uuids::uuid uuid = lobby_generator();

    const std::string random_uuid = boost::uuids::to_string(uuid);
    //boost::lexical_cast<std::string>(generator());
    return random_uuid;
}
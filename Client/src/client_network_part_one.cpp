/*
** EPITECH PROJECT, 2025
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** client_network_part_one
*/

#include "../include/client.hpp"


void Client::receive_tcp_message(Mediator& _mediator)
{
    const float wait_time = 0.02f;
    std::vector<char> buffer(2024);
    std::size_t byte_received = 0;

    if (_selector.wait(sf::seconds(0.016f))) {
        if (has_start || lobby_started) {
            if (_selector.isReady(tcp_socket)) {
                if (tcp_socket.receive(buffer.data(), buffer.size(), byte_received) != sf::Socket::Done) {
                    //std::cout << "(Tcp) Message not received" << std::endl;
                    return;
                }

                // if (byte_received < sizeof(BinaryProtocol::MessageHeader)) {
                //     std::cout << "(Tcp) Incomplete message received" << std::endl;
                //     return;
                // }

                BinaryProtocol::MessageHeader header;
                std::memcpy(&header, buffer.data(), sizeof(BinaryProtocol::MessageHeader));

                // // Check if the entire message is received
                // if (byte_received < sizeof(BinaryProtocol::MessageHeader) + header.length) {
                //     std::cout << "(Tcp) Incomplete message received" << std::endl;
                //     return;
                // }

                switch (header.type) {
                    case BinaryProtocol::MessageType::AUTHENTICATION_SUCCESS:
                        handle_auth_success(buffer);
                        break;
                    case BinaryProtocol::MessageType::AUTHENTICATION_FAILURE:
                        break;
                    case BinaryProtocol::MessageType::CLIENT_LIST:
                        handle_client_list(header, buffer);
                        break;
                    case BinaryProtocol::MessageType::NEW_CLIENT:
                        handle_new_client(header, buffer);
                        break;
                    case BinaryProtocol::MessageType::START_SUCCESS:
                        handle_start_success(header, buffer);
                        break;
                    case BinaryProtocol::MessageType::START_FAILURE:
                        handle_start_failure(header, buffer);
                        break;
                    case BinaryProtocol::MessageType::CLIENT_QUIT:
                        handle_close(header, buffer);
                        break;
                    case BinaryProtocol::MessageType::LEVEL_LIST:
                        //handle_level_loaded(header, buffer);
                        break;
                    case BinaryProtocol::MessageType::CURRENT_LEVEL:
                        //std::cout << "Enter current level";
                        handle_current_level(header, buffer);
                        break;
                    case BinaryProtocol::MessageType::GAME_ID:
                        create_game_session(header, buffer);
                    default:
                        //std::cout << "(Tcp) Unknown message type received" << std::endl;
                        break;
                }
            }
        }

        if (game_started) {
            if (_selector.isReady(udp_socket)) {
                if (udp_socket.receive(buffer.data(), buffer.size(), byte_received, recipient, port) != sf::Socket::Done) {
                    std::cout << "(Udp) Message not received" << std::endl;
                return;
            }

            //std::cout << "(Udp) Message received bytes(" << byte_received <<   ")"<< std::endl;

            BinaryProtocol::MessageHeader header;
            std::memcpy(&header, buffer.data(), sizeof(BinaryProtocol::MessageHeader));

            // Décompression si nécessaire
            std::vector<char> decompressed_buffer;
            if (header.compressed == 1) {
                uLong source_len = byte_received - sizeof(BinaryProtocol::MessageHeader);
                uLong dest_len = header.uncompressed_length;
                std::vector<char> decompressed_buffer(dest_len);

                int result = uncompress(reinterpret_cast<Bytef*>(decompressed_buffer.data()), &dest_len,
                                        reinterpret_cast<const Bytef*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader)), source_len);
                if (result != Z_OK) {
                    std::cerr << "(Udp) Decompression failed: " << result << std::endl;
                    return;
                }

                decompressed_buffer.resize(dest_len); // Ajuster à la taille décompressée réelle
                buffer = std::move(decompressed_buffer);
            }
            
            switch (header.type) {
                case BinaryProtocol::MessageType::CLIENT_UPDATE:
                    handle_update(header, buffer);
                    break;
                case BinaryProtocol::MessageType::ENEMY_SBIRES_CREATE:
                    //create_Enemy(header,  buffer);
                    break;
                case BinaryProtocol::MessageType::ENEMY_SBIRES_UPDATE:
                    update_Enemy(header,  buffer);
                    break;
                case BinaryProtocol::MessageType::SHOOT_UDPATE:
                    shoot_udpade(header, buffer);
                    break;
                case BinaryProtocol::MessageType::SHOOT_CREATE:
                    shoot_create(header, buffer);
                    break;
                case BinaryProtocol::MessageType::SIMPLE_ENEMY_CREATE:
                    create_simple_enemy(header, buffer);
                    break;
                case BinaryProtocol::MessageType::ENEMY_DELETE:
                    handle_enemy_delete(header, buffer);
                    break;
                case BinaryProtocol::MessageType::SCORE:
                    handle_score(header, buffer);
                    break;
                case BinaryProtocol::MessageType::SINGLE_ENEMY_UPDATE:
                    update_simple_enemy(header, buffer);
                    break;
                case BinaryProtocol::MessageType::DELETE_ONE_ENEMY:
                    delete_simple_enemy(header, buffer);
                    break;
                case BinaryProtocol::MessageType::CREATE_OBS:
                    create_obstacle(header, buffer);
                    break;
                case BinaryProtocol::MessageType::UPDATE_OBS:
                    break;
                case BinaryProtocol::MessageType::CREATE_POWER_UP:
                    create_power(header, buffer);
                    break;
                case BinaryProtocol::MessageType::UPDATE_POWER_UP:
                    update_power(header, buffer);
                    break;
                case BinaryProtocol::MessageType::PLAYER_HEALTH:
                   update_health(header, buffer);
                   break;
                case BinaryProtocol::MessageType::GAME_ID:
                    create_game_session(header, buffer);
                    break;
                case BinaryProtocol::MessageType::COLLISION:
                    update_collision(header, buffer);
                    break;
                case BinaryProtocol::MessageType::ENEMY_BULLET_CREATE:
                    //std::cout << "received enemy bullet create";
                    create_enemy_bullet(header, buffer);
                    break;
                case BinaryProtocol::MessageType::ENEMY_BULLET_UPDATE:
                    //std::cout << "received enemy bullet update";
                    update_enemy_bullet(header, buffer);
                    break;
                case BinaryProtocol::MessageType::BOSS_CREATE:
                    create_boss(header, buffer);
                    break;
                case BinaryProtocol::MessageType::BOSS_UPDATE:
                   update_boss(header, buffer);
                   break;
                case BinaryProtocol::MessageType::BOSS_INCOMMING:
                    handle_incomming_boss(header, buffer);
                    break;
                case BinaryProtocol::MessageType::GET_LEVEL:
                    handle_level(header, buffer);
                    break;
                case BinaryProtocol::MessageType::LEVEL_TIME:
                    handle_level_time(header, buffer);
                    break;
                case BinaryProtocol::MessageType::BOSS_SHOT:
                    handle_boss_shot(header, buffer);
                    break;
                case BinaryProtocol::MessageType::BOSS_DEATH:
                    handle_boss_death(header, buffer);
                    break;
                case BinaryProtocol::MessageType::TIMER:
                    handle_next_level(header, buffer);
                    break;
                case BinaryProtocol::MessageType::START_NEXT_LEVEL:
                    handle_start_next_level(header, buffer);
                    break;
                case BinaryProtocol::MessageType::LEVEL_INFO:
                    handle_level_info(header, buffer);
                    break;
                case BinaryProtocol::MessageType::GAME_FINISH:
                    handle_game_finish(header, buffer);
                    break;
                default:
                    std::cout << "(Udp) Unknown message type received" << std::endl;
                    std::cout << "unknow length" << header.length << "\n";
                    break;
            }            
        }
    }
    }
}

void Client::handle_auth_success(std::vector<char> buffer)
{
    BinaryProtocol::Auth* msg_data = reinterpret_cast<BinaryProtocol::Auth*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader));
    std::cout << "Received auth data - Player ID: " << msg_data->id << std::endl;

    _id = msg_data->id;

    // Créer l'entité locale pour le joueur
    player = _medi.createEntity();
    
    // Mapper l'ID du serveur (_id) avec l'entité locale (player)
    entityMapper.mapEntity(_id, player);
    
    // Initialiser la position
    player_position.x = msg_data->posX;
    player_position.y = msg_data->posY;

    // Ajouter les composants en utilisant l'ID local
    _medi.AddComponent(player, player_position);

    // Logique pour les sprites des joueurs
    if (msg_data->id == 1) {
        _medi.AddComponent(player, _manager.getSkins()[_manager.getSelectedSkin()]);
        _medi.AddComponent(player, Health{0});
    } else if (msg_data->id == 2) {
        _medi.AddComponent(player, _manager.getSkins()[_manager.getSelectedSkin()]);
        _medi.AddComponent(player, Health{0});
    } else if (msg_data->id == 3) {
        _medi.AddComponent(player, _manager.getSkins()[_manager.getSelectedSkin()]);
        _medi.AddComponent(player, Health{0});
    } else if (msg_data->id == 4) {
        _medi.AddComponent(player, _manager.getSkins()[_manager.getSelectedSkin()]);
        _medi.AddComponent(player, Health{0});
    }

    // Stocker les informations du joueur
    players.push_back({_id, player});

    std::cout << "Player created - Server ID: " << _id 
              << " Local Entity: " << player 
              << " Position: (" << player_position.x << ", " << player_position.y << ")" << std::endl;

    has_start = false;
    lobby_started = true;

    // Exemple d'utilisation du mapping pour vérification
    Entity localId = entityMapper.getLocalEntity(_id);
    if (localId == player) {
        std::cout << "Entity mapping successful" << std::endl;
    }
}

void Client::handle_client_list(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Client_list_t clientList;

    size_t offset = sizeof(header);
    std::memcpy(&clientList.clients_length, buffer.data() + offset, sizeof(clientList.clients_length));
    offset += sizeof(clientList.clients_length);

    std::memcpy(&clientList.nb_client, buffer.data() + offset, sizeof(clientList.nb_client));
    offset += sizeof(clientList.nb_client);

    std::memcpy(clientList.ids, buffer.data() + offset, sizeof(clientList.ids));
    offset += sizeof(clientList.ids);

    std::memcpy(clientList.posX, buffer.data() + offset, sizeof(clientList.posX));
    offset += sizeof(clientList.posY);

    std::memcpy(clientList.posY, buffer.data() + offset, sizeof(clientList.posY));

    for (int i = 0; i < clientList.nb_client; i++) {
        int serverId = clientList.ids[i];
        std::cout << "Creating client with Server ID: " << serverId << std::endl;

        // Créer l'entité locale
        Entity localEntity = _medi.createEntity();
        
        // Mapper l'entité locale avec l'ID serveur
        entityMapper.mapEntity(serverId, localEntity);

        std::pair<int, Entity> temp;
        temp.first = serverId;
        temp.second = localEntity;
    
        // Ajout des composants selon l'ID
        if (temp.first == 1) {
            if (is_moded == 1) {
                _medi.AddComponent(localEntity, _manager.getSpriteMap()["mod_player1"]);
                _medi.AddComponent(localEntity, Health{0});
            } else {
                _medi.AddComponent(localEntity, _manager.getPlayersSprites()[0]);
                _medi.AddComponent(localEntity, Health{0});
            }
            _medi.AddComponent(localEntity, Position{float(clientList.posX[i]), float(clientList.posY[i])});
        } else if (temp.first == 2) {
            if (is_moded == 1) {
                _medi.AddComponent(localEntity, _manager.getSpriteMap()["mod_player2"]);
                _medi.AddComponent(localEntity, Health{0});
            } else {
                _medi.AddComponent(localEntity, _manager.getPlayersSprites()[1]);
                _medi.AddComponent(localEntity, Health{0});
            }
            _medi.AddComponent(localEntity, Position{float(clientList.posX[i]), float(clientList.posY[i])});
        } else if (temp.first == 3) {
            if (is_moded == 1) {
                _medi.AddComponent(localEntity, _manager.getSpriteMap()["mod_player3"]);
                _medi.AddComponent(localEntity, Health{0});
            } else {
                _medi.AddComponent(localEntity, _manager.getPlayersSprites()[2]);
                _medi.AddComponent(localEntity, Health{0});
            }
            _medi.AddComponent(localEntity, Position{float(clientList.posX[i]), float(clientList.posY[i])});
        } else if (temp.first == 4) {
            if (is_moded == 1) {
                _medi.AddComponent(localEntity, _manager.getSpriteMap()["mod_player4"]);
                _medi.AddComponent(localEntity, Health{0});
            } else {
                _medi.AddComponent(localEntity, _manager.getPlayersSprites()[3]);
                _medi.AddComponent(localEntity, Health{0});
            }
            _medi.AddComponent(localEntity, Position{float(clientList.posX[i]), float(clientList.posY[i])});
        }
        players.push_back(temp);
        
        std::cout << "Created player - Server ID: " << serverId 
                  << " Local Entity: " << localEntity << std::endl;
    }
}

void Client::handle_new_client(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::NewClient* msg_data = reinterpret_cast<BinaryProtocol::NewClient*>(
        buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    Entity serverId = msg_data->id;
    Entity localEntity = _medi.createEntity();
    
    // Mapper l'entité locale avec l'ID serveur
    entityMapper.mapEntity(serverId, localEntity);

    std::pair<int, Entity> temp;
    temp.first = serverId;
    temp.second = localEntity;

    Position playerPos{float(msg_data->posX), float(msg_data->posY)};

    if (temp.first == 1) {
        if (is_moded == 1) {
            _medi.AddComponent(localEntity, _manager.getSpriteMap()["mod_player1"]);
            _medi.AddComponent(localEntity, Health{0});
        } else {
            _medi.AddComponent(localEntity, _manager.getPlayersSprites()[0]);
            _medi.AddComponent(localEntity, Health{0});
        }
        _medi.AddComponent(localEntity, playerPos);
    } else if (temp.first == 2) {
        if (is_moded == 1) {
            _medi.AddComponent(localEntity, _manager.getSpriteMap()["mod_player2"]);
            _medi.AddComponent(localEntity, Health{0});
        } else {
            _medi.AddComponent(localEntity, _manager.getPlayersSprites()[1]);
            _medi.AddComponent(localEntity, Health{0});
        }
        _medi.AddComponent(localEntity, playerPos);
    } else if (temp.first == 3) {
        if (is_moded == 1) {
            _medi.AddComponent(localEntity, _manager.getSpriteMap()["mod_player3"]);
            _medi.AddComponent(localEntity, Health{0});
        } else {
            _medi.AddComponent(localEntity, _manager.getPlayersSprites()[2]);
            _medi.AddComponent(localEntity, Health{0});
        }
        _medi.AddComponent(localEntity, playerPos);
    } else if (temp.first == 4) {
        if (is_moded == 1) {
            _medi.AddComponent(localEntity, _manager.getSpriteMap()["mod_player4"]);
            _medi.AddComponent(localEntity, Health{0});
        } else {
            _medi.AddComponent(localEntity, _manager.getPlayersSprites()[3]);
            _medi.AddComponent(localEntity, Health{0});
        }
        _medi.AddComponent(localEntity, playerPos);
    }

    players.push_back(temp);
    
    std::cout << "New client created - Server ID: " << serverId 
              << " Local Entity: " << localEntity 
              << " Position: (" << playerPos.x << ", " << playerPos.y << ")" << std::endl;
}

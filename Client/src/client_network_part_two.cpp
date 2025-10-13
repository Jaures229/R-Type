/*
** EPITECH PROJECT, 2025
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** client_network_part_two
*/

#include "../include/client.hpp"


void Client::handle_level_loaded(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol:: Level_list* msg_data = reinterpret_cast<BinaryProtocol:: Level_list*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    float windowWidth = 1920; // Remplacez par la largeur de votre fenêtre
    float windowHeight = 1080; // Remplacez par la hauteur de votre fenêtre

    float startY = windowHeight / 2;

    for (int i = 0; i < msg_data->string_count; i++) {
        sf::Text temp;
        temp.setString(msg_data->strings[i]);
        temp.setCharacterSize(30);
        temp.setFillColor(sf::Color::White);
        temp.setFont(_game_font);

        // Centrer le texte
        sf::FloatRect textRect = temp.getLocalBounds();
        temp.setOrigin(textRect.width / 2, textRect.height / 2);
        temp.setPosition(windowWidth / 2, (windowHeight / 2) + (i * 40));

        //temp.getString();
        levels_texts.push_back(temp);
    }
}



void Client::handle_start_success(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::SimpleMessage * msg_data = reinterpret_cast<BinaryProtocol::SimpleMessage*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    _game_session = msg_data->msg;

    //std::cout << "SUCCESS MESSAGE IS:\n";
    std::cout << msg_data->msg << "\n";

    lobby_started = false;
    has_start = false;

     if (udp_socket.bind(tcp_socket.getLocalPort()) != sf::Socket::Done) {
        std::cerr << "Error biding udp socket\n";
     } else {
        std::cout << "Success while binding on " << port + _id << "\n";
        _selector.add(udp_socket);
        std::vector<char> id_msg = BinaryProtocol::GetCleintIDMessage(_id);
        send_udp_message(id_msg);
        std::cout << "ok sended id\n";
        game_started = true;
     }
}

void Client::handle_start_failure(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::SimpleMessage * msg_data = reinterpret_cast<BinaryProtocol::SimpleMessage*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader));

     std::cout << "FAILURE MESSAGE IS:\n";
     std::cout << msg_data->msg << "\n";

     _text_status.setString(msg_data->msg);
}

void Client::handle_close(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Quit_client *msg_data = reinterpret_cast<BinaryProtocol::Quit_client*>(buffer.data() + sizeof(BinaryProtocol::Quit_client));

    std::cout << "closed\n";

    for (int i = 0; i < players.size(); i++) {
        if (players[i].first == msg_data->id) {
            _medi.deleteEntity(players[i].second);
            players.erase(players.begin() + 1);
            break;
        }
    }
}

void Client::handle_update(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Client_list_update* clientList = reinterpret_cast<BinaryProtocol::Client_list_update*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    //std::cout << "Player pos " << clientList->posX[0] << " " << clientList->posY[0] << "\n";

    for (int i = 0; i < clientList->nb_client; i++) {
        //std::cout << "Client ID: " << clientList->ids[i] << " Position X: " << clientList->posX[i] << " Position Y: " << clientList->posY[i] << "\n";
        // Find the player with the matching ID
        for (int j = 0; j < players.size(); j++) {
            if (players[j].first == clientList->ids[i]) {
                // Found the player, update its position
                auto &position = _medi.GetComponent<Position>(players[j].second);
                position.x = clientList->posX[i];
                position.y = clientList->posY[i];

                //std::cout << "Updated player with ID: " << clientList->ids[i] << " to new position X: " << position.x << " Y: " << position.y << "\n";
                break;
            }
        }
    }
}

void Client::update_Enemy(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Enemy_list* clientList = reinterpret_cast<BinaryProtocol::Enemy_list*>(
        buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    for (int i = 0; i < clientList->nb_client; i++) {
        Entity serverId = clientList->ids[i];
        Entity localEntity = entityMapper.getLocalEntity(serverId);

        if (localEntity != Entity(-1)) {
            auto& position = _medi.GetComponent<Position>(localEntity);
            position.x = clientList->posX[i];
            position.y = clientList->posY[i];
        }
    }
}

void Client::shoot_create(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Shoot_info* info = reinterpret_cast<BinaryProtocol::Shoot_info*>(
        buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    for (int i = 0; i < info->nb_client; i++) {
        Entity serverId = info->id[i];
        Entity localEntity = _medi.createEntity();
        
        // Mapper l'entité locale avec l'ID serveur
        entityMapper.mapEntity(serverId, localEntity);
        all_entity[serverId] = localEntity; // Garder la compatibilité avec le code existant

        // Ajouter la position
        _medi.AddComponent(localEntity, Position{float(info->posX[i]), float(info->posY[i])});

        // Ajouter le sprite approprié selon le type
        //std::string bulletSprite;

        if (info->type[i] == 0) {
            _medi.AddComponent(localEntity, _manager.getSpriteMap()["bullet"]);

        } if (info->type[i] == 1) {
            _medi.AddComponent(localEntity, _manager.getSpriteMap()["bulletv2"]);
            
        } if (info->type[i] == 2) {
            _medi.AddComponent(localEntity, _manager.getSpriteMap()["bulletv3"]);
            
        } if (info->type[i] == 3) {
            _medi.AddComponent(localEntity, _manager.getSpriteMap()["bulletv4"]);
            
        } if (info->type[i] == 4) {
            _medi.AddComponent(localEntity, _manager.getSpriteMap()["bulletv5"]);
        } if (info->type[i] == 5) {
            _medi.AddComponent(localEntity, _manager.getSpriteMap()["bulletv6"]);
        } if (info->type[i] == 6) {
            _medi.AddComponent(localEntity, _manager.getSpriteMap()["charge_attack"]);
            _medi.AddComponent(localEntity, _manager.getAnimationMap()["charge_attack"]);
            _animation_system->setAnimation(_medi, "idle", localEntity);
        }
        // std::cout << "Created projectile - Server ID: " << serverId 
        //           << " Local Entity: " << localEntity 
        //           << " Type: " << info->type[i] << std::endl;
    }
}

void Client::shoot_udpade(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Shoot_info* info = reinterpret_cast<BinaryProtocol::Shoot_info*>(
        buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    for (int i = 0; i < info->nb_client; i++) {
        Entity serverId = info->id[i];
        Entity localEntity = entityMapper.getLocalEntity(serverId);

        if (localEntity != Entity(-1)) {
            auto& position = _medi.GetComponent<Position>(localEntity);
            position.x = info->posX[i];
            position.y = info->posY[i];
        } else {
            std::cout << "Warning: Projectile with server ID " << serverId 
                      << " not found in local entity mapping" << std::endl;
        }
    }
}

void Client::handle_current_level(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::SimpleMessage * msg_data = reinterpret_cast<BinaryProtocol::SimpleMessage*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    std::string cur_lvl = "CHOOSE LEVEL: " + std::string(msg_data->msg);
    //std::cout << "this === " << cur_lvl << "\n";
    _choose_level.setString(cur_lvl);
    _current_level = msg_data->msg;
}

void Client::create_simple_enemy(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Enemy *enem = reinterpret_cast<BinaryProtocol::Enemy*>(
        buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    // Créer l'entité locale
    Entity localEntity = _medi.createEntity();
    Entity serverId = enem->ids;

    // Mapper l'entité locale avec l'ID serveur
    entityMapper.mapEntity(serverId, localEntity);
    all_entity[serverId] = localEntity; // Garder la compatibilité avec le code existant

    // Ajouter les composants nécessaires
    _medi.AddComponent(localEntity, Position({float(enem->posX), float(enem->posY)}));
    _medi.AddComponent(localEntity, _manager.getSpriteMap()[enem->type]);
    _medi.AddComponent(localEntity, _manager.getAnimationMap()[enem->type]);
    _animation_system->setAnimation(_medi, "idle", localEntity);

    // std::cout << "Created simple enemy - Server ID: " << serverId 
    //           << " Local Entity: " << localEntity 
    //           << " Type: " << enem->type 
    //           << " Position: (" << enem->posX << ", " << enem->posY << ")" << std::endl;
}

void Client::handle_enemy_delete(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Enemy_delete *enem = reinterpret_cast<BinaryProtocol::Enemy_delete*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    Entity localEntity = entityMapper.getLocalEntity(enem->id);
    if (localEntity != Entity(-1)) {
        entityMapper.removeMapping(localEntity);
        _medi.deleteEntity(localEntity);
        all_entity.erase(enem->id);
    }
}



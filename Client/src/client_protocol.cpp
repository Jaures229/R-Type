/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** client_protocol
*/

#include "../include/client.hpp"

void Client::update_simple_enemy(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
}

void Client::delete_simple_enemy(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Enemy_delete* enem = reinterpret_cast<BinaryProtocol::Enemy_delete*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader));


    Entity localEntity = entityMapper.getLocalEntity(enem->id);
    if (localEntity != Entity(-1)) {
        entityMapper.removeMapping(localEntity);
        _medi.deleteEntity(localEntity);
        all_entity.erase(enem->id);
    }

    // auto it = all_entity.find(enem->id);
    // if (it != all_entity.end()) {
    //     _medi.deleteEntity(it->second); // Supprime l'entité dans le médiateur
    //     all_entity.erase(it);           // Ensuite, supprimez l'entrée dans la map
    //     std::cout << "Enemy with ID " << enem->id << " deleted successfully\n";
    // } else {
    //     std::cerr << "Enemy with ID " << enem->id << " not found in all_entity\n";
    // }


}

void Client::create_obstacle(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    //Vérifier la taille du tampon
    if (buffer.size() < sizeof(BinaryProtocol::MessageHeader) + sizeof(BinaryProtocol::Obstacle)) {
        std::cerr << "Error: Buffer too small for Obstacle deserialization." << std::endl;
        return;
    }

    // Désérialisation
    BinaryProtocol::Obstacle* obs = reinterpret_cast<BinaryProtocol::Obstacle*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    // Valider nb_obstacle
    if (obs->nb_obstacle <= 0 || obs->nb_obstacle > 100) {
        std::cerr << "Error: Invalid number of obstacles in data (" << obs->nb_obstacle << ")." << std::endl;
        return;
    }
    // Traiter les obstacles
    for (int i = 0; i < obs->nb_obstacle; i++) {

        Entity serverEntityId = obs->ids[i];
        Entity localEntity = _medi.createEntity();  // Créer une nouvelle entité locale

        entityMapper.mapEntity(serverEntityId, localEntity);
        all_entity[serverEntityId] = localEntity;

        // Ajouter les composants en fonction du type
        if (obs->type == 1) {
            _medi.AddComponent(localEntity, _manager.getSpriteMap()["top_wall"]);
            _medi.AddComponent(localEntity, Position{float(obs->posX[i]), float(obs->posY[i])});
        }
        else if (obs->type == 2) {
            //std::cout << "oooo\n";
            _medi.AddComponent(localEntity, _manager.getSpriteMap()["back_wall"]);
            _medi.AddComponent(localEntity, Position{float(obs->posX[i]), float(obs->posY[i])});
        }
    }
}

void Client::create_power(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Power_up_create * msg_data = reinterpret_cast<BinaryProtocol::Power_up_create*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    Entity serverEntityId = msg_data->id;
    Entity localEntity = _medi.createEntity();  // Créer une nouvelle entité locale

    entityMapper.mapEntity(serverEntityId, localEntity);

    all_entity[serverEntityId] = localEntity;

    _medi.AddComponent(localEntity, Position{float(msg_data->posX), float(msg_data->posY)});

    if (msg_data->type == 1) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["Health"]);
    } else if (msg_data->type == 2) {
        // not yet implementated
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["Speed"]);
        // _medi.AddComponent(all_entity[msg_data->id], _manager.getAnimationMap()["Speed"]);
    } else if (msg_data->type == 3) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["Power"]);
    }
}

void Client::update_power(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Power_up_update * msg_data = reinterpret_cast<BinaryProtocol::Power_up_update*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    Entity serverEntityId = msg_data->id;
    Entity localEntity = entityMapper.getLocalEntity(serverEntityId);

    if (localEntity != Entity(-1)) {
        auto& pos = _medi.GetComponent<Position>(localEntity);
        pos.x = msg_data->posX;
        pos.y = msg_data->posY;
    }
}

void Client::create_game_session(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::SimpleMessage * msg_data = reinterpret_cast<BinaryProtocol::SimpleMessage*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    _game_session = std::string(msg_data->msg);

    std::cout << "received game id" << _game_session << "\n";
}

void Client::load_replays()
{
    // load all the replays list in th file
}

void Client::update_health(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Players_health* info = reinterpret_cast<BinaryProtocol::Players_health*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    for (int i = 0; i < info->nb_client; i++) {
        for (int j = 0; j < players.size(); j++) {
            if (players[j].first == info->ids[i]) {
                auto &health = _medi.GetComponent<Health>(players[j].second);
                health.life = info->health[i];
                break;
            }
        }
    }
    //std::cout << "Player health == " << info->health << "\n";
}

void Client::update_collision(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::All_collision * info = reinterpret_cast<BinaryProtocol::All_collision*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    for (int i = 0; i < info->nb_collision; i++) {

        Collider c;
        c.type = info->type[i];
        c.x = info->x[i];
        c.y = info->y[i];

        collision_to_display.push_back(c);
    }
}

void Client::create_enemy_bullet(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Protocol_Enemy_Bullet_create* enem = 
        reinterpret_cast<BinaryProtocol::Protocol_Enemy_Bullet_create*>(
            buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    Entity serverEntityId = enem->ids;
    Entity localEntity = _medi.createEntity();

    // Mapper l'entité locale à l'ID du serveur
    entityMapper.mapEntity(serverEntityId, localEntity);
    all_entity[serverEntityId] = localEntity;  // Garder la compatibilité avec le code existant

    // std::cout << "Created enemy bullet - Server ID: " << serverEntityId 
    //           << " Local ID: " << localEntity << "Position x: " << enem->x << " Position y:" << enem->y << "\n";

    // Ajouter les composants en utilisant l'ID local
    _medi.AddComponent(localEntity, Position{float(enem->x), float(enem->y)});

    if (enem->type == 1) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["simple_bullet"]);
        _medi.AddComponent(localEntity, _manager.getAnimationMap()["simple_bullet"]);
        _animation_system->setAnimation(_medi, "idle", localEntity);
    } else if (enem->type == 2) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["simple_bullet"]);
        _medi.AddComponent(localEntity, _manager.getAnimationMap()["simple_bullet"]);
        _animation_system->setAnimation(_medi, "idle", localEntity);
    } else if (enem->type == 3) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["bullet_type3"]);
        _medi.AddComponent(localEntity, _manager.getAnimationMap()["bullet_type3"]);
        _animation_system->setAnimation(_medi, "idle", localEntity);   
    }  else if (enem->type == 4) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["FireBall"]);
        _medi.AddComponent(localEntity, _manager.getAnimationMap()["FireBall"]);
        _animation_system->setAnimation(_medi, "idle", localEntity);   
    } else if (enem->type == 5) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["Breath"]);
        _medi.AddComponent(localEntity, _manager.getAnimationMap()["Breath"]);
        _animation_system->setAnimation(_medi, "idle", localEntity);   
    } else if (enem->type == 6) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["Wave"]);
        _medi.AddComponent(localEntity, _manager.getAnimationMap()["Wave"]);
        _animation_system->setAnimation(_medi, "idle", localEntity);   
    } else if (enem->type == 7) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["missile"]);
    } else if (enem->type == 8) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["laser"]);
    } else if (enem->type == 9) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["bullet_type3"]);
        _medi.AddComponent(localEntity, _manager.getAnimationMap()["bullet_type3"]);
        _animation_system->setAnimation(_medi, "idle", localEntity);   
    }
    soundManager.playSound("enemy-shot");
}

void Client::update_enemy_bullet(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Protocol_Enemy_Bullet * enem = reinterpret_cast<BinaryProtocol::Protocol_Enemy_Bullet*>(buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    for (int i = 0; i < enem->nb_bullet; i++) {
        Entity serverEntityId = enem->ids[i];
        Entity localEntity = entityMapper.getLocalEntity(serverEntityId);

        if (localEntity != Entity(-1)) {
            auto& pos = _medi.GetComponent<Position>(localEntity);

            //std::cout << "Enemy bullet " << localEntity << " Position x: " << enem->x[i] << " Position y:" << enem->y[i] << "\n";
            pos.x = enem->x[i];
            pos.y = enem->y[i];
        }
    }
}

void Client::create_boss(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Boss_create *info = 
        reinterpret_cast<BinaryProtocol::Boss_create*>(
            buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    Entity serverEntityId = info->id;
    Entity localEntity = _medi.createEntity();

    entityMapper.mapEntity(serverEntityId, localEntity);
    all_entity[serverEntityId] = localEntity;

    _medi.AddComponent(localEntity, Position{float(info->x), float(info->y)});
    _medi.AddComponent(localEntity, Health{info->health});
    _medi.AddComponent(localEntity, Boss{info->health});

    _medi.AddComponent(localEntity, _manager.getSpriteMap()[info->type]);
    _medi.AddComponent(localEntity, _manager.getAnimationMap()[info->type]);
    _animation_system->setAnimation(_medi, "idle", localEntity);

    std::cout << "Create Boss " << info->type << " at " << "position x = " << info->x << " position y = " << info->y << "with health " << info->health << std::endl;
    Current_boss = std::string(info->type);

    Boss_ref = localEntity;

    soundManager.playGameMusic(info->type);
}

void Client::update_boss(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    //std::cout << "Update boss\n";

    BinaryProtocol::Boss_update *info = 
        reinterpret_cast<BinaryProtocol::Boss_update*>(
            buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    Entity serverEntityId = info->id;
    Entity localEntity = entityMapper.getLocalEntity(serverEntityId);


    if (localEntity != Entity(-1)) {
        auto& pos = _medi.GetComponent<Position>(localEntity);
        auto &health =  _medi.GetComponent<Health>(localEntity);
    
        pos.x = info->x;
        pos.y = info->y;
        health.life = info->health;
    }
}

void Client::handle_incomming_boss(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    std::cout << "Receive incomming boss\n";
    is_boss_comming = 1;
}

void Client::handle_level(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    std::cout << "Receive level\n";

    BinaryProtocol::SimpleMessage *info = 
        reinterpret_cast<BinaryProtocol::SimpleMessage*>(
            buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    current_level = info->msg;

    if (_manager.getBackgroundSprites().begin() == _manager.getBackgroundSprites().end()) {
        std::cerr << "Error: Level " << current_level << " not found in sprite map.\n";
        return;
    }

    backgroundLayer1.setTexture(_manager.getBackgroundSprites()[current_level].texture);
    backgroundLayer2.setTexture(_manager.getBackgroundSprites()[current_level].texture);

    backgroundLayer1.setPosition(0.f, 0.f);
    backgroundLayer2.setPosition(backgroundLayer1.getGlobalBounds().width, 0.f);

    soundManager.playGameMusic(current_level);
    std::wstring ws (current_level.begin(), current_level.end() );
    Level_text.setString(ws);

    float padding = 20.f;

    // Level_text.setPosition(padding,
    //     _window.getSize().y - Level_text.getGlobalBounds().height - 20.f
    // );
    Level_text.setPosition(
    (1920 - Level_text.getGlobalBounds().width) / 2,  // X : centré horizontalement
    padding  // Y : padding depuis le haut
);
}

void Client::handle_level_time(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Level_time *info = 
        reinterpret_cast<BinaryProtocol::Level_time*>(
            buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    level_time = info->time;

    // Utilisation d'un ostringstream pour formater la précision
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << level_time;

    // Mise à jour du texte avec le formatage
    level_time_text.setString("TIME: " + oss.str());
}

void Client::handle_boss_shot(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::Protocol_Enemy_Bullet_create* enem = 
        reinterpret_cast<BinaryProtocol::Protocol_Enemy_Bullet_create*>(
            buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    Entity serverEntityId = enem->ids;
    Entity localEntity = _medi.createEntity();

    // Mapper l'entité locale à l'ID du serveur
    entityMapper.mapEntity(serverEntityId, localEntity);
    all_entity[serverEntityId] = localEntity;  // Garder la compatibilité avec le code existant

    std::cout << "Created enemy bullet - Server ID: " << serverEntityId 
              << " Local ID: " << localEntity << "Position x: " << enem->x << " Position y:" << enem->y << "type :" << enem->type << "\n";

    // Ajouter les composants en utilisant l'ID local
    _medi.AddComponent(localEntity, Position{float(enem->x), float(enem->y)});

    if (enem->type == 1) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["simple_bullet"]);
        _medi.AddComponent(localEntity, _manager.getAnimationMap()["simple_bullet"]);
        _animation_system->setAnimation(_medi, "idle", localEntity);
    } else if (enem->type == 2) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["simple_bullet"]);
        _medi.AddComponent(localEntity, _manager.getAnimationMap()["simple_bullet"]);
        _animation_system->setAnimation(_medi, "idle", localEntity);
    } else if (enem->type == 3) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["bullet_type3"]);
        _medi.AddComponent(localEntity, _manager.getAnimationMap()["bullet_type3"]);
        _animation_system->setAnimation(_medi, "idle", localEntity);   
    } else if (enem->type == 4) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["FireBall"]);
        _medi.AddComponent(localEntity, _manager.getAnimationMap()["FireBall"]);
        _animation_system->setAnimation(_medi, "idle", localEntity);   
    } else if (enem->type == 5) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["Breath"]);
        _medi.AddComponent(localEntity, _manager.getAnimationMap()["Breath"]);
        _animation_system->setAnimation(_medi, "idle", localEntity);   
    } else if (enem->type == 6) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["Wave"]);
        _medi.AddComponent(localEntity, _manager.getAnimationMap()["Wave"]);
        _animation_system->setAnimation(_medi, "idle", localEntity);   
    } else if (enem->type == 7) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["missile"]);
    } else if (enem->type == 8) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["laser"]);
    } else if (enem->type == 9) {
        _medi.AddComponent(localEntity, _manager.getSpriteMap()["bullet_type3"]);
        _medi.AddComponent(localEntity, _manager.getAnimationMap()["bullet_type3"]);
        _animation_system->setAnimation(_medi, "idle", localEntity);   
    }
    soundManager.playSound("boos_shot");
}


void Client::handle_boss_death(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    Boss_ref = -1;
    // to draw next level passed;
    soundManager.stopBackgroundMusic();
    soundManager.playSound("Boss-Defeated");
    level_passed = 1;

    //std::cout << "Receivend level passed << " << level_passed << "\n";
}

void Client::handle_next_level(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    // to draw next level passed;

    BinaryProtocol::Level_time *info = 
        reinterpret_cast<BinaryProtocol::Level_time*>(
            buffer.data() + sizeof(BinaryProtocol::MessageHeader));

    float timer_time = info->time;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << timer_time;

    // Mise à jour du texte avec le formatage
    Timer_text.setString("TIME BEFORE NEXT LEVEL:" + oss.str());
}

void Client::handle_start_next_level(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    level_passed = 0;
}

void Client::handle_level_info(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    BinaryProtocol::NextLevel_Protocol *info = 
        reinterpret_cast<BinaryProtocol::NextLevel_Protocol*>(
            buffer.data() + sizeof(BinaryProtocol::MessageHeader));   
    

    std::string current_info = "STAGE " + std::to_string(info->current) + "/" + std::to_string(info->total);
    Level_info_text.setString(current_info);
}

void Client::handle_game_finish(BinaryProtocol::MessageHeader header, std::vector<char> buffer)
{
    // Vider le scoreboard avant de le remplir à nouveau
    scoreboard.clear();

    // Décomprimer et récupérer les informations du message
    BinaryProtocol::Protocol_GameFinish *info = 
        reinterpret_cast<BinaryProtocol::Protocol_GameFinish*>(
            buffer.data() + sizeof(BinaryProtocol::MessageHeader)); 

    // Ajouter les joueurs au scoreboard
    for (int i = 0; i < info->playerCount; i++) {
        std::pair<std::string, int> player_score;

        player_score.first = std::string(info->players[i].name);
        player_score.second = info->players[i].score;

        std::cout << "Player " << player_score.first << " score " << player_score.second << "\n";
        scoreboard.push_back(player_score);
    }

    // Dimensions de la fenêtre
    sf::Vector2u windowSize = _window.getSize(); // Assurez-vous que "window" est votre sf::RenderWindow

    // Ajouter le header pour le scoreboard
    sf::Text header_text;
    header_text.setFont(_game_font);
    header_text.setOutlineColor(sf::Color::White);
    header_text.setCharacterSize(40); // Taille du texte (plus grand que le reste pour se démarquer)
    header_text.setFillColor(sf::Color::Yellow); // Couleur différente pour le header
    header_text.setString("Scoreboard"); // Titre

    // Calcul pour centrer le header
    sf::FloatRect headerBounds = header_text.getLocalBounds();
    header_text.setPosition(
        (windowSize.x - headerBounds.width) / 2, // Centre horizontal
        10                                      // Position verticale
    );
    scoreboard_text.push_back(header_text);

    // Ajouter les scores des joueurs
    for (size_t i = 0; i < scoreboard.size(); ++i) {
        sf::Text text;
        text.setFont(_game_font);
        text.setOutlineColor(sf::Color::White);
        text.setCharacterSize(30); // Taille du texte
        text.setFillColor(sf::Color::White); // Couleur du texte

        // Construire le texte du joueur
        text.setString(std::to_string(i + 1) + ". " + scoreboard[i].first + " - " + std::to_string(scoreboard[i].second));

        // Calcul pour centrer le texte
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            (windowSize.x - textBounds.width) / 2, // Centre horizontal
            50 + i * 30                            // Verticalement en dessous du header
        );

        scoreboard_text.push_back(text);
    }

    game_finish = 1;    
}

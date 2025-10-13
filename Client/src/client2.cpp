/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** client2
*/


#include "../include/client.hpp"

void Client::defaultColorSettingButton()
{
    upButton.setFillColor(sf::Color::White);
    downButton.setFillColor(sf::Color::White);
    leftButton.setFillColor(sf::Color::White);
    rightButton.setFillColor(sf::Color::White);
    shootButton.setFillColor(sf::Color::White);
    saveButton.setFillColor(sf::Color::White);
}

void Client::saveKeyBindingsToFile()
{
    std::ofstream file("key_bindings_" + std::to_string(_id)+ ".txt");
    if (!file.is_open()) {
        std::cerr << "Waiting" << std::endl;
        return;
    }
    // Ecrire chaque clé avec la touche assignée
    for (const auto& pair : keyMapDefault) {
        file << pair.first << ": " << keyAssigned(pair.second) << std::endl;
    }
    file.close();
    std::cout << "Configuration des touches sauvegardée" << std::endl;
}

void Client::loadKeyBindingsFromFile()
{
    std::ifstream file("key_bindings_" + std::to_string(_id)+ ".txt");
    if (!file.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier pour la lecture." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string action, keyName;
        if (std::getline(iss, action, ':') && std::getline(iss, keyName)) {
            // Supprimer les espaces en début et fin
            action.erase(action.find_last_not_of(" \t") + 1);
            keyName.erase(0, keyName.find_first_not_of(" \t"));
            keyName.erase(keyName.find_last_not_of(" \t") + 1);

            // Trouver la touche correspondant à `keyName`
            auto it = keyb_list.find(keyName);
            if (it != keyb_list.end()) {
                keyMapDefault[action] = it->second;
            } else {
                std::cerr << "Touche non reconnue : " << keyName << " pour l'action " << action << std::endl;
            }
        }
    }

    file.close();
}

void Client::Set_health_bar()
{
    if (game_finish == 0) {
        for (int i = 0; i < players.size(); i++) {
            auto& position = _medi.GetComponent<Position>(players[i].second);
            auto& health = _medi.GetComponent<Health>(players[i].second);
            auto& sprite = _medi.GetComponent<Sprite>(players[i].second);

            int player_health = health.life;
            float posX = position.x;
            float posY = position.y;

            float sprite_width = sprite.sprite.getGlobalBounds().width;
            float sprite_height = sprite.sprite.getGlobalBounds().height;

            float health_percentage = static_cast<float>(player_health) / 100.f;
            float bar_width = max_width * health_percentage;
            
            float vertical_offset = sprite_height / 2 - 10;

            sf::RectangleShape border(sf::Vector2f(max_width + 2 * border_thickness, height + 2 * border_thickness));
            border.setFillColor(sf::Color::White);
            border.setPosition(
                posX + sprite_width / 2 - max_width / 2 - border_thickness,
                posY - vertical_offset - border_thickness
            );

            sf::RectangleShape health_bar(sf::Vector2f(bar_width, height));
            health_bar.setFillColor(sf::Color::Red);
            health_bar.setPosition(
                posX + sprite_width / 2 - max_width / 2,
                posY - vertical_offset
            );

            _window.draw(border);
            _window.draw(health_bar);

        }
    }
}

void Client::display_boss_health_bar()
{
    if (Boss_ref != -1) {
        // Récupérer les composants du boss
        auto &Boss_position = _medi.GetComponent<Position>(Boss_ref);
        auto &Boss_health = _medi.GetComponent<Health>(Boss_ref);
        auto &Boss_type = _medi.GetComponent<Boss>(Boss_ref);

        // Calculer le pourcentage de vie
        float health_percentage = static_cast<float>(Boss_health.life) / Boss_type.life;
        
        // Dimensions de la fenêtre
        float windowWidth = _window.getSize().x;
        float windowHeight = _window.getSize().y;

        // Texte du nom du boss
        sf::Text bossText;
        bossText.setFont(_game_font);
        bossText.setString(Current_boss);
        bossText.setCharacterSize(50);
        bossText.setFillColor(sf::Color::Red);
        
        // Centrer le texte horizontalement
        sf::FloatRect textBounds = bossText.getLocalBounds();
        bossText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
        bossText.setPosition(windowWidth / 2.0f, windowHeight * 0.05f); // Positionné plus haut

        // Configuration de la barre de vie
        sf::RectangleShape backgroundBar(sf::Vector2f(300, 30));
        backgroundBar.setFillColor(sf::Color(100, 100, 100, 200)); // Gris semi-transparent pour l'arrière-plan
        backgroundBar.setOrigin(backgroundBar.getLocalBounds().width / 2.0f, backgroundBar.getLocalBounds().height / 2.0f);
        backgroundBar.setPosition(windowWidth / 2.0f, windowHeight * 0.1f);

        // Configuration de la barre de vie
        sf::RectangleShape healthBar(sf::Vector2f(300 * health_percentage, 30));
        healthBar.setFillColor(sf::Color::Red);
        healthBar.setOrigin(healthBar.getLocalBounds().width / 2.0f, healthBar.getLocalBounds().height / 2.0f);
        healthBar.setPosition(backgroundBar.getPosition());

        // Configuration de la bordure blanche autour de la barre de vie
        sf::RectangleShape borderBar(sf::Vector2f(300 + 4, 30 + 4)); // Légèrement plus grand que la barre de vie
        borderBar.setFillColor(sf::Color::Transparent);
        borderBar.setOutlineThickness(2); // Épaisseur de la bordure
        borderBar.setOutlineColor(sf::Color::White);
        borderBar.setOrigin(borderBar.getLocalBounds().width / 2.0f, borderBar.getLocalBounds().height / 2.0f);
        borderBar.setPosition(windowWidth / 2.0f, windowHeight * 0.1f);

        // Dessiner les éléments
        _window.draw(bossText);
        _window.draw(borderBar); // Dessiner la bordure avant la barre de vie
        _window.draw(backgroundBar); // Dessiner la barre de fond
        _window.draw(healthBar); // Dessiner la barre de vie
    }
}

void Client::updateLobbyAnimation()
{
    if (animationClock.getElapsedTime().asSeconds() >= frameDuration) {
        currentFrame = (currentFrame + 1) % lobbyFrames.size();
        animatedLobbySprite.setTexture(lobbyFrames[currentFrame]);
        animationClock.restart();
    }
}

void Client::enter_lobby_event()
{
    if (_start_button.getGlobalBounds().contains(sf::Vector2f(mousePos)) && (sf::Mouse::isButtonPressed(sf::Mouse::Left))) {
        _start_button.setFillColor(sf::Color::Red);
        std::string start_msg = "START";
            //std::cout << "sended start with " << _current_level << "\n";
        std::vector<char> msg = BinaryProtocol::GetStartMessage(start_msg);
            //std::vector<char> msg = BinaryProtocol::GetStartLevelMessage(_current_level);
        send_tcp_message(msg);
    }
}

std::string Client::keyAssigned(sf::Keyboard::Key key) {
    std::string _key;
    for (const auto& pair : keyb_list) {
        if (key == pair.second) {
            std::cout << "Touche appuyée : " << pair.first << std::endl;
            _key = pair.first;
        }
    }
    return _key;
}

void Client::InputManagement()
{
    static sf::Clock shootCooldownClock;
    const float shootCooldown = 0.2f;

        if (sf::Keyboard::isKeyPressed(keyMapDefault["Up"])) {
            std::vector<char> msg = BinaryProtocol::GetUPMesaage(_id);
            send_udp_message(msg);
        }
        if (sf::Keyboard::isKeyPressed(keyMapDefault["Down"])) {
            std::vector<char> msg = BinaryProtocol::GetDownMesaage(_id);
            send_udp_message(msg);
        }
        if (sf::Keyboard::isKeyPressed(keyMapDefault["Left"])) {
            std::vector<char> msg = BinaryProtocol::GetLeftMesaage(_id);
            send_udp_message(msg);
        }
        if (sf::Keyboard::isKeyPressed(keyMapDefault["Right"])) {
            std::vector<char> msg = BinaryProtocol::GetRightMesaage(_id);
            send_udp_message(msg);
        }
        if (sf::Keyboard::isKeyPressed(keyMapDefault["Shoot"])) {
            if (shootCooldownClock.getElapsedTime().asSeconds() >= shootCooldown) {
                std::vector<char> msg = BinaryProtocol::GetShootMesaage(_id);
                send_udp_message(msg);
                soundManager.playSound("shot");
                shootCooldownClock.restart();
            }
        }

        if (sf::Keyboard::isKeyPressed(keyMapDefault["Charge"])) {
            if (!isCharging) {
                // Commencer à charger
                chargeClock.restart();
                isCharging = true;
                soundManager.playSound("charge");
            }

            // Mise à jour de la barre de charge
            float chargeTime = chargeClock.getElapsedTime().asSeconds();
            float progress = std::min(chargeTime / chargeThreshold, 1.0f); // Limite à 1.0

            sf::Color dynamicColor = sf::Color(255 * (1 - progress), 255 * progress, 50); // Couleur dynamique
            chargeBar.setFillColor(dynamicColor);
            chargeBar.setSize(sf::Vector2f(chargeBarWidth * progress, chargeBarHeight));
        } else if (isCharging) {
            // Relâcher la touche de charge
            float chargeTime = chargeClock.getElapsedTime().asSeconds();
            isCharging = false;

            //soundManager.stopSong("charge"); // Stop le son de charge

            if (chargeTime >= chargeThreshold) {
                // Charge complète : attaque puissante
                std::vector<char> msg = BinaryProtocol::GetChargeAttack(_id);
                send_udp_message(msg);
                soundManager.playSound("shot");
            } else {
                // // Charge incomplète : attaque faible ou annulée
                // soundManager.playSound("cancel_charge");
            }
            // Réinitialiser la barre de charge
            chargeBar.setSize(sf::Vector2f(0, chargeBarHeight));
        }
}

void  Client::close_game()
{
    stop_recording();
    _window.close();
}

void Client::display_explosion()
{
    for (size_t i = 0; i < active_explosions.size(); ++i) {
        Explosion& explosion = active_explosions[i];

        // Vérifiez si assez de temps s'est écoulé pour changer de frame
        if (explosion.animation_timer.getElapsedTime().asMilliseconds() > 30) { // 100 ms par frame
            explosion.current_frame++;
            explosion.animation_timer.restart();
        }

        // Si l'animation est terminée, retirez l'explosion
        if (explosion.current_frame >= simple_explosion.size()) {
            active_explosions.erase(active_explosions.begin() + i);
            --i; // Ajuster l'indice après la suppression
            continue;
        }

        // Affiche la frame actuelle de l'explosion
        sf::Sprite& frame = simple_explosion[explosion.current_frame];
        frame.setPosition(explosion.x, explosion.y);

        // Jouez le son d'explosion une seule fois par explosion
        if (explosion.current_frame == 0) {  // Le son est joué uniquement lors de la première frame
            soundManager.playSound("explosion");
        }

        _window.draw(frame);
    }
}

void Client::add_explosion(float x, float y) {
    Explosion explosion;
    explosion.x = x;
    explosion.y = y;
    active_explosions.push_back(explosion);
}


void Client::display_collison()
{
    if (!collision_to_display.empty()) {
        for (auto& collision : collision_to_display) {
            if (collision.type == 0) {
                add_explosion(collision.x, collision.y); // Ajouter une nouvelle explosion
            } else if (collision.type == 1) {
                // Autres types de collisions
            }
        }
        collision_to_display.clear();
    }
    display_explosion();
}

void Client::display_warning()
{
    // Démarrer l'animation si le boss arrive
    if (is_boss_comming == 1 && !isAnimating) {
        isAnimating = true;
        warningTimer.restart();
        soundManager.playSound("warning");
    }

    if (isAnimating) {
        float elapsedTime = warningTimer.getElapsedTime().asSeconds();

        // Calculer l'index de la frame actuelle (20 FPS, 19 frames)
        warningcurrentFrame = static_cast<int>(elapsedTime * 20) % 19;

        // Utiliser le sprite correspondant
        sf::Sprite warningSprite = warning_animation[warningcurrentFrame];

        // Positionner le sprite dans le coin supérieur droit
        float windowWidth = _window.getSize().x;
        warningSprite.setPosition(windowWidth - warningSprite.getGlobalBounds().width, 0);

        // Dessiner le sprite
        _window.draw(warningSprite);

        // Terminer l'animation après 5 secondes
        if (elapsedTime >= 3.0f) {
            isAnimating = false;
            is_boss_comming = 0; // Réinitialiser le flag
        }
    }
}

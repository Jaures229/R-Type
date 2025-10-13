/*
** EPITECH PROJECT, 2025
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** client_update
*/

#include "../include/client.hpp"


void Client::run()
{
    game_loop();
}

void Client::game_loop()
{
    // Initialisation des composants ECS
    _medi.RegisterComponent<Position>();
    _medi.RegisterComponent<Sprite>();
    _medi.RegisterComponent<Health>();
    _medi.RegisterComponent<AnimationComponent>();
    _medi.RegisterComponent<Boss>();

    auto _render_system = _medi.RegisterSystem<RenderSystem>();
    _animation_system = _medi.RegisterSystem<AnimationSystem>();

    _signatureDisplay.set(_medi.GetComponentType<Position>());
    _signatureDisplay.set(_medi.GetComponentType<Sprite>());

    _signatureAnimation.set(_medi.GetComponentType<Sprite>());
    _signatureAnimation.set(_medi.GetComponentType<AnimationComponent>());

    _medi.SetSystemSignature<RenderSystem>(_signatureDisplay);
    _medi.SetSystemSignature<AnimationSystem>(_signatureAnimation);

    sf::Listener::setGlobalVolume(100.f);

    // Création de l'écran de saisie du nom
    NameEntryScreen nameScreen(_window);
    bool nameEntered = false;
    std::string playerName;

    // Mediator test;
    // test.Init();

    // SoloPlay _soloplay(_window, test);

    while (_window.isOpen()) {
        while (_window.pollEvent(_event)) {
            if (_event.type == sf::Event::Closed) {
                send_close_message();
                close_game();
            }

            // Si le nom n'a pas encore été entré, on gère l'écran de saisie
            if (!nameEntered) {
                nameScreen.handleEvent(_event);
                if (nameScreen.isFinished()) {
                    playerName = nameScreen.getPlayerName();
                    nameEntered = true;
                    has_start = true;  // Active le menu principal
                    // Vous pouvez stocker le nom du joueur ici
                    player_name = playerName;

                    //std::cout << "PLayer name is " << player_name;
                    soundManager.playMenuMusic();
                }
                continue;  // On ne traite pas les autres events tant que le nom n'est pas entré
            }

            // Une fois le nom entré, on gère le reste du jeu normalement
            if (has_start) {
                enter_game_event();
            }

            if (lobby_started) {
                enter_lobby_event();
            }
            // if (in_solo_play) {
            //     _soloplay.handle_event(_event);
            // }

            if (in_skin) {
                _manager.handleEvents(_window, _event);
                if (_event.type == sf::Event::MouseButtonPressed && 
                    _back_text.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    in_skin = false;
                    has_start = true;
                }
            }
        }

        _window.clear();  // Nettoie la fenêtre au début de chaque frame

        // Si le nom n'est pas encore entré, on affiche uniquement l'écran de saisie
        if (!nameEntered) {
            nameScreen.update();
            nameScreen.draw();
        } else {
            // Logique normale du jeu une fois le nom entré
            if (has_start || lobby_started || in_skin || in_solo_play) {
                receive_tcp_message(_medi);
            }
            mousePos = sf::Mouse::getPosition(_window);

            if (game_started) {
                game_update(_render_system);
            }
            // if (in_solo_play) {
            //     _soloplay.run();
            // }
            soundManager.update();
            update();
        }
        _window.display();
    }
}

void Client::enter_game_event()
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(_window);

    if (_start_text.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        _start_text.setFillColor(sf::Color::Red);
        if (!isHoveringStartText) { // Si la souris commence à survoler
            soundManager.playSound("hover");
            isHoveringStartText = true;
        }
    } else {
        _start_text.setFillColor(sf::Color::White);
        isHoveringStartText = false; // Réinitialiser l'état
    }

    if (_setting_button.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        _setting_button.setFillColor(sf::Color::Red);
        if (!isHoveringSettingButton) {
            soundManager.playSound("hover");
            isHoveringSettingButton = true;
        }
    } else {
        _setting_button.setFillColor(sf::Color::White);
        isHoveringSettingButton = false;
    }

    if (_quit_text.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        _quit_text.setFillColor(sf::Color::Red);
        if (!isHoveringQuitText) {
            soundManager.playSound("hover");
            isHoveringQuitText = true;
        }
    } else {
        _quit_text.setFillColor(sf::Color::White);
        isHoveringQuitText = false;
    }

    if (_replay_text.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        if (!isHoveringReplayText) {
            //ModedButton.setFillColor(sf::Color::Blue);
            soundManager.playSound("hover");
            isHoveringReplayText = true;
        }
    } else {
        _replay_text.setFillColor(sf::Color::White);
        isHoveringReplayText = false;
    }

    if (ModedButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        if (!isHoveringModedText) {
            ModedButton.setFillColor(sf::Color::Red);
            soundManager.playSound("hover");
            isHoveringModedText = true;
        }
    } else {
        ModedButton.setFillColor(sf::Color::White);
        isHoveringModedText = false;
    }

    if (_event.type == sf::Event::MouseButtonPressed && _start_text.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
        soundManager.playSound("click");
        std::cout << "PLayer name is " << player_name;
        std::string copy = player_name;
        tcp_authenticate(copy);
        //lobby_started = true;
    }
    if (_event.type == sf::Event::MouseButtonPressed && ModedButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
        soundManager.playSound("click");
        has_start = false;
        in_skin = true;
    }

    if (_event.type == sf::Event::MouseButtonPressed && _setting_button.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
        soundManager.playSound("click");
        settings();
        //_setting_button.setFillColor(sf::Color::Red);
    }

    if (_event.type == sf::Event::MouseButtonPressed && _quit_text.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
        soundManager.playSound("click");
        //_quit_text.setFillColor(sf::Color::Red);
        send_close_message();
        _window.close();
    }

    if (_event.type == sf::Event::MouseButtonPressed && _replay_text.getGlobalBounds().contains(sf::Vector2f(mousePos))) {
        if (record) {
            record = false;
            _replay_text.setFillColor(sf::Color::Red);
            soundManager.playSound("click");
        } else if (!record) {
            record = true;
            _replay_text.setFillColor(sf::Color::Green);
            soundManager.playSound("click");
        }
    }
}

void Client::Display_next_level()
{
    if (level_passed == 1) {
        _window.draw(Level_passed_text);
        _window.draw(Timer_text);
        _window.draw(Level_info_text);
    }
}

void Client::game_update(std::shared_ptr<RenderSystem>& _render_system)
{
    receive_tcp_message(_medi);
    InputManagement();
    paralax_scroll();
    _window.clear(sf::Color::Black);
    _window.draw(backgroundLayer1);
    _window.draw(backgroundLayer2);
    _window.draw(level_time_text);
    _window.draw(chargeBarBackground);
    _window.draw(chargeBar);

    if (Boss_ref == -1) {
        _window.draw(Level_text);
    }

    float deltaTime = clock.restart().asSeconds();
    if (game_finish != 1) {
        _render_system->Render(_window, _medi);
        _animation_system->update(_medi, deltaTime);
    }

    if (r_score == 1) {
        _window.draw(_score);
    }

    Set_health_bar();
    display_collison();
    display_warning();
    display_boss_health_bar();
    Display_next_level();

    if (record) {
        start_recording();
        record = false;
    }
    if (is_recording) {
        record_session();
    }

    if (game_finish == 1) {
        display_scoreboard();
        updateLobbyAnimation();
    }
}

void Client::update()
{
    if (has_start) {
        updateLobbyAnimation();
        _window.draw(animatedLobbySprite);
        _window.draw(game_name);
        _window.draw(_start_text);
        _window.draw(ModedButton);
        _window.draw(_setting_button);
        _window.draw(_quit_text);
        _window.draw(_replay_text);
        soundManager.playMenuMusic();
    }

    if (lobby_started) {
        _window.draw(lobby_sprite);
        _window.draw(lobby_text);
        _window.draw(_start_button);
        soundManager.playLobbyMusic();
    }

    if (in_skin) {
        _window.clear();
        _manager.draw(_window);
        _window.draw(_back_text);
        //skinSelector.draw(_window);
    }
}

void Client::paralax_scroll() 
{
    backgroundLayer1.move(-parallaxSpeed, 0.f);
    backgroundLayer2.move(-parallaxSpeed, 0.f);

    if (backgroundLayer1.getPosition().x + backgroundLayer1.getGlobalBounds().width <= 0) {
        backgroundLayer1.setPosition(backgroundLayer2.getPosition().x + backgroundLayer2.getGlobalBounds().width, 0.f);
    }

    if (backgroundLayer2.getPosition().x + backgroundLayer2.getGlobalBounds().width <= 0) {
        backgroundLayer2.setPosition(backgroundLayer1.getPosition().x + backgroundLayer1.getGlobalBounds().width, 0.f);
    }
}

void Client::display_scoreboard()
{
    _window.draw(animatedLobbySprite);

    for (const auto& text : scoreboard_text) {
        _window.draw(text);
    }
    _window.draw(credits_text);
}

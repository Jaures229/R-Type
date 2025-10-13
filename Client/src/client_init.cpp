#include "../include/client.hpp"


void Client::Init(Mediator& medi)
{
    // init all the ressource for the game
    connect_socket();
    _window.create(sf::VideoMode(1920, 1080), "R-TYPE Client");
    _selector.add(tcp_socket);    

    _name = "PLAYER";

    if (!_global_font.loadFromFile("Client/ressources/Roboto-Black.ttf")) {
        std::cerr << "Error loading font" << std::endl;
    }
    if (!lobby_texture.loadFromFile("Client/ressources/lobby_image.jpg")) {
        std::cerr << "Error loading texture" << std::endl;
    }
    if (!heart_texture.loadFromFile("Client/ressources/heart.png")) {
        std::cerr << "Error loading texture" << std::endl;
    }

    for (int i = 0; i <= 23; ++i) {
        sf::Texture texture;
        std::string framePath = "Client/ressources/Ressources_Background/frame" + std::to_string(i) + ".jpg";
        if (!texture.loadFromFile(framePath)) {
            std::cerr << "Erreur de chargement de la frame: " << framePath << std::endl;
            continue;
        }
        lobbyFrames.push_back(texture);
    }

    simple_explosion.resize(27);
    simple_explosion_texture.reserve(27); // Réserve de l'espace pour éviter les copies inattendues

    for (int i = 0; i < 27; ++i) {
        auto texture = std::make_shared<sf::Texture>();
        std::string framePath = "Client/ressources/Explosion/vnbvq_" + std::to_string(i) + ".png";

        if (!texture->loadFromFile(framePath)) {
            std::cerr << "Erreur de chargement de la frame: " << framePath << std::endl;
            continue;
        }

        simple_explosion_texture.push_back(texture);
        simple_explosion[i].setTexture(*texture); // Utilise la texture chargée
        simple_explosion[i].setScale(0.5, 0.5);
    }

    boss_warning_texture.reserve(19);
    warning_animation.resize(19);

    for (int i = 0; i < 19; ++i) {
        auto texture = std::make_shared<sf::Texture>();
        std::string framePath = "Client/ressources/Warning/frame_" + std::to_string(i) + "_delay-0.05s.png";

        if (!texture->loadFromFile(framePath)) {
            std::cerr << "Erreur de chargement de la frame: " << framePath << std::endl;
            continue;
        }

        boss_warning_texture.push_back(texture);
        warning_animation[i].setTexture(*texture);
        warning_animation[i].setScale(0.5, 0.5);
    }

    if (!lobbyFrames.empty()) {
        animatedLobbySprite.setTexture(lobbyFrames[0]); // Initialise avec la première frame
        animatedLobbySprite.setScale(1, 1);
        animatedLobbySprite.setPosition(0, 0);
    }
    if (!_game_font.loadFromFile("Client/ressources/INVASION2000.TTF")) {
        std::cerr << "Error loading font" << std::endl;
    }

    if (!_test_font.loadFromFile("Client/ressources/Game-Paused.otf")) {
        std::cerr << "Error loading font" << std::endl;
    }
    parallaxSpeed = 0.9f;

    sf::Vector2u windowSize = _window.getSize();
    lobby_sprite.setTexture(lobby_texture);
    lobby_sprite.setScale(1, 1);
    lobby_text.setFont(_global_font);
    lobby_text.setString("LOBBY");
    lobby_text.setCharacterSize(30);
    lobby_text.setFillColor(sf::Color::White);
    lobby_text.setPosition(windowSize.x / 2.0f, 0);

    _replay_header.setFont(_game_font);
    _replay_header.setString("REPLAYS");
    _replay_header.setCharacterSize(60);
    _replay_header.setFillColor(sf::Color::White);
    _replay_header.setPosition(windowSize.x / 2.0f, 0);


    _start_button.setFont(_game_font);
    _start_button.setString("START");
    _start_button.setCharacterSize(40);
    _start_button.setFillColor(sf::Color::White);
    _start_button.setPosition(windowSize.x / 2.0f + 500, 0);

    back_text.setFont(_game_font);
    back_text.setString("<Back>");
    back_text.setCharacterSize(40);
    back_text.setFillColor(sf::Color::White);
    back_text.setPosition(windowSize.x / 2.0f - 500, 0);



    _start_text.setFont(_game_font);
    _start_text.setString("START");
    _start_text.setCharacterSize(50);
    _start_text.setFillColor(sf::Color::White);
    float centerX = windowSize.x / 2.0f;
    float centerY = windowSize.y / 2.0f;
    _start_text.setPosition(centerX - 100, centerY - 50);

    ModedButton.setFont(_game_font);
    ModedButton.setString("CHOOSE SKIN");
    ModedButton.setCharacterSize(50);
    ModedButton.setFillColor(sf::Color::White);
    ModedButton.setPosition(centerX - 200, centerY - 200);

    solo_play_text.setFont(_game_font);
    solo_play_text.setString("SOLO PLAY");
    solo_play_text.setCharacterSize(50);
    solo_play_text.setFillColor(sf::Color::White);
    solo_play_text.setPosition(centerX - 200, centerY - 100);

    _text_status.setFont(_game_font);
    _text_status.setString("START");
    _text_status.setCharacterSize(30);
    _text_status.setFillColor(sf::Color::Red);
    _text_status.setPosition(centerX, centerY);

    menu_sprite.setTexture(menu_texture);
    menu_sprite.setScale(1, 1);
    menu_sprite.setPosition(0, 0);

    heart_sprite.setTexture(heart_texture);
    heart_sprite.setScale(0.1, 0.1);
    //heart_sprite.setPosition(0, 0);

    _quit_text.setFont(_game_font);
    //_quit_text.setScale(1, 1);
    _quit_text.setString("EXIT");
    _quit_text.setCharacterSize(50);
    _quit_text.setFillColor(sf::Color::White);
    _quit_text.setPosition(centerX - 80, centerY + 100);

    _replay_text.setFont(_game_font);
    _replay_text.setString("REPLAY");
    _replay_text.setCharacterSize(50);
    _replay_text.setFillColor(sf::Color::White);
    _replay_text.setPosition(centerX - 100, centerY + 175);

    game_name.setFont(_game_font);
    game_name.setString("R-TYPE");
    game_name.setCharacterSize(100);
    game_name.setFillColor(sf::Color::White);
    game_name.setPosition(centerX - 150, 0);

    _setting_button.setFont(_game_font);
    _setting_button.setString("SETTINGS");
    _setting_button.setCharacterSize(50);
    _setting_button.setFillColor(sf::Color::White);
    _setting_button.setPosition(centerX - 100, centerY + 25);

    _score.setFont(_game_font);
    _score.setString("SCORE: " + std::to_string(score));
    _score.setCharacterSize(30);
    _score.setFillColor(sf::Color::White);
    _score.setPosition(1600, 10);

    _back_text.setFont(_game_font);
    _back_text.setString("BACK");
    _back_text.setCharacterSize(30);
    _back_text.setFillColor(sf::Color::White);
    _back_text.setPosition(
    (_window.getSize().x -  back_text.getLocalBounds().width - 50),
    (_window.getSize().y - back_text.getLocalBounds().height- 50));
    
    level_time_text.setFont(_game_font);
    level_time_text.setString("TIME: " + lexical_cast<std::string>(level_time));
    level_time_text.setCharacterSize(30);
    level_time_text.setFillColor(sf::Color::White);
    level_time_text.setPosition(0, 10);

    // Initialisation des propriétés de la barre
    chargeBarBackground.setSize(sf::Vector2f(chargeBarWidth, chargeBarHeight));
    chargeBarBackground.setFillColor(sf::Color(50, 50, 50)); // Couleur de fond (gris foncé)
    chargeBarBackground.setPosition(centerX - 300, _window.getSize().y - 50); // Position en bas de l'écran

    chargeBar.setSize(sf::Vector2f(0, chargeBarHeight)); // Initialement vide
    chargeBar.setFillColor(sf::Color(100, 200, 100));    // Couleur verte
    chargeBar.setPosition(chargeBarBackground.getPosition());

    Level_passed_text.setFont(_game_font);
    Level_passed_text.setString("[LEVEL PASSED GG]");
    Level_passed_text.setCharacterSize(50);
    Level_passed_text.setFillColor(sf::Color::White);

    sf::FloatRect level_textBounds = Level_passed_text.getLocalBounds();
    float level_centerX = (_window.getSize().x - level_textBounds.width) / 2.0f - level_textBounds.left;
    float level_centerY = (_window.getSize().y - level_textBounds.height) / 2.0f - level_textBounds.top;

    Level_passed_text.setPosition(level_centerX, level_centerY);

    Level_failed_text.setFont(_game_font);
    Level_failed_text.setString("LEVEL FAILED Lock in Boy >:");
    Level_failed_text.setCharacterSize(50);
    Level_failed_text.setFillColor(sf::Color::White);
    Level_failed_text.setPosition(centerX, centerY);

    Timer_text.setFont(_game_font);
    Timer_text.setString("TIME BEFORE NEXT LEVEL:");
    Timer_text.setCharacterSize(50);
    Timer_text.setFillColor(sf::Color::White);
    Timer_text.setPosition(level_centerX, level_centerY + 100);
    
    Level_info_text.setFont(_game_font);
    Level_info_text.setString("TEXT");
    Level_info_text.setCharacterSize(50);
    Level_info_text.setFillColor(sf::Color::White);
    Level_info_text.setPosition(level_centerX, level_centerY + 200);
    
    Level_text.setFont(_test_font);
    Level_text.setString(current_level);
    Level_text.setCharacterSize(50);
    Level_text.setFillColor(sf::Color::White);

    float padding = 20.0f;

    Level_text.setPosition((1920 - Level_text.getGlobalBounds().width) / 2,  // X : centré horizontalement
    padding  // Y : padding depuis le haut
    );

    credits_text.setFont(_game_font);
    std::string credits = "THANKS FOR PLAYING THE GAME :)\nMADE BY THE 3X DEVELOPPER AGOSSOU JAURES :)\n SCHALLUM SETONDJI\nHARRY FATOLOU\nCHARDONE AMOKI\nJORES DA-TRINIDADE\n";
    credits_text.setString(credits);
    credits_text.setFillColor(sf::Color::White);
    credits_text.setCharacterSize(20);
    credits_text.setPosition(centerX, centerY + 300);
}

void Client::settings()
{
    optionsWindow.create(sf::VideoMode(450, 400), "Options - Remap Keys");
    optionsWindow.setPosition({460, 540});
    instructions = sf::Text("Cliquez sur un bouton et appuyez sur la nouvelle touche", _global_font, 15);
    instructions.setPosition(20, 10);

    upButton = sf::Text("UP", _global_font, 20);
    upButton.setPosition(50, 50);
    downButton = sf::Text("DOWN", _global_font, 20);
    downButton.setPosition(50, 100);
    leftButton = sf::Text("LEFT", _global_font, 20);
    leftButton.setPosition(50, 150);
    rightButton = sf::Text("RIGHT", _global_font, 20);
    rightButton.setPosition(50, 200);
    shootButton = sf::Text("SHOOT", _global_font, 20);
    shootButton.setPosition(50, 250);
    saveButton = sf::Text("SAVE", _global_font, 20);
    saveButton.setPosition(225, 300);

    while (optionsWindow.isOpen()) {
        sf::Event event;
        while (optionsWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                optionsWindow.close();
            }

            sf::Vector2i mousePos = sf::Mouse::getPosition(optionsWindow);
            if (upButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                upButton.setFillColor(sf::Color::Red);
            } 
            else if (downButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                downButton.setFillColor(sf::Color::Red);
            } 
            else if (leftButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                leftButton.setFillColor(sf::Color::Red);                    
            } 
            else if (rightButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                rightButton.setFillColor(sf::Color::Red);
            } 
            else if (shootButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                shootButton.setFillColor(sf::Color::Red);
            } 
            else if (saveButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                saveButton.setFillColor(sf::Color::Red);
            } 
            else {
                defaultColorSettingButton();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (upButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    upButton.setFillColor(sf::Color::Green);
                    //std::cout << "Up button clicked \n";
                    isKeyUp = true;             
                } else if (downButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    downButton.setFillColor(sf::Color::Green);
                    //std::cout << "Down button clicked \n";
                    isKeyDown = true;                   
                } else if (leftButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    leftButton.setFillColor(sf::Color::Green);
                    //std::cout << "Left button clicked \n";
                    isKeyLeft = true;                   
                } else if (rightButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    rightButton.setFillColor(sf::Color::Green);
                    //std::cout << "Right button clicked \n";
                    isKeyRight = true;                   
                }
                else if (shootButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    shootButton.setFillColor(sf::Color::Green);
                    //std::cout << "Shoot button clicked \n";
                    isKeyShoot = true;                   
                }
                else if (saveButton.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    saveButton.setFillColor(sf::Color::Green);
                    //std::cout << "Save button clicked \n";
                    saveKeyBindingsToFile();
                    optionsWindow.close();                   
                }
                else {
                    defaultColorSettingButton();
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (isKeyUp) {
                    keyMapDefault["Up"] = event.key.code;
                    std::cout << "Nouvelle touche assignée \n";
                    upButtonAssigned = sf::Text(keyAssigned(keyMapDefault["Up"]), _global_font, 20);
                    upButtonAssigned.setPosition(250, 50);
                    isKeyUp = false;
                }
                if (isKeyDown) {
                    keyMapDefault["Down"] = event.key.code;
                    std::cout << "Nouvelle touche assignée \n";
                    downButtonAssigned = sf::Text(keyAssigned(keyMapDefault["Down"]), _global_font, 20);
                    downButtonAssigned.setPosition(250, 100);
                    isKeyDown = false;
                }
                if (isKeyLeft) {
                    keyMapDefault["Left"] = event.key.code;
                    std::cout << "Nouvelle touche assignée \n";
                    leftButtonAssigned = sf::Text(keyAssigned(keyMapDefault["Left"]), _global_font, 20);
                    leftButtonAssigned.setPosition(250, 150);
                    isKeyLeft = false;
                }
                if (isKeyRight) {
                    keyMapDefault["Right"] = event.key.code;
                    std::cout << "Nouvelle touche assignée \n";
                    rightButtonAssigned = sf::Text(keyAssigned(keyMapDefault["Right"]), _global_font, 20);
                    rightButtonAssigned.setPosition(250, 200);
                    isKeyRight = false;
                }
                if (isKeyShoot) {
                    keyMapDefault["Shoot"] = event.key.code;
                    std::cout << "Nouvelle touche assignée \n";
                    shootButtonAssigned = sf::Text(keyAssigned(keyMapDefault["Shoot"]), _global_font, 20);
                    shootButtonAssigned.setPosition(250, 250);
                    isKeyRight = false;
                }
            }
        }
        optionsWindow.clear();
        optionsWindow.draw(instructions);
        optionsWindow.draw(upButton);
        optionsWindow.draw(downButton);
        optionsWindow.draw(leftButton);
        optionsWindow.draw(rightButton);
        optionsWindow.draw(shootButton);

        optionsWindow.draw(upButtonAssigned);
        optionsWindow.draw(downButtonAssigned);
        optionsWindow.draw(leftButtonAssigned);
        optionsWindow.draw(rightButtonAssigned);
        optionsWindow.draw(shootButtonAssigned);

        optionsWindow.draw(saveButton);
        optionsWindow.display();
    }
}

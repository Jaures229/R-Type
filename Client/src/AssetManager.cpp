/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** AssetManager
*/

#include "../include/AssetManager.hpp"

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{

}

void AssetManager::Init()
{
    // Load all assets
    std::ifstream json_file("Config/client_config.json");

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

    // load the sprite for the players based on the path in the configuration file

     _players_sprites.resize(4);

    _players_sprites[0].height = 10;
    _players_sprites[0].path = json_data["players"]["player1"].as<std::string>();
    if (!_players_sprites[0].texture.loadFromFile(_players_sprites[0].path)) {
        std::cerr << "Error loading sprite : " << _players_sprites[0].path << "\n";
    }
    _players_sprites[0].sprite.setTexture(_players_sprites[0].texture);
    _players_sprites[0].sprite.scale(0.5, 0.5);

    _players_sprites[1].height = 10;
    _players_sprites[1].path = json_data["players"]["player2"].as<std::string>();
    if (!_players_sprites[1].texture.loadFromFile(_players_sprites[1].path)) {
        std::cerr << "Error loading sprite : " << _players_sprites[1].path << "\n";
    }
    _players_sprites[1].sprite.setTexture(_players_sprites[1].texture);
    _players_sprites[1].sprite.scale(0.5, 0.5);

    _players_sprites[2].height = 10;
    _players_sprites[2].path = json_data["players"]["player3"].as<std::string>();
    if (!_players_sprites[2].texture.loadFromFile(_players_sprites[2].path)) {
        std::cerr << "Error loading sprite : " << _players_sprites[2].path << "\n";
    }
    _players_sprites[2].sprite.setTexture(_players_sprites[2].texture);
    _players_sprites[2].sprite.scale(0.5, 0.5);

    _players_sprites[3].height = 10;
    _players_sprites[3].path = json_data["players"]["player4"].as<std::string>();
    if (!_players_sprites[3].texture.loadFromFile(_players_sprites[3].path)) {
        std::cerr << "Error loading sprite : " << _players_sprites[3].path << "\n";
    }
    _players_sprites[3].sprite.setTexture(_players_sprites[3].texture);
    _players_sprites[3].sprite.scale(0.5, 0.5);

    _projectiles_sprites.height = 10;
    _projectiles_sprites.path = json_data["players"]["bullet"].as<std::string>();
    if (!_projectiles_sprites.texture.loadFromFile(_projectiles_sprites.path)){
        std::cerr << "Error loading sprite : " << _projectiles_sprites.path << "\n";
    }
    _projectiles_sprites.sprite.setTexture(_projectiles_sprites.texture);
    _projectiles_sprites.sprite.setScale(0.5, 0.5);


    // load all asset of the enemys field in the set

    if (json_data.contains("enemys")) {
        const json& enemies = json_data["enemys"];
            // Parcourir les champs de "enemies"
            for (auto& kv : enemies.object_range()) {
                const std::string& enemyType = kv.key();

                json enemy = kv.value();

                std::string path = enemy["path"].as<std::string>();
                int width = enemy["width"].as<int>();
                int height = enemy["height"].as<int>();
                float factorX = enemy["factorY"].as<float>();
                float factorY = enemy["factorY"].as<float>();
                std::cout << "Loading asset: " << enemyType << "\n";
                // Afficher les informations de l'ennemi
                std::cout << "  Path: " << path << std::endl;
                std::cout << "  Width: " << width << std::endl;
                std::cout << "  Height: " << height << std::endl;

                _sprites[enemyType].height = height;
                _sprites[enemyType].width = width;
                _sprites[enemyType].path = path;
                if (!_sprites[enemyType].texture.loadFromFile(path)) {
                    std::cerr << "Error loading sprite :" << _sprites[enemyType].path << "\n";
                }
                _sprites[enemyType].sprite.setTexture(_sprites[enemyType].texture);
                _sprites[enemyType].sprite.setScale(factorX, factorY);

                // // Parcourir les animations de l'ennemi
                if (enemy.contains("Animations")) {
                    const json& animations = enemy["Animations"];

                    for (auto& anim_kv : animations.object_range()) {
                        const std::string& animName = anim_kv.key();
                        const json& anim = anim_kv.value();

                        _animations[enemyType].animations[animName].startFrame = sf::IntRect(
                            anim["StartFrame_left"].as<int>(),
                            anim["StartFrame_top"].as<int>(),
                            anim["StartFrame_width"].as<int>(),
                            anim["StartFrame_height"].as<int>());
                        
                        _animations[enemyType].animations[animName].frameSize = sf::Vector2i(
                            anim["FrameSize_x"].as<int>(),
                            anim["FrameSize_y"].as<int>());

                        _animations[enemyType].animations[animName].frameCount = anim["frameCount"].as<int>();
                        _animations[enemyType].animations[animName].frameDuration = anim["frameDuration"].as<float>();
                        _animations[enemyType].animations[animName].looping = true;
                        _animations[enemyType].animations[animName].frameOffset = sf::Vector2i(
                            anim["offset_x"].as<int>(),
                            anim["offset_y"].as<int>());
                    }
                } else {
                    std::cerr << "No animations found for enemy type: " << enemyType << std::endl;
                }
            }
    } else {
        std::cerr << "Clé 'enemies' non trouvée dans le JSON." << std::endl;
    }

    if (json_data.contains("game")) {
        const json& game = json_data["game"];

        _sprites["top_wall"].height = 10;
        _sprites["top_wall"].width = 10;
        _sprites["top_wall"].path = game["top_wall"].as<std::string>();

        if (!_sprites["top_wall"].texture.loadFromFile(_sprites["top_wall"].path)) {
            std::cerr << "Error loading sprite :" << _sprites["top_wall"].path << "\n";
        }

        _sprites["top_wall"].sprite.setTexture(_sprites["top_wall"].texture);
        _sprites["top_wall"].sprite.setScale(1, 1);

        _sprites["back_wall"].height = 10;
        _sprites["back_wall"].width = 10;
        _sprites["back_wall"].path = game["back_wall"].as<std::string>();

        if (!_sprites["back_wall"].texture.loadFromFile(_sprites["back_wall"].path)) {
            std::cerr << "Error loading sprite :" << _sprites["back_wall"].path << "\n";
        }

        _sprites["back_wall"].sprite.setTexture(_sprites["back_wall"].texture);
        _sprites["back_wall"].sprite.setScale(1, 1);
        // Load other assets for the game.
    }

    // load all the power ups assets
    if (json_data.contains("power_ups")) {
        const json& enemies = json_data["power_ups"];
            // Parcourir les champs de "enemies"
            for (auto& kv : enemies.object_range()) {
                const std::string& enemyType = kv.key();

                json enemy = kv.value();

                std::string path = enemy["path"].as<std::string>();
                int width = enemy["width"].as<int>();
                int height = enemy["height"].as<int>();
                float factorX = enemy["factorY"].as<float>();
                float factorY = enemy["factorY"].as<float>();
                std::cout << "Loading asset: " << enemyType << "\n";
                // Afficher les informations de l'ennemi
                std::cout << "  Path: " << path << std::endl;
                std::cout << "  Width: " << width << std::endl;
                std::cout << "  Height: " << height << std::endl;

                _sprites[enemyType].height = height;
                _sprites[enemyType].width = width;
                _sprites[enemyType].path = path;
                if (!_sprites[enemyType].texture.loadFromFile(path)) {
                    std::cerr << "Error loading sprite :" << _sprites[enemyType].path << "\n";
                }
                _sprites[enemyType].sprite.setTexture(_sprites[enemyType].texture);
                _sprites[enemyType].sprite.setScale(factorX, factorY);
            }
    } else {
        std::cerr << "Clé 'power_ups' non trouvée dans le JSON." << std::endl;
    }

    const auto& players = json_data["players"];
    int width = 100;
    int height = 100;

    // Boucle pour afficher chaque clé-valeur
    for (const auto& item : players.object_range()) {
        std::string key = item.key();
        std::string value = item.value().as<std::string>();

        _sprites[key].height = height;
        _sprites[key].width = width;
        _sprites[key].path =  value;

        if (!_sprites[key].texture.loadFromFile(value)) {
            std::cerr << "Failed to load texture for: " << key << std::endl;
        }
        _sprites[key].sprite.setTexture(_sprites[key].texture);
        _sprites[key].sprite.setScale(0.5, 0.5);
        std::cout << key << ": " << value << '\n';
    }

    //_backgrounds_sprites;

    const auto& backgrounds = json_data["background_list"];

    // Boucle pour afficher chaque clé-valeur
    for (const auto& item : backgrounds.object_range()) {
        std::string key = item.key();
        std::string value = item.value().as<std::string>();
        _backgrounds_sprites[key].height = height;
        _backgrounds_sprites[key].width = width;
        _backgrounds_sprites[key].path =  value;
        if (!_backgrounds_sprites[key].texture.loadFromFile(value)) {
            std::cerr << "Failed to load texture for: " << key << std::endl;
        }
        _backgrounds_sprites[key].sprite.setTexture(_backgrounds_sprites[key].texture);
        _backgrounds_sprites[key].sprite.setScale(0.5, 0.5);
        std::cout << key << ": " << value << '\n';
    }

    if (json_data.contains("Enemy_bullets")) {
        const json& enemies = json_data["Enemy_bullets"];
            // Parcourir les champs de "enemies"
            for (auto& kv : enemies.object_range()) {
                const std::string& enemyType = kv.key();

                json enemy = kv.value();

                std::string path = enemy["path"].as<std::string>();
                int width = enemy["width"].as<int>();
                int height = enemy["height"].as<int>();
                float factorX = enemy["factorY"].as<float>();
                float factorY = enemy["factorY"].as<float>();
                std::cout << "Loading asset: " << enemyType << "\n";
                // Afficher les informations de l'ennemi
                std::cout << "  Path: " << path << std::endl;
                std::cout << "  Width: " << width << std::endl;
                std::cout << "  Height: " << height << std::endl;

                _sprites[enemyType].height = height;
                _sprites[enemyType].width = width;
                _sprites[enemyType].path = path;
                if (!_sprites[enemyType].texture.loadFromFile(path)) {
                    std::cerr << "Error loading sprite :" << _sprites[enemyType].path << "\n";
                }
                _sprites[enemyType].sprite.setTexture(_sprites[enemyType].texture);
                _sprites[enemyType].sprite.setScale(factorX, factorY);

                if (enemy.contains("Animations")) {
                    const json& animations = enemy["Animations"];

                    for (auto& anim_kv : animations.object_range()) {
                        const std::string& animName = anim_kv.key();
                        const json& anim = anim_kv.value();

                        _animations[enemyType].animations[animName].startFrame = sf::IntRect(
                            anim["StartFrame_left"].as<int>(),
                            anim["StartFrame_top"].as<int>(),
                            anim["StartFrame_width"].as<int>(),
                            anim["StartFrame_height"].as<int>());
                        
                        _animations[enemyType].animations[animName].frameSize = sf::Vector2i(
                            anim["FrameSize_x"].as<int>(),
                            anim["FrameSize_y"].as<int>());

                        _animations[enemyType].animations[animName].frameCount = anim["frameCount"].as<int>();
                        _animations[enemyType].animations[animName].frameDuration = anim["frameDuration"].as<float>();
                        _animations[enemyType].animations[animName].looping = true;
                        _animations[enemyType].animations[animName].frameOffset = sf::Vector2i(
                            anim["offset_x"].as<int>(),
                            anim["offset_y"].as<int>());
                    }
                } else {
                    std::cerr << "No animations found for enemy type: " << enemyType << std::endl;
                }
            }
    } else {
        std::cerr << "Clé 'power_ups' non trouvée dans le JSON." << std::endl;
    }

    if (json_data.contains("skins")) {
        const json& enemies = json_data["skins"];
        for (const auto& item : enemies.object_range()) {
            std::string key = item.key();
            std::string value = item.value().as<std::string>();

            _skins[key].path = value;
            if(!_skins[key].texture.loadFromFile(value)) {
                std::cerr << "Failed to load texture for: " << key << std::endl;

            }
            _skins[key].height = 32;
            _skins[key].width = 32;
            _skins[key].sprite.setTexture(_skins[key].texture);
            _skins[key].sprite.setScale(1, 1);
            std::cout << key << ": " << value << '\n';
        }        
    }

    Back_texture = new sf::Texture();
    button_texture = new sf::Texture();

    if (!Back_texture->loadFromFile("Client/ressources/Card.png")) {
        std::cerr << "Failed to load background texture" << std::endl;
    }
    // //Back.setTexture(Back_texture);
    if (!button_texture->loadFromFile("Client/ressources/Button_Active.png")) {
        std::cerr << "Failed to load background texture" << std::endl;
    }
    //button_Back.setTexture(button_texture);
}

std::vector<Sprite>& AssetManager::getPlayersSprites(void)
{
    return _players_sprites;
}

Sprite& AssetManager::getMobsSprites(void)
{
    return _mobs_sprites;
} 

Sprite & AssetManager::getBoss1Sptrite(void)
{
    return _boss1_sprites;
}

Sprite & AssetManager::getBoss2Sptrite(void)
{
    return _boss2_sprites;
}

Sprite & AssetManager::getBulletSptrite(void)
{
    return _projectiles_sprites;
}

std::unordered_map<std::string, Sprite>& AssetManager::getSpriteMap(void)
{
    return _sprites;
}

std::unordered_map<std::string, AnimationComponent>& AssetManager::getAnimationMap(void)
{
    return _animations;
}

void AssetManager::draw(sf::RenderWindow& window)
{
        // 1. Fond spécifique pour la section
        sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
        background.setTexture(Back_texture);
        window.draw(background);

        // 2. Titre de la section
        sf::Font font;
        if (!font.loadFromFile("Client/ressources/INVASION2000.TTF")) {
            return;
        }

        sf::Text title("Choisissez votre skin", font, 40);
        title.setFillColor(sf::Color::White);
        title.setPosition(window.getSize().x / 2 - title.getGlobalBounds().width / 2, 20);
        window.draw(title);

        // 3. Dessiner le carré contenant la grille de skins
        sf::RectangleShape skinBox(sf::Vector2f(400, 400));
        skinBox.setTexture(Back_texture);
        skinBox.setPosition(50, 100);
        window.draw(skinBox);

        // 4. Grille des skins à l'intérieur du carré
        float x = skinBox.getPosition().x + 20.0f;
        float y = skinBox.getPosition().y + 20.0f;
        int column = 0;
        const float padding = 10.0f;

        // Calculer l'index de début et de fin pour la page actuelle
        int startIndex = _currentPage * SKINS_PER_PAGE;
        int currentIndex = 0;
        int skinsOnCurrentPage = 0;

        for (auto& [name, sprite] : _skins) {
            if (currentIndex < startIndex) {
                currentIndex++;
                continue;
            }

            if (skinsOnCurrentPage >= SKINS_PER_PAGE) {
                break;
            }

            sprite.sprite.setPosition(x, y);
            window.draw(sprite.sprite);

            // Surbrillance si survolé ou sélectionné
            if (name == _selectedSkin) {
                sf::RectangleShape highlight(sf::Vector2f(sprite.sprite.getGlobalBounds().width, sprite.sprite.getGlobalBounds().height));
                highlight.setPosition(x, y);
                highlight.setFillColor(sf::Color::Transparent);
                highlight.setOutlineColor(sf::Color::Yellow);
                highlight.setOutlineThickness(5.0f);
                window.draw(highlight);
            }

            x += sprite.sprite.getGlobalBounds().width + padding;
            column++;
            if (column == 4) {
                column = 0;
                x = skinBox.getPosition().x + 20.0f;
                y += sprite.sprite.getGlobalBounds().height + padding;
            }

            skinsOnCurrentPage++;
            currentIndex++;
        }

        // 5. Boutons de navigation
        // Bouton "Précédent"
        if (_currentPage > 0) {
            sf::RectangleShape prevButton(sf::Vector2f(100, 40));
            prevButton.setTexture(button_texture);
            prevButton.setPosition(50, window.getSize().y - 160);
            window.draw(prevButton);

            sf::Text prevText("< Prev", font, 20);
            prevText.setFillColor(sf::Color::White);
            prevText.setPosition(prevButton.getPosition().x + prevButton.getSize().x / 2 - prevText.getGlobalBounds().width / 2,
                               prevButton.getPosition().y + prevButton.getSize().y / 2 - prevText.getGlobalBounds().height / 2);
            window.draw(prevText);
        }

        // Bouton "Suivant"
        if (_currentPage < getTotalPages() - 1) {
            sf::RectangleShape nextButton(sf::Vector2f(100, 40));
            nextButton.setTexture(button_texture);
            //nextButton.setFillColor(sf::Color(100, 100, 100));
            nextButton.setPosition(350, window.getSize().y - 160);
            window.draw(nextButton);

            sf::Text nextText("Next >", font, 20);
            nextText.setFillColor(sf::Color::White);
            nextText.setPosition(nextButton.getPosition().x + nextButton.getSize().x / 2 - nextText.getGlobalBounds().width / 2,
                               nextButton.getPosition().y + nextButton.getSize().y / 2 - nextText.getGlobalBounds().height / 2);
            window.draw(nextText);
        }

        // Indicateur de page
        sf::Text pageIndicator("Page " + std::to_string(_currentPage + 1) + "/" + std::to_string(getTotalPages()),
                             font, 20);
        pageIndicator.setFillColor(sf::Color::White);
        pageIndicator.setPosition(window.getSize().x / 2 - pageIndicator.getGlobalBounds().width / 2,
                                window.getSize().y - 160);
        window.draw(pageIndicator);

        // 6. Aperçu du skin sélectionné (reste inchangé)
        if (!_selectedSkin.empty()) {
            Sprite& selected = _skins[_selectedSkin];
            sf::Sprite preview = selected.sprite;
            preview.setScale(3.0f, 3.0f);
            preview.setPosition(window.getSize().x - 400, window.getSize().y / 2 - preview.getGlobalBounds().height / 2);
            window.draw(preview);

            sf::Text skinName(_selectedSkin, font, 30);
            skinName.setFillColor(sf::Color::White);
            skinName.setPosition(window.getSize().x - 400, window.getSize().y / 2 + preview.getGlobalBounds().height / 2 + 20);
            window.draw(skinName);
        }

        // 7. Bouton "Confirmer"
        sf::RectangleShape confirmButton(sf::Vector2f(200, 60));
        confirmButton.setFillColor(sf::Color(50, 150, 50));
        confirmButton.setPosition(window.getSize().x / 2 - confirmButton.getSize().x / 2, window.getSize().y - 100);
        confirmButton.setTexture(Back_texture);
        window.draw(confirmButton);

        sf::Text buttonText("Confirmer", font, 25);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(confirmButton.getPosition().x + confirmButton.getSize().x / 2 - buttonText.getGlobalBounds().width / 2,
                             confirmButton.getPosition().y + confirmButton.getSize().y / 2 - buttonText.getGlobalBounds().height / 2);
        window.draw(buttonText);

        // 8. Message de confirmation
        if (_messageClock.getElapsedTime().asSeconds() < 3.0f && !_selectedSkin.empty()) {
            sf::Text confirmationMessage("Skin choisi : " + _selectedSkin, font, 30);
            confirmationMessage.setFillColor(sf::Color::Green);
            confirmationMessage.setPosition(window.getSize().x / 2 - confirmationMessage.getGlobalBounds().width / 2,
                                         window.getSize().y / 2);
            window.draw(confirmationMessage);
        }
}

void AssetManager::handleEvents(sf::RenderWindow& window, sf::Event& event)
{
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

            // Vérifier le clic sur les boutons de navigation
            sf::FloatRect prevButtonBounds(50, window.getSize().y - 160, 100, 40);
            sf::FloatRect nextButtonBounds(350, window.getSize().y - 160, 100, 40);

            if (prevButtonBounds.contains(mousePos) && _currentPage > 0) {
                _currentPage--;
                return;
            }

            if (nextButtonBounds.contains(mousePos) && _currentPage < getTotalPages() - 1) {
                _currentPage++;
                return;
            }

            // Vérifier les clics sur les skins de la page courante
            int currentIndex = 0;
            int startIndex = _currentPage * SKINS_PER_PAGE;
            int skinsOnCurrentPage = 0;

            for (auto& [name, sprite] : _skins) {
                if (currentIndex < startIndex) {
                    currentIndex++;
                    continue;
                }

                if (skinsOnCurrentPage >= SKINS_PER_PAGE) {
                    break;
                }

                if (sprite.sprite.getGlobalBounds().contains(mousePos)) {
                    _selectedSkin = name;
                    break;
                }

                skinsOnCurrentPage++;
                currentIndex++;
            }

            // Vérifier le clic sur le bouton "Confirmer"
            sf::FloatRect confirmButtonBounds(
                window.getSize().x / 2 - 100,
                window.getSize().y - 100,
                200,
                60
            );

            if (confirmButtonBounds.contains(mousePos) && !_selectedSkin.empty()) {
                _messageClock.restart();
                std::cout << "Skin sélectionné : " << _selectedSkin << std::endl;
            }
    }
}

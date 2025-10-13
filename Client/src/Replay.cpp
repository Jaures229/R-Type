/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** Replay
*/

#include "../include/Replay.hpp"

Replay::Replay(std::string replay_dir)
{
    _replay_dir = replay_dir;
}

Replay::~Replay()
{

}

void Replay::run()
{
    _window.create(sf::VideoMode(1920, 1080), "R-Type-Replay", sf::Style::Default);

    // Variables pour l'état de lecture
    bool isPaused = false; // Indique si la vidéo est en pause
    float frameRate = 30.0f; // Fréquence d'images (30 FPS)
    int frameStep = static_cast<int>(10.0f * frameRate); // Frames pour 10 secondes

    // Chargement de la police pour l'interface utilisateur
    sf::Font font;
    if (!font.loadFromFile("Client/ressources/INVASION2000.TTF")) {
        throw std::runtime_error("Failed to load font for UI");
    }

    // Texte pour l'état (Paused/Playing)
    sf::Text statusText;
    statusText.setFont(font);
    statusText.setCharacterSize(24);
    statusText.setFillColor(sf::Color::White);
    statusText.setPosition(10.f, 10.f);

    // Texte pour le temps actuel et total
    sf::Text timeText;
    timeText.setFont(font);
    timeText.setCharacterSize(20);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(10.f, 70.f);

    // Barre de progression
    sf::RectangleShape progressBarBackground(sf::Vector2f(800.f, 10.f));
    progressBarBackground.setFillColor(sf::Color(50, 50, 50)); // Fond gris foncé
    progressBarBackground.setPosition(10.f, 50.f);

    sf::RectangleShape progressBar(sf::Vector2f(0.f, 10.f));
    progressBar.setFillColor(sf::Color(100, 200, 100)); // Vert
    progressBar.setPosition(10.f, 50.f);

    // Calculer la durée totale de la vidéo en secondes
    float totalTimeInSeconds = static_cast<float>(_total_frames) / frameRate;

    while (_window.isOpen()) {
        while (_window.pollEvent(_event)) {
            if (_event.type == sf::Event::Closed) {
                _window.close();
            }

            if (_event.type == sf::Event::KeyPressed) {
                if (_event.key.code == sf::Keyboard::Space) {
                    isPaused = !isPaused; // Basculer pause/lecture
                } else if (_event.key.code == sf::Keyboard::Left) {
                    _current_frame = std::max(0, _current_frame - frameStep); // Reculer de 10 secondes
                    loadFramesInCache(_current_frame); // Mettre à jour le cache
                } else if (_event.key.code == sf::Keyboard::Right) {
                    _current_frame = std::min(_total_frames - 1, _current_frame + frameStep); // Avancer de 10 secondes
                    loadFramesInCache(_current_frame); // Mettre à jour le cache
                }
            }
        }

        if (!isPaused) { // Ne pas avancer les frames si en pause
            if (clock.getElapsedTime().asSeconds() >= 1.0f / frameRate) {
                _current_frame++;
                clock.restart();

                if (_current_frame >= _total_frames) {
                    _current_frame = 0; // Boucler à la première frame
                }
            }
        }

        // Charger les frames dynamiquement si nécessaire
        if (_frame_sprites.find(_current_frame) == _frame_sprites.end()) {
            loadFramesInCache(_current_frame);
        }

        // Mettre à jour l'état du texte
        statusText.setString(isPaused ? "Paused" : "Playing");

        // Mettre à jour la barre de progression
        float progress = static_cast<float>(_current_frame) / _total_frames;
        progressBar.setSize(sf::Vector2f(progress * 800.f, 10.f)); // 800 pixels de largeur max

        // Calculer le temps actuel
        float currentTimeInSeconds = static_cast<float>(_current_frame) / frameRate;

        // Formater le temps au format MM:SS
        int currentMinutes = static_cast<int>(currentTimeInSeconds) / 60;
        int currentSeconds = static_cast<int>(currentTimeInSeconds) % 60;
        int totalMinutes = static_cast<int>(totalTimeInSeconds) / 60;
        int totalSeconds = static_cast<int>(totalTimeInSeconds) % 60;

        // Mettre à jour le texte du temps
        timeText.setString(
            "Time: " +
            std::to_string(currentMinutes) + ":" + (currentSeconds < 10 ? "0" : "") + std::to_string(currentSeconds) +
            " / " +
            std::to_string(totalMinutes) + ":" + (totalSeconds < 10 ? "0" : "") + std::to_string(totalSeconds)
        );

        _window.clear();

        if (_frame_sprites.find(_current_frame) != _frame_sprites.end()) {
            _window.draw(_frame_sprites[_current_frame]);
        }

        _window.draw(statusText);
        _window.draw(progressBarBackground);
        _window.draw(progressBar);
        _window.draw(timeText);

        _window.display();
    }
}

void Replay::Init()
{
    if (!fs::exists(_replay_dir) || !fs::is_directory(_replay_dir)) {
        throw std::runtime_error("Invalid replay directory: " + _replay_dir);
    }

    // Compter le nombre total de frames disponibles
    for (const auto &entry : fs::directory_iterator(_replay_dir)) {
        const auto &path = entry.path();
        if (path.extension() == ".png" || path.extension() == ".jpg" || path.extension() == ".jpeg") {
            _total_frames++;
        }
    }

    if (_total_frames == 0) {
        throw std::runtime_error("No frames found in replay directory");
    }

    std::cout << "Found " << _total_frames << " frames in " << _replay_dir << std::endl;

    // Charger les premières frames dans le cache
    loadFramesInCache(0);
}

void Replay::loadFramesInCache(int start_frame)
{
    clearCache();

    for (int i = start_frame; i < start_frame + _cache_size && i < _total_frames; ++i) {
        std::string frame_name = _replay_dir + "/frame_" + std::to_string(i) + ".png";
        sf::Texture texture;
        if (texture.loadFromFile(frame_name)) {
            _frame_textures[i] = std::move(texture);
            sf::Sprite sprite;
            sprite.setTexture(_frame_textures[i]);
            _frame_sprites[i] = sprite;
        } else {
            std::cerr << "Failed to load frame: " << frame_name << std::endl;
        }
    }
}

void Replay::clearCache()
{
    _frame_textures.clear();
    _frame_sprites.clear();
}

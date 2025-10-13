/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** SoundManager
*/

#include "../include/SoundManager.hpp"

SoundManager::SoundManager()
{
    // Charger les sons et musiques à partir du fichier JSON
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
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    if (json_data.contains("sounds")) {
        const json& sounds = json_data["sounds"];
        for (auto& kv : sounds.object_range()) {
            std::string key = kv.key();
            std::string value = kv.value().as<std::string>();
            loadSound(key, value);
        }
    }

    if (json_data.contains("music")) {
        const json& music = json_data["music"];

        if (music.contains("menu") && music["menu"].is_string()) {
            menuMusicPath = music["menu"].as<std::string>();
        }

        if (music.contains("lobby") && music["lobby"].is_string()) {
            lobbyMusicPath = music["lobby"].as<std::string>();
        }

        // if (music.contains("game") && music["game"].is_string()) {
        //     gameMusicPath = music["game"];
        // }
    }
    if (json_data.contains("game_music")) {
        std::cout << "Oui contain game music\n";
        const json& gameMusic = json_data["game_music"];

        for (auto& kv : gameMusic.object_range()) {
            std::string key = kv.key();
            std::string value = kv.value().as<std::string>();
            loadGameMusic(key, value); // Charger chaque musique dans la map
        }
    }

    // Initialiser la musique de fond
    currentMusicPath = "";
    backgroundMusic.setLoop(true);
    backgroundMusic.setVolume(50.f); // Volume par défaut
}

SoundManager::~SoundManager()
{
}

void SoundManager::playSound(const std::string& name)
{
    if (soundBuffers.find(name) != soundBuffers.end()) {
        //std::cout << "Buffer found for sound: " << name << std::endl;
        auto sound = std::make_unique<sf::Sound>();
        sound->setBuffer(soundBuffers[name]);
        sound->setVolume(100.f);
        sound->play();
        //sound->stop();
        //std::cout << "Playing sound: " << name << std::endl;
        activeSounds.push_back(std::move(sound));
    } else {
        std::cerr << "Sound not found: " << name << std::endl;
    }
}

void SoundManager::update()
{
    //std::cout << "Active sounds before update: " << activeSounds.size() << std::endl;
    activeSounds.erase(
        std::remove_if(activeSounds.begin(), activeSounds.end(),
            [](const std::unique_ptr<sf::Sound>& sound) { 
                return sound->getStatus() == sf::Sound::Stopped; 
            }),
        activeSounds.end()
    );
    //std::cout << "Active sounds after update: " << activeSounds.size() << std::endl;
}

void SoundManager::loadSound(const std::string& name, const std::string& filePath)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(filePath)) {
        throw std::runtime_error("Failed to load sound: " + filePath);
    }
    soundBuffers[name] = std::move(buffer);
}

void SoundManager::loadGameMusic(const std::string& musicName, const std::string& filePath) {
    gameMusicPaths[musicName] = filePath;
}

void SoundManager::playGameMusic(const std::string& musicName) {
    if (gameMusicPaths.find(musicName) != gameMusicPaths.end()) {
        playBackgroundMusic(gameMusicPaths[musicName]);
    } else {
        std::cerr << "Game music not found: " << musicName << std::endl;
    }
}

void SoundManager::playMenuMusic()
{
    playBackgroundMusic(menuMusicPath);
}

void SoundManager::playLobbyMusic()
{
    playBackgroundMusic(lobbyMusicPath);
}

void SoundManager::playBackgroundMusic(const std::string& filePath)
{
    if (currentMusicPath != filePath) {
        backgroundMusic.stop(); // Arrêter la musique en cours
        if (!backgroundMusic.openFromFile(filePath)) {
            throw std::runtime_error("Failed to load background music: " + filePath);
        }
        currentMusicPath = filePath;
        backgroundMusic.play();
    }
}

void SoundManager::stopBackgroundMusic()
{
    backgroundMusic.stop();
    currentMusicPath = "";
}

void SoundManager::Stop_song(std::string song)
{
}

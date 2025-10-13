/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** SoundManager
*/

#ifndef SOUNDMANAGER_HPP_
    #define SOUNDMANAGER_HPP_
    #include <SFML/Audio.hpp>
    #include <map>
    #include <string>
    #include <iostream>
    #include <jsoncons/json.hpp>
    #include <fstream>
    #include <algorithm>
    #include <stdexcept>
    #include <fstream>

    using namespace jsoncons;

    class SoundManager {
        public:
            SoundManager();
            ~SoundManager();

            void playSound(const std::string& name);

            void update();
            void playMenuMusic();
            void playLobbyMusic();
            void playGameMusic(const std::string& musicName);
            void stopBackgroundMusic();
            void Stop_song(std::string song);

            void setBackgroundMusicVolume(float volume) {backgroundMusic.setVolume(volume); }
        private:
            std::map<std::string, sf::SoundBuffer> soundBuffers; // Buffers pour les effets sonores
            std::vector<std::unique_ptr<sf::Sound>> activeSounds;
            sf::Music backgroundMusic; // Musique de fond
            std::string currentMusicPath; // Chemin de la musique actuellement jouée

            // Chemins pour les musiques spécifiques
            std::string menuMusicPath;
            std::string lobbyMusicPath;
            std::map<std::string, std::string> gameMusicPaths; // Musiques pour le jeu

            void loadSound(const std::string& name, const std::string& filePath);
            void loadGameMusic(const std::string& musicName, const std::string& filePath);
            void playBackgroundMusic(const std::string& filePath);
    };

#endif

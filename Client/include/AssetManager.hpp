/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** AssetManager
*/

#ifndef ASSETMANAGER_HPP_
    #define ASSETMANAGER_HPP_
    #pragma once
    #include <jsoncons/json.hpp>
    #include <iostream>
    #include <ostream>
    #include <fstream>
    #include <map>
    #include <unordered_set>
    #include <SFML/Graphics.hpp>
    #include "../../GameEngine/Include/GameLogic.hpp"
    #include "../../GameEngine/Include/Mediator.hpp"
    #include "../../GameEngine/Include/RenderSystem.hpp"

    using namespace jsoncons;

    class AssetManager {
        public:
            AssetManager();
            ~AssetManager();
            void Init();
            std::vector<Sprite>& getPlayersSprites(void);
            Sprite& getMobsSprites(void);
            Sprite &getBoss1Sptrite(void);
            Sprite &getBoss2Sptrite(void);
            Sprite &getBulletSptrite(void);

            std::unordered_map<std::string, Sprite>& getSpriteMap(void);
            std::unordered_map<std::string, AnimationComponent>& getAnimationMap(void);
            std::unordered_map<std::string, Sprite>& getBackgroundSprites(void) {return _backgrounds_sprites;}
            std::unordered_map<std::string, Sprite>& getSkins(void) {return _skins;}
            void draw(sf::RenderWindow& window);
            void handleEvents(sf::RenderWindow& window, sf::Event& event);
            std::string getSelectedSkin(void) {return _selectedSkin;}

        protected:
        private:
            std::vector<Sprite>  _players_sprites;
            std::unordered_map<std::string, Sprite>  _backgrounds_sprites;
            Sprite _mobs_sprites;
            Sprite _boss1_sprites;
            Sprite _boss2_sprites;
            Sprite _projectiles_sprites;
            sf::Clock _messageClock;
            sf::Font _font;
            //sf::Sprite Back;
            sf::Texture *Back_texture = nullptr;

            //sf::Sprite button_Back;
            sf::Texture *button_texture = nullptr;

            std::unordered_map<std::string, Sprite> _sprites;
            std::unordered_map<std::string, Sprite> _skins;
            std::unordered_map<std::string, AnimationComponent> _animations;
            std::string  _selectedSkin = "skin1";

            int _currentPage = 0;
            const int SKINS_PER_PAGE = 8;
            // Nouvelle méthode pour calculer le nombre total de pages
            int getTotalPages() const {
                return static_cast<int>(ceil(static_cast<float>(_skins.size()) / SKINS_PER_PAGE));
            }

    };

#endif /* !ASSETMANAGER_HPP_ */

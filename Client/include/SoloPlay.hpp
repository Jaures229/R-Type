/*
** EPITECH PROJECT, 2025
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** SoloPlay
*/

#ifndef SOLOPLAY_HPP_
    #define SOLOPLAY_HPP_
    #pragma once
    #include <iostream>
    #include <ostream>
    #include <fstream>
    #include <SFML/Network.hpp>
    #include <SFML/Graphics.hpp>
    #include "../../Server/include/protocol.hpp"
    #include "../../GameEngine/Include/GameLogic.hpp"
    #include "../../GameEngine/Include/Mediator.hpp"
    #include "../../GameEngine/Include/RenderSystem.hpp"
    #include "AssetManager.hpp"
    #include "../../GameEngine/Include/SoundSystem.hpp"
    #include "../../GameEngine/Include/AnimationSystem.hpp"
    #include "../../GameEngine/Include/GameLogic.hpp"
    #include "../../GameEngine/Include/Mediator.hpp"
    #include "../../GameEngine/Include/MovementSystem.hpp"
    #include "../../GameEngine/Include/EnemySystem.hpp"
    #include "../../GameEngine/Include/ProjectileSystem.hpp"
    #include "../../GameEngine/Include/CollisionSystem.hpp"
    #include "../../GameEngine/Include/LuaScript.hpp"
    #include "../../GameEngine/Include/ObstacleSystem.hpp"
    #include "../../GameEngine/Include/CollectableSystem.hpp"
    #include "../../GameEngine/Include/Enemy_Bullet_System.hpp"
    #include "../../GameEngine/Include/BossSystem.hpp"
    #include <zlib.h>
    #include <vector>
    #include <stdexcept>
    #include <cstring>
    #include <atomic>
    #include <queue>
    #include <mutex>
    #include <condition_variable>
    #include "SoundManager.hpp"
    #include <boost/lexical_cast.hpp>
    #include <iomanip>

    class SoloPlay {
        public:
            SoloPlay(sf::RenderWindow & windows, Mediator & med);
            ~SoloPlay();
            void handle_event(sf::Event event);
            void run();
            void render(float deltaTime);
        protected:
        private:
            sf::RenderWindow & _windows;
            Mediator& _mediator;
            AssetManager _manager;

            GameLogic _logic;
            Signature mvSignature;
            Signature enemySignature;
            Signature projectileSignature;
            Signature collisionSignature;
            Signature obstacleignature;
            Signature collectableSignature;
            Signature enemybulletSignature;
            Signature BossSignature;
            Signature _signatureDisplay;
            Signature _signatureAnimation;
            LuaScript levelScript;
            sf::Clock clock;

            SoundManager soundManager;

            std::map<std::string, sf::Keyboard::Key> keyMapDefault = {
                {"Up", sf::Keyboard::Up},
                {"Down", sf::Keyboard::Down},
                {"Left", sf::Keyboard::Left},
                {"Right", sf::Keyboard::Right},
                {"Shoot", sf::Keyboard::Enter},
                {"Charge", sf::Keyboard::Space}
            };
            std::vector<std::string> _levels;

            std::string current_level = "";

            float level_time_elapsed = 0.0f;

            std::chrono::steady_clock::time_point level_start_time;

            std::shared_ptr<MovementSystem> _movement_system;
            std::shared_ptr<EnemySystem> enemy_movement_system;
            std::shared_ptr<RenderSystem> _render_system;
            std::shared_ptr<ProjectileSystem> projectile_system;
            std::shared_ptr<CollisionSystem> collision_system;
            std::vector<std::pair<int, Entity>> enemyList;
            std::shared_ptr<ObstacleSystem> _obstacle_system;
            std::shared_ptr<CollectableSystem> collectable_system;
            std::shared_ptr<Enemy_Bullet_System> enemyBullet_system;
            std::shared_ptr<BossSystem> boss_system;
            std::shared_ptr<AnimationSystem> _animation_system;

            std::chrono::steady_clock::time_point transition_start_time;

            std::vector<sf::Sprite> simple_explosion;
            std::vector<std::shared_ptr<sf::Texture>> simple_explosion_texture;

            std::vector<std::shared_ptr<sf::Texture>> boss_warning_texture;
            std::vector<sf::Sprite> warning_animation;
            std::vector<sf::Text> scoreboard_text;

            sf::Clock shootCooldownClock;
            sf::Clock chargeClock;
            bool isCharging = false;
            const float shootCooldown = 0.2f;
            const float chargeThreshold = 5.0f; // Temps nécessaire pour une charge complète (en secondes)

            sf::RectangleShape chargeBarBackground; // Fond de la barre
            sf::RectangleShape chargeBar;          // Barre de progression
            const float chargeBarWidth = 500.0f;   // Largeur de la barre
            const float chargeBarHeight = 20.0f;   // Hauteur de la barre

            std::unordered_set<Entity> _enemyToDelete;
            std::unordered_set<Entity>  _entityToDelete;
            int _score;

            float physics_accumulator = 0.0f;

            // Durées fixes pour les mises à jour
            const float physics_time_step = 1.0f / 30.0f; // Par exemple, 60 mises à jour par seconde        
            void fixedUpdate();
            void updatePhysics(float deltaTime);

            void update_level(float level_time_elapsed);
            int is_warning = 0;
            float parallaxSpeed = 0.9f;
            sf::Sprite backgroundLayer1;
            void paralax_scroll();
            void spawnEnemy(const std::string& type, const std::string& script, float x, float y, int colx, int coly, int health);
            void spawnBoss(const std::string& type, const std::string& script, float x, float y, int colx, int coly, int health, int nb_type);
    };

#endif /* !SOLOPLAY_HPP_ */

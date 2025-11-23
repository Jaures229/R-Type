/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** Game
*/

#ifndef GAME_HPP_
    #pragma once
    #define GAME_HPP_
    #include <istream>
    #include <iostream>
    #include <fstream>
    #include <random>
    #include <optional>

    // Structure pour représenter un power-up
    struct PowerUp {
        std::string name;
        int x; // Position X
        int y; // Position Y
    };
    class Game {
        public:
            Game() {}
            ~Game() {}

        protected:
        private:
    };

    class Level
    {
        private:
            std::string _level_name;
            // int player_health;
            // int player_speed;
            // float player_damage;
            // int mobs_health;
            // float mobs_damage;
            // float mobs_speed;
            // int boss_health;
            // float boss_damage;
            // int boss_speed;
            // std::string type;
            // int score;
    public:
        Level(/* args */) {}
        ~Level() {}

        // Getters
        std::string getLevelName() const { return _level_name; }
        // int getPlayerHealth() const { return player_health; }
        // int getPlayerSpeed() const { return player_speed; }
        // float getPlayerDamage() const { return player_damage; }
        // int getMobsHealth() const { return mobs_health; }
        // float getMobsDamage() const { return mobs_damage; }
        // int getMobsSpeed() const { return mobs_speed; }
        // int getBossHealth() const { return boss_health; }
        // float getBossDamage() const { return boss_damage; }
        // float getBossSpeed() const { return boss_speed; }
        // std::string getType() const {return type;}
        // int getLevelScore() {return score;}
        

        // Setters
        void setLevelName(const std::string& level_name) { _level_name = level_name; }
        // void setPlayerHealth(int health) { player_health = health; }
        // void setPlayerSpeed(float speed) { player_speed = speed; }
        // void setPlayerDamage(float damage) { player_damage = damage; }
        // void setMobsHealth(int health) { mobs_health = health; }
        // void setMobsDamage(float damage) { mobs_damage = damage; }
        // void setMobsSpeed(float speed) { mobs_speed = speed; }
        // void setBossHealth(int health) { boss_health = health; }
        // void setBossDamage(float damage) { boss_damage = damage; }
        // void setBossSpeed(float speed) { boss_speed = speed; }
        // void setType(std::string _type) {type = _type;}
        // void setLevelScore(int sc) {score = sc;}
    };

#endif /* !GAME_HPP_ */


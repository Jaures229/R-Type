/*
** EPITECH PROJECT, 2024
** test_net
** File description:
** network
*/

#ifndef NETWORK_HPP_
    #define NETWORK_HPP_
    #pragma once
    #include <iostream>
    #include <string.h>
    #include <vector>
    #include <map>
    #include <memory>
    #include <boost/asio.hpp>
    #include <jsoncons/json.hpp>
    #include <boost/json.hpp>
    #include <mutex>
    #include <condition_variable>
    #include <thread>
    #include <exception>
    #include <fstream>
    #include <cassert>
    #include <random>
    #include <optional>
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
    #include <boost/uuid/uuid.hpp>
    #include <boost/uuid/uuid_generators.hpp>
    #include <boost/uuid/uuid_io.hpp>
    #include <boost/lexical_cast.hpp>
    #include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
    #include "boost/filesystem/fstream.hpp"    // ditto
    #include "protocol.hpp"
    #include "Game.hpp"
    #include <zlib.h>
    #include "tuple"
    #include <algorithm>
    #include "../../GameEngine/Include/Quadtree.hpp"
    #include "../../GameEngine/Include/BossSystem.hpp"

    using namespace jsoncons;

    #define SERVER_PORT 5254

    using namespace boost::asio;
    using namespace boost::filesystem;

    using boost::asio::ip::udp;
    using boost::asio::ip::tcp;


    class ClientInfo
    {
        private:
            std::string _ip;
            unsigned short _port;
            udp::endpoint _client_enddpoint;
            int _id;
            int PosX;
            int PosY;
        public:
            ClientInfo(const std::string &ip, unsigned short port, int id);
            ~ClientInfo();
            std::string getIp() const;
            void set_PosX(int x) {PosX = x;}
            void set_PosY(int y) {PosY = y;}

            int get_PosX() const {return PosX;}
            int get_PosY() const {return  PosY;}

            void set_Port(unsigned short port) { _port = port;}
            void set_Ip(std::string &ip) {_ip = ip;}
            unsigned short getPort() const;
            std::string getName() const;
            void setName(std::string name);
            int getID() const;
            std::string _name;
    };

    class GameServer
    {
        private:
            udp::socket _socket;
            udp::endpoint _sender_endpoint;
            std::vector<ClientInfo> _clients;
            Mediator &_mediator;
            char _data[1024];
            GameLogic _logic;
            Signature mvSignature;
            Signature enemySignature;
            Signature projectileSignature;
            Signature collisionSignature;
            Signature obstacleignature;
            Signature collectableSignature;
            Signature enemybulletSignature;
            Signature BossSignature;

            float level_time_elapsed = 0.0f;

            steady_timer _timer;

            std::chrono::steady_clock::time_point level_start_time;
            short _port;

            std::shared_ptr<MovementSystem> _movement_system;
            std::shared_ptr<EnemySystem> enemy_movement_system;
            std::shared_ptr<ProjectileSystem> projectile_system;
            std::shared_ptr<CollisionSystem> collision_system;
            std::vector<std::pair<int, Entity>> enemyList;
            std::shared_ptr<ObstacleSystem> _obstacle_system;
            std::shared_ptr<CollectableSystem> collectable_system;
            std::shared_ptr<Enemy_Bullet_System> enemyBullet_system;
            std::shared_ptr<BossSystem> boss_system;

            std::vector<int> _enemy_bullet;
            int _enemys_ids = 1;

            int player_dmg = 0;
            int game_staterd = 1;

            std::string current_level = "";
            int current_level_num = 0;
            // int level_time_elapsed = 0;
            bool is_game_active = false;

            LuaScript levelScript;

            // Game varaibles
            int _score = 0;
            int _players_init_health;
            int _mobs_init_health;
            int _players_init_spedd;
            int _mobs_init_speed;
            int _boss1_init_speed;
            int power_speed;
            int power_damage;
            int power_health;
            float spawn1_frequency;
            float spawn2_frequency;
            //int level = 1;
            int _score_multipliyer = 0;

            Entity _boss;
            bool bossActive = false;
            boost::uuids::random_generator generator;

            std::unordered_set<Entity> _enemyToDelete;
            std::unordered_set<Entity> _entityToDelete;
            std::vector<std::pair<std::string, Entity>> entitiesToCreate;
            std::vector<std::pair<std::string, Entity>> BossToCreate;
            std::vector<std::pair<std::string, Entity>> PowerUpToCreate;
            std::vector<int> EnemyBulletToCreate;
            int is_warning;
            //std::vector<std::pair<Entity, int>> player_bullet_type;

            Quadtree _quadtree;
            std::vector<std::string> powerUps = {"Health", "Damage", "Speed"};
            int send_game_id = 0;
            const float spawn_interval = 15.0f;
            float time_since_last_spawn = 0.0f; // Temps écoulé depuis le dernier spawn

            std::vector<Level>& _levels;
            int in_level = 0;
            bool in_transition = false;
            std::chrono::steady_clock::time_point transition_start_time;
            std::map<int, std::string>& _players_uuids;
            int game_finish = 0;
            void Handle_game_end();            
        public:
            GameServer(io_context& io_context, std::string &ip ,short port, std::vector<ClientInfo>& clients, Mediator& med, std::vector<Level>& levels, std::map<int, std::string>& players_uuids);
            GameServer() = default;
            ~GameServer();
            void Init();
            void start_game();
            void close_game();


            std::string _game_id_session;
            
            void start_receive();
            void start_update();
            void send_udp_message(udp::endpoint endpoint, std::vector<char> message);
            int GetClientId(udp::endpoint endpoint);
            void handle_game_data(char *data, std::size_t length, udp::endpoint endpoint);
            void initBoss_Enemy();
            void init_enemy();
            std::set<std::pair<int, int>> enemy_pos;
            std::pair<int, int> generateUniquePosition(std::set<std::pair<int, int>>& existingPositions, int xMin, int xMax, int yMin, int yMax);


            void GenerateProceduralLevel();

            void updateLevel(float currentTime);

            std::string Generate_UUID(void);

            // spawn enemies function
            void spawnEnemy(const std::string& type, const std::string& script, float x, float y, int colx, int coly, int health);

            void spawnBoss(const std::string& type, const std::string& script, float x, float y, int colx, int coly, int health, int nb_type);

            void Send_to_all_client(std::vector<char> msg);


            void Delete_ent_enemy(std::unordered_set<Entity> enemyToDelete);

            void Delete_ent_simple(std::unordered_set<Entity> enemyToDelete);
            void spawn_power_ups();
            void handle_boss_death();
            // updates function

            // update the physics
            void fixedUpdate(float deltaTime);

            // update the network
            void network_update();

            void Update_player();
            void Update_enemy();
            void Update_projectile();
            void Update_deleted_entities();
            void Generate_obstacles();
            void Update_enemy_bullets();
            void Update_power_up();

            void Update_collsion();
            void Boss_update();
            void Game_update();
            void Physic_update(float deltaTime);
            void Current_game_update();
            int getGamefinish(void) { return game_finish;}
    };

    class LobbyTCP
    {
        public:
            LobbyTCP(io_context& io_context, std::string &ip, short port, const std::string& game_server_ip, unsigned short game_server_port);
            ~LobbyTCP();
            const std::vector<ClientInfo>& get_clients() const;
            bool is_game_server_ready() const;
            void start_lobby();
            void close();
            Level& getCurrentLevel(void);
            std::vector<Level>& GetLevels(void) { return game_levels;}
            std::map<int, std::string>& GetPlayers_uuids(void) {return _players_uuids;}
            int nb_player = 0;
        private:
            void start_accept();
            void start_read(std::shared_ptr<tcp::socket> client_socket);
            void handle_message(std::shared_ptr<tcp::socket> client_socket, std::size_t length);
            void redirect_to_game_server(std::shared_ptr<tcp::socket> client_socket);
            void send_start_failure();
            void send_start_success();
            void handle_authentification(std::shared_ptr<tcp::socket> client_socket, uint32_t length);
            void send_message(std::shared_ptr<tcp::socket> client_socket, std::vector<char> message);
            void notify_all(std::vector<char> msg);
            void handle_start(std::shared_ptr<tcp::socket> client_socket, uint32_t length);
            void remove_client(std::shared_ptr<tcp::socket> client_socket);
            std::pair<int, int> generateUniquePosition(std::set<std::pair<int, int>>& existingPositions, int xMin, int xMax, int yMin, int yMax);
            void parse_file(void);
            void handle_level_choice(BinaryProtocol::MessageHeader* header, std::shared_ptr<tcp::socket> client_socket);
            void handle_level_start(BinaryProtocol::MessageHeader* header, std::shared_ptr<tcp::socket> client_socket);

            tcp::acceptor _acceptor;
            std::string game_server_ip;
            unsigned short game_server_port;
            std::vector<ClientInfo> _clients;

            std::vector<char> _data;
            int max_nb;
            int auth_nb = 0;
            bool game_server_ready = false;
            std::atomic<bool> _is_running{true};
            std::vector<std::shared_ptr<tcp::socket>> _client_sockets;
            int count = 1;
            std::set<std::pair<int, int>> existingPositions;
            std::vector<Level> game_levels;
            Level choose_level;

            std::string game_id;
            boost::uuids::random_generator lobby_generator;

            std::string Generate_UUID(void);
            std::map<int, std::string> _players_uuids;
            
    };
    std::vector<std::pair<std::string, int>> getSortedScoreboard(std::map<int, std::string>& players_uuids, std::unordered_map<Entity, int>& players_scores);
#endif /* !NETWORK_HPP_ */

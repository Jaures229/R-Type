/*
** EPITECH PROJECT, 2024
** test_network
** File description:
** protocol
*/

#ifndef PROTOCOL_HPP_
    #define PROTOCOL_HPP_
    #include <iostream>
    #include <string>
    #include <cstring>
    #include <cstdint>
    #include <vector>
    #include <map>
    #include <unordered_set>
    #include <unordered_map>
    #include <zlib.h>
    #include <stdexcept>

    /// @brief : Namespace for all that concern the Binary Protocol
    namespace 
    BinaryProtocol {

        /// different type of message
        enum class MessageType : uint8_t {
            AUTHENTICATION,
            AUTHENTICATION_SUCCESS,
            AUTHENTICATION_FAILURE,
            ACTION,
            START,
            START_SUCCESS,
            START_FAILURE,
            GAME_DATA,
            NEW_CLIENT,
            CLIENT_QUIT, // for the client
            CLIENT_CLOSE, // for the server
            LEFT,
            UP,
            DOWN,
            RIGHT,
            CLIENT_LIST,
            SHOOT,
            CLIENT_UPDATE,
            CLIENT_ID,
            ENEMY_SBIRES_CREATE,
            SHOOT_CREATE,
            SHOOT_UDPATE,
            ENEMY_SBIRES_UPDATE, // for enemies
            LEVEL_LIST,
            START_LEVEL,
            SINGLE_ENEMY_UPDATE,
            SINGLE_PROJECTILE_UPDATE,
            CHOOSE_LEVEL,
            CURRENT_LEVEL,
            SIMPLE_ENEMY_CREATE,
            ENEMY_DELETE,
            SCORE,
            GET_LEVEL,
            ALL_ENEMY_UPDATE,
            DELETE_ONE_ENEMY,
            UPDATE_OBS,
            CREATE_OBS,
            CREATE_POWER_UP,
            UPDATE_POWER_UP,
            PLAYER_HEALTH,
            GAME_ID,
            COLLISION,
            ENEMY_BULLET_CREATE,
            ENEMY_BULLET_UPDATE,
            BOSS_INCOMMING,
            BOSS_CREATE,
            BOSS_UPDATE,
            BOSS_DEATH,
            LEVEL_TIME,
            BOSS_SHOT,
            CHARGE_ATTACK,
            TIMER,
            START_NEXT_LEVEL,
            LEVEL_INFO,
            GAME_FINISH,
           // ENEMY_BOSS // for enemies
        };

        /// Header for the protocol
        struct MessageHeader {
            MessageType type;
            uint32_t length; // Length of the message data
            int compressed;
            uint32_t uncompressed_length;
        };

        enum class ResponseStatus : uint8_t {
            SUCCESS,
            FAILURE,
        };

        struct ResponseHeader {
            ResponseStatus status;
            uint32_t length;
        };

        struct AuthMessage {
            uint32_t username_length;
            char username[1024];
        };
        struct ActionMessage {
            uint32_t action_length;
            char action[100];
        };

        struct SimpleMessage {
            uint32_t msg_length;
            char msg[100];
        };

        struct Auth {
            uint32_t auth_length;
            int id;
            int posX;
            int posY;
            char msg[100];
        };
        struct NewClient {
            uint32_t client_length;
            int id;
            int posX;
            int posY;
            char name[100];
        };
        struct Client_list_t {
            uint32_t clients_length;
            int ids[10];
            int posX[10];
            int posY[10];
            int nb_client;
        };

        struct Client_update  {
            uint32_t clients_length;
            int id;
            int posX;
            int posY;
        };

        struct Movement {
            uint32_t movement_length;
            int id;
        };

        struct Quit_client {
             uint32_t client_length;
            int id;
        };

        struct Client_id {
            uint32_t client_length;
            int id;            
        };

        struct Client_list_update {
            uint32_t clients_length;
            int ids[10];
            int posX[10];
            int posY[10];
            int nb_client;
        };

        struct Enemy_list {
            uint32_t enemy_length;
            int ids[100]{0};
            int posX[100]{0};
            int posY[100]{0};
            int nb_client;
        };

        struct Shoot_info {
            uint32_t projectile_length;
            int id[100];
            int posX[100];
            int posY[100];
            int type[100];
            int nb_client;
        };

        struct Level_list {
            uint32_t string_count;
            char strings[10][50]; 
        };

        struct Enemy {
            uint32_t enemy_length;
            int ids;
            int posX;
            int posY;
            char type[100];
            int nb_client;
        };

        struct Projectile_update {
            uint32_t string_count;
            char id[500];
            int x;
            int y;
            /* data */
        };

        struct Enemy_update {
            uint32_t string_count;
            char id[1024];
            int x;
            int y;
            /* data */
        };
        
        struct Enemy_delete {
            int id;
            uint32_t enemy_length;
        };

        struct Score {
            int score;
            uint32_t score_length;
        };

        struct Players_health  {
            int health[100];
            int ids[100];
            int nb_client;
            uint32_t health_length;
        };


        struct Message {
            char msg[1024];
            uint32_t length;
        };

        struct Obstacle {
            uint32_t ob_length;
            int ids[100]{0};
            int posX[100]{0};
            int posY[100]{0};
            int type;
            int nb_obstacle;
        };

        struct Power_up_create {
            uint32_t ob_length;
            int id;
            int posX;
            int posY;
            int type;
            int nb_obstacle;
        };
        struct Power_up_update {
            uint32_t ob_length;
            int id;
            int posX;
            int posY;
            int type;
            int nb_obstacle;
        };

        struct All_collision {
            int nb_collision;
            int type[100];
            int x[100];
            int y[100];
        };

        struct Protocol_Enemy_Bullet {
            int ids[100];
            int nb_bullet;
            int type[100];
            int x[100];
            int y[100];
        };

        struct Protocol_Enemy_Bullet_create {
            int ids;
            int nb_bullet;
            int type;
            int x;
            int y;
        };

        struct Boss_create {
            int id;
            int x;
            int y;
            char type[100];
            int nb_boss;
            int health;
        };

        struct Boss_update {
            int id;
            int x;
            int y;
            int health;
        };

        struct Boss_incomming {
            int id;
            int x;
            int y;
        };
        struct Level_time {
            float time;
        };

        struct NextLevel_Protocol {
            char level[100];
            int current;
            int total;
        };
        struct PlayerScore {
            char name[100];               // Nom du joueur (longueur fixe, max 32 caractères)
            int score;                   // Score du joueur
        };

        struct Protocol_GameFinish {
            uint16_t playerCount;
            PlayerScore players[10];
        };

        
        // function to return the authentification message inn binary
        std::vector<char> GetAuthMessage(std::string& message);

         // function to return an action message in binary
        std::vector<char> GetActionMessage(std::string& message);

         // function to return an action message in binary
        std::vector<char> GetStartMessage(std::string& message);

        std::vector<char> GetStartMessage(std::string& message);

        std::vector<char> GetStartFailureMessage(std::string& message);

        std::vector<char> GetStartSuccessMessage(std::string& message);

        std::vector<char> GetAuthSuccessMessage(std::string& message, int id, int x, int y);

        std::vector<char> GetGameDataMessage(std::string& message);

        std::vector<char> GetNewClientMessage(std::string& message, int id, int x, int y);

        std::vector<char> GetNewClientListMessage(int* ids, int nb_client, std::vector<std::pair<int, int>> positions);

        std::vector<char> GetClienClosetMessage(int id);

        std::vector<char> GetLeftMesaage(int id);

        std::vector<char> GetRightMesaage(int id);

        std::vector<char> GetUPMesaage(int id);

        std::vector<char> GetDownMesaage(int id);

        std::vector<char> GetShootMesaage(int id);

        std::vector<char> GetPlayerHealth(int *id, int *health, int nb_players);

        std::vector<char> GetCleintIDMessage(int id);

        std::vector<char> GetClientUpdateAll(int *ids, int nb_client, int *x, int *y);

        std::vector<char> GetprojectileList(int *ids, int nb_enemy, int *x, int *y, int *type);

        std::vector<char> GetprojectileListUpdate(int *ids, int nb_enemy, int *x, int *y, int *type);

        std::vector<char> GetEnemyListUpdate(int *ids, int nb_enemy, int *x, int *y);

        std::vector<char> GetStartLevelMessage(std::string &msg);

        std::vector<char> GetEnemyCreate(int id, int nb_enemy, int x, int y, std::string & type);
        std::vector<char> GetEnemyDelete(int id);

        std::vector<char> GetScore(int score);

        std::vector<char> GetLevel(std::string level);
        std::vector<char> GetPowerCreate(int id, int x, int y, std::string & type);
        std::vector<char> GetPowerUpdate(int id, int x, int y);
        std::vector<char> GetObstacleCreate(int *id, int nb_obstacle, int *x, int *y, int type);

        std::vector<char> GetAllColllision(int *type, int *x, int *y, int nb_collison);

        std::vector<char> GetEnemyBulletCreate(int ids, int x, int y, int type, int nb_enemy);
        std::vector<char> GetEnemyBulletUpdate(int *ids, int *x, int *y, int nb_enemy);

        std::vector<char> GetBossCreate(int id, int x, int y, int nb_boss, std::string & type, int health);
        std::vector<char> GetBossUpdate(int id, int x, int y, int nb_boss, int health);

        std::vector<char> GetBossIncomming(int id, int x, int y);
        std::vector<char> GetLevelTime(float time);

        std::vector<char> GetBossBulletCreate(int ids, int x, int y, int type, int nb_enemy);
        std::vector<char> GetChargeAttack(int id);
        std::vector<char> GetBossDeath(void);
        std::vector<char> GetTimer(float timer);
        std::vector<char> NextLevel(void);
        std::vector<char> LevelInfo(std::string level, int current, int total);
        std::vector<char> GameFinish(std::vector<std::pair<std::string, int>>& scoreboard);
    }
#endif /* !PROTOCOL_HPP_ */

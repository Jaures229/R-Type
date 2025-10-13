/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** client
*/

#ifndef CLIENT_HPP
    #define BUFFER_SIZE 1024
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
    #include <zlib.h>
    #include <vector>
    #include <stdexcept>
    #include <cstring>
    #include <atomic>
    #include <queue>
    #include <mutex>
    #include <condition_variable>
    #include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
    #include "boost/filesystem/fstream.hpp"
    #include "SoundManager.hpp"
    #include "NetworkEntityMapper.hpp"
    #include <boost/lexical_cast.hpp>
    #include <iomanip>
    #include "NameEntryScreen.hpp"
    #include "SoloPlay.hpp"

    using namespace boost::filesystem;
    using boost::lexical_cast;
    using boost::bad_lexical_cast;

    struct Explosion {
        float x, y;               
        int current_frame = 0;
        sf::Clock animation_timer;
    };

    class Client
    {
        private:

            bool isKeyUp = false;
            bool isKeyDown = false;
            bool isKeyLeft = false;
            bool isKeyRight = false;
            bool isKeyShoot = false;
            bool clicked = false;

            std::map<std::string, sf::Keyboard::Key> keyb_list = {
                {"A",  sf::Keyboard::A},
                {"B",  sf::Keyboard::B},
                {"C",  sf::Keyboard::C},
                {"D",  sf::Keyboard::D},
                {"E",  sf::Keyboard::E},
                {"F",  sf::Keyboard::F},
                {"G",  sf::Keyboard::G},
                {"H",  sf::Keyboard::H},
                {"I",  sf::Keyboard::I},
                {"J",  sf::Keyboard::J},
                {"K",  sf::Keyboard::K},
                {"L",  sf::Keyboard::L},
                {"M",  sf::Keyboard::M},
                {"N",  sf::Keyboard::N},
                {"O",  sf::Keyboard::O},
                {"P",  sf::Keyboard::P},
                {"Q",  sf::Keyboard::Q},
                {"R",  sf::Keyboard::R},
                {"S",  sf::Keyboard::S},
                {"T",  sf::Keyboard::T},
                {"U",  sf::Keyboard::U},
                {"V",  sf::Keyboard::V},
                {"W",  sf::Keyboard::W},
                {"X",  sf::Keyboard::X},
                {"Y",  sf::Keyboard::Y},
                {"Z",  sf::Keyboard::Z},
                {"Up",  sf::Keyboard::Up},
                {"Down",  sf::Keyboard::Down},
                {"Left",  sf::Keyboard::Left},
                {"Right",  sf::Keyboard::Right},
                {"Numpad0",  sf::Keyboard::Numpad0},
                {"Numpad1",  sf::Keyboard::Numpad1},
                {"Numpad2",  sf::Keyboard::Numpad2},
                {"Numpad3",  sf::Keyboard::Numpad3},
                {"Numpad4",  sf::Keyboard::Numpad4},
                {"Numpad5",  sf::Keyboard::Numpad5},
                {"Numpad6",  sf::Keyboard::Numpad6},
                {"Numpad7",  sf::Keyboard::Numpad7},
                {"Numpad8",  sf::Keyboard::Numpad8},
                {"Numpad9",  sf::Keyboard::Numpad9}
            };

            std::map<std::string, sf::Keyboard::Key> keyMapDefault = {
                {"Up", sf::Keyboard::Up},
                {"Down", sf::Keyboard::Down},
                {"Left", sf::Keyboard::Left},
                {"Right", sf::Keyboard::Right},
                {"Shoot", sf::Keyboard::Enter},
                {"Charge", sf::Keyboard::Space}
            };

            std::size_t byte_received;

            int lobby_port = 12345;
            unsigned short port;

            bool has_start;
            bool game_started;
            bool lobby_started;

            bool has_start_error;
            bool in_replay;

            sf::IpAddress recipient;
            sf::TcpSocket tcp_socket;

            // selector

            Position player_position;

            sf::SocketSelector _selector;
            sf::SocketSelector _udp_selecort;
            sf::UdpSocket udp_socket;
            sf::Socket::Status tcp_sckt_status;
            int _id;
            std::string _name;
            

            // Grpahics variable
            AssetManager _manager;
            sf::RenderWindow _window;
            sf::Event _event;
            sf::Text _start_text;
            sf::Text _quit_text;
            sf::Text _setting_button;
            sf::Text _start_button;
            sf::Font _global_font;
            sf::Font _game_font;
            sf::Font _test_font;

            sf::Vector2i mousePos;

            std::vector<std::pair<int, Sprite>> spaceships;
            std::vector<std::pair<int, Entity>> players;
            std::vector<std::pair<int, Entity>> enemy_sbires_vector;
            std::vector<std::pair<int, Entity>> projectiles;
            std::vector<sf::Text> levels_texts;
            sf::Sprite lobby_sprite;
            sf::Texture lobby_texture;
            sf::Text lobby_text;
            sf::Text game_name;

            sf::Sprite menu_sprite;
            sf::Texture menu_texture;

            sf::Sprite heart_sprite;
            sf::Texture heart_texture;
            //Scroll 
            sf::Texture backgroundTexture1;
            sf::Sprite backgroundLayer1, backgroundLayer2;
            float parallaxSpeed;

            sf::Text quit_text;
            sf::Text _text_status;
            sf::Text _choose_level;
            sf::Clock error_clock;
            std::string _current_level;

            std::unordered_map<int, Entity> all_entity;
            //Entity variable
            Mediator& _medi;
            GameLogic _logic;
            Entity player;
            Signature _signatureDisplay;
            Signature _signatureAnimation;
            std::shared_ptr<AnimationSystem> _animation_system;
            sf::Clock clock;

            // Variables pour l'animation du lobby
            std::vector<sf::Texture> lobbyFrames;       // Vecteur pour stocker les textures des frames
            sf::Sprite animatedLobbySprite;             // Sprite pour afficher l'animation du lobby
            sf::Clock animationClock;                   // Horloge pour contrôler la fréquence des frames
            int currentFrame = 0;                       // Indice de la frame actuelle
            float frameDuration = 0.05f;                 // Durée d'une frame en secondes

            sf::RenderWindow optionsWindow;
            sf::Text instructions;

            sf::Text upButton;
            sf::Text downButton;
            sf::Text leftButton;
            sf::Text rightButton;
            sf::Text shootButton;
            sf::Text saveButton;

            sf::Text upButtonAssigned;
            sf::Text ModedButton;
            sf::Text downButtonAssigned;
            sf::Text leftButtonAssigned;
            sf::Text rightButtonAssigned;
            sf::Text shootButtonAssigned;

            sf::Text _score;
            sf::Text back_text;
            sf::Text _replay_text;
            sf::Text _replay_header;
            std::string _game_session;
            std::string replays_path;

            std::string player_skins = "";
            std::string player_name = "";
            bool in_skin = false;

            std::vector<sf::Sprite> simple_explosion;
            std::vector<std::shared_ptr<sf::Texture>> simple_explosion_texture;

            std::vector<std::shared_ptr<sf::Texture>> boss_warning_texture;
            std::vector<sf::Sprite> warning_animation;
            std::vector<sf::Text> scoreboard_text;

            void load_replays();    
            void handle_auth_success(std::vector<char> buffer);
            void handle_update(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void handle_client_list(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void enter_lobby_event();
            void handle_new_client(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void handle_start_success(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void handle_start_failure(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void handle_close(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            //void create_Enemy(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void update_Enemy(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void shoot_udpade(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void shoot_create(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void handle_current_level(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            
            std::string keyAssigned(sf::Keyboard::Key key);
            void defaultColorSettingButton();
            void saveKeyBindingsToFile();
            void loadKeyBindingsFromFile();
            void settings();
            void handle_score(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void handle_enemy_delete(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void create_simple_enemy(BinaryProtocol::MessageHeader header, std::vector<char> buffer);

            void update_simple_enemy(BinaryProtocol::MessageHeader header, std::vector<char> buffer);

            void delete_simple_enemy(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void capture_frame(const std::string& filename);
            void record_session();

            void Set_health_bar();
            void display_warning();

            std::string generate_filename(int frame_number);
            void create_obstacle(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            int score = 0;
            int r_score = 0;

            int is_moded = 0;
            bool record;
            bool isAnimating = false;
            sf::Clock warningTimer;
            int warningcurrentFrame = 0;

            std::string current_level = "";

            void create_power(BinaryProtocol::MessageHeader header, std::vector<char> buffer);

            void update_power(BinaryProtocol::MessageHeader header, std::vector<char> buffer);

            void create_game_session(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void update_health(BinaryProtocol::MessageHeader header, std::vector<char> buffer);

            void update_collision(BinaryProtocol::MessageHeader header, std::vector<char> buffer);

            void display_collison();
            void display_explosion();
            void add_explosion(float x, float y);

            void save_frames_to_files();

            void handle_level(BinaryProtocol::MessageHeader header, std::vector<char> buffer);

            void close_game();

            void start_recording();
            void stop_recording();
            void process_recording();
            void create_enemy_bullet(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void update_enemy_bullet(BinaryProtocol::MessageHeader header, std::vector<char> buffer);

            void create_boss(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void update_boss(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void handle_incomming_boss(BinaryProtocol::MessageHeader header, std::vector<char> buffer);


            void handle_level_time(BinaryProtocol::MessageHeader header, std::vector<char> buffer);

            void handle_boss_shot(BinaryProtocol::MessageHeader header, std::vector<char> buffer);

            void handle_boss_death(BinaryProtocol::MessageHeader header, std::vector<char> buffer);

            void handle_next_level(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void display_boss_health_bar();

            void handle_start_next_level(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void Display_next_level(void);
            void handle_game_finish(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void handle_level_info(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void display_scoreboard();
            std::vector<Collider> collision_to_display;

            std::string Current_boss = "";
            // Dimensions de la barre de vie
            const float max_width = 100.f;
            const float height = 7.f;    // Hauteur de la barre
            const float border_thickness = 2.f; // Épaisseur de la bordure blanche
        
            std::vector<Explosion> active_explosions;
            std::vector<sf::Image> captured_frames;
            int start_record = 1;
            int saved_frame = 0;
            int is_boss_comming = 0;

            std::atomic<bool> is_recording{false};
            std::thread record_thread;

            std::queue<sf::Image> frame_queue; // Tampon partagé pour les frames
            std::mutex queue_mutex;
            std::condition_variable condition;

            std::string game_replay_dir;
            SoundManager soundManager;
            bool isHoveringStartText = false;
            bool isHoveringSettingButton = false;
            bool isHoveringQuitText = false;
            bool isHoveringReplayText = false;
            bool isHoveringModedText = false;
            NetworkEntityMapper entityMapper;

            int level_passed = 0;

            Entity Boss_ref = -1;
            sf::Text _back_text;
            bool in_solo_play = false;

            float level_time = 0.0f;
            sf::Text level_time_text;
            void game_update(std::shared_ptr<RenderSystem>& _render_system);

            sf::Clock shootCooldownClock;
            sf::Clock chargeClock;
            bool isCharging = false;
            const float shootCooldown = 0.2f;
            const float chargeThreshold = 5.0f; // Temps nécessaire pour une charge complète (en secondes)

            sf::RectangleShape chargeBarBackground; // Fond de la barre
            sf::RectangleShape chargeBar;          // Barre de progression
            const float chargeBarWidth = 500.0f;   // Largeur de la barre
            const float chargeBarHeight = 20.0f;   // Hauteur de la barre

            sf::Text Level_passed_text;
            sf::Text Timer_text;
            sf::Text Level_failed_text;

            sf::Text Level_info_text;
            sf::Text Level_text;
            int game_finish = 0;
            sf::Text credits_text;
            std::vector<std::pair<std::string, int>> scoreboard;
            sf::Text solo_play_text;
            
        public:
            Client(sf::IpAddress addresse, unsigned short connexion_port, Mediator & med);

            SoundSystem soundsys;
            void connect_socket();
            void send_tcp_message(std::vector<char> & message);
            void receive_tcp_message(Mediator& _medi);
            void enter_game_event();
            void update();
            void send_udp_message(std::vector<char> & message);
            void receive_udp_message();
            void tcp_authenticate(std::string msg);
            void send_close_message();
            void run();
            void Init( Mediator&);
            void game_loop();
            void InputManagement();
            void paralax_scroll();
            void handle_level_loaded(BinaryProtocol::MessageHeader header, std::vector<char> buffer);
            void updateLobbyAnimation();
            ~Client();
    };
#endif

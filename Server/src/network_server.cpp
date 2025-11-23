/*
** EPITECH PROJECT, 2024
** test_net
** File description:
** network_server
*/

#include "../include/network.hpp"

GameServer::GameServer(io_context& io_context, std::string &ip ,short port, std::vector<ClientInfo>& clients, Mediator& med, std::vector<Level>& levels, std::map<int, std::string>& players_uuids)
: _socket(io_context, udp::endpoint(boost::asio::ip::make_address(ip), port)), _clients(clients), _mediator(med), _logic(_mediator),
_timer(io_context, boost::asio::chrono::milliseconds(30)), _levels(levels), _quadtree(0, sf::FloatRect(0, 0, 1920, 1080)), _players_uuids(players_uuids)
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    //std::cout << "Game server was created on " << ip << ":" << port << std::endl;
    //std::cout << "UDP Server Client:\n";
    _port = port;
    //std::chrono::steady_clock::time_point level_start_time;
    is_game_active = true;

    current_level = _levels[0].getLevelName();

    Entity temp = _mediator.createEntity();
    // for (const auto &elem : _players_uuids) {
    //     std::cout << "Player : " << elem.first << " Name " << elem.second << std::endl;
    // }
    //_game_id_session = Generate_UUID();
}

GameServer::~GameServer()
{
    _socket.close();
    std::cout << "Game server was destroyed" << std::endl;
}

void GameServer::start_receive()
{
    _socket.async_receive_from(boost::asio::buffer(_data), _sender_endpoint,
        [this](const std::error_code &error, std::size_t data_length) {
            if (!error) {
                handle_game_data(_data, data_length, _sender_endpoint);
                start_receive();
            } else {
                std::cerr << "Error receiving data: " << error.message()
                          << " (Error code: " << error.value() << ")\n";
                std::cerr << "Sender endpoint: " << _sender_endpoint.address().to_string()
                          << ":" << _sender_endpoint.port() << std::endl;
            }
    });
}

void GameServer::handle_game_data(char* data, std::size_t length, udp::endpoint endpoint)
{
    if (length < sizeof(BinaryProtocol::MessageHeader)) {
        std::cerr << "Incompatible header Type\n";
    }

    BinaryProtocol::MessageHeader *header = reinterpret_cast<BinaryProtocol::MessageHeader*>(data);

    BinaryProtocol::Movement* mov_data = reinterpret_cast<BinaryProtocol::Movement*>(data + sizeof(BinaryProtocol::MessageHeader));

     //overwrite the ip and the ip at each receive command
     for (int i = 0; i < _clients.size(); i++) {
        if (_clients[i].getID() == mov_data->id) {
            std::string temp_ip = _sender_endpoint.address().to_string();
            int temp_port = _sender_endpoint.port();
            _clients[i].set_Ip(temp_ip);
            _clients[i].set_Port(temp_port);
        }
    }

    int a = GetClientId(endpoint);
    if (header->type ==  BinaryProtocol::MessageType::UP) {
        if (a != -1) {
            //std::cout << "ok il entre\n";
            auto& vit = _mediator.GetComponent<Vitesse>(_logic.getEntity(a));
            vit.vx = 0;
            vit.vy = - (vit.init_speed);
        }
    } else if (header->type ==  BinaryProtocol::MessageType::DOWN) {
        if (a != -1) {
            auto& vit = _mediator.GetComponent<Vitesse>(_logic.getEntity(a));
            vit.vx = 0;
            vit.vy = (vit.init_speed);
        }
    } else if (header->type ==  BinaryProtocol::MessageType::LEFT) {
        if (a != -1) {
            auto& vit = _mediator.GetComponent<Vitesse>(_logic.getEntity(a));
            vit.vx = -(vit.init_speed);
            vit.vy = 0;
        }
    } else if (header->type ==  BinaryProtocol::MessageType::RIGHT) {
        if (a != -1) {
            auto& vit = _mediator.GetComponent<Vitesse>(_logic.getEntity(a));
            vit.vx = (vit.init_speed);
            vit.vy = 0;
        }
    } else if (header->type ==  BinaryProtocol::MessageType::CLIENT_CLOSE) {
    } else if (header->type == BinaryProtocol::MessageType::SHOOT) {
        if (a != -1 &&  _logic.getEntity(a) != -1) {
            int type = -1;
            auto &bullet_type = _mediator.GetComponent<Bullet_type>(_logic.getEntity(a));

            type = bullet_type.type;

            // get the position of the player
            auto& pos = _mediator.GetComponent<Position>(_logic.getEntity(a));
            Entity projectile = _mediator.createEntity();
            auto & dmg = _mediator.GetComponent<Damage>(a);
            _logic.AddProjectile(projectile, (pos.x + 100 / 2), pos.y, _logic.getEntity(a), dmg.damage, type);

            std::vector<int> ids;
            ids.push_back(projectile);

            std::vector<int> posX{}; 
            std::vector<int> posY{};
            std::vector<int> types{};

            posX.push_back(pos.x);  
            posY.push_back(pos.y);
            types.push_back(type);

            //std::cout << "player " << a << " Shooted x = " << pos.x << " y = " << pos.y << std::endl;
            // std::vector<char> projectle_msg = BinaryProtocol::GetprojectileList(ids.data(), ids.size(), posX.data(), posY.data());

            //std::cout << "Player "  << a << " shooted\n";
            std::vector<char> projectile_msg = BinaryProtocol::GetprojectileList(ids.data(), ids.size(), posX.data(), posY.data(), types.data());
            Send_to_all_client(projectile_msg);
        }
    }  else if (header->type == BinaryProtocol::MessageType::CHARGE_ATTACK) {
        if (a != -1 && _logic.getEntity(a) != -1) {
            int type = 6;
    
            // get the position of the player
            auto& pos = _mediator.GetComponent<Position>(_logic.getEntity(a));
            Entity projectile = _mediator.createEntity();
            _logic.AddProjectile(projectile, (pos.x + 100 / 2), pos.y, _logic.getEntity(a), 100, type);

            std::vector<int> ids;
            ids.push_back(projectile);

            std::vector<int> posX{}; 
            std::vector<int> posY{};
            std::vector<int> types{};

            posX.push_back(pos.x);  
            posY.push_back(pos.y);
            types.push_back(type);

            std::vector<char> projectile_msg = BinaryProtocol::GetprojectileList(ids.data(), ids.size(), posX.data(), posY.data(), types.data());
            Send_to_all_client(projectile_msg);
        }
    }
}

void GameServer::start_game()
{
    level_start_time = std::chrono::steady_clock::now();
    start_receive();         // Gérer les messages UDP entrants
    start_update();
}

void GameServer::close_game()
{

}

/// @brief Function to init all the object for the game world
void GameServer::Init()
{
    std::ifstream json_file("Config/server_config.json");

    if (!json_file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        exit(1);
    }

    std::string json_str((std::istreambuf_iterator<char>(json_file)), std::istreambuf_iterator<char>());
    json_file.close();

    json json_data = json::parse(json_str);

    _players_init_health = json_data["players"]["health"].as<int>();

    _players_init_spedd = json_data["players"]["speed"].as<int>();

    player_dmg = json_data["players"]["damage"].as<int>();

    spawn1_frequency = json_data["game"]["spawn1_frequency"].as<float>();
    spawn2_frequency = json_data["game"]["spawn2_frequency"].as<float>();
    _mobs_init_health = json_data["players"]["health"].as<int>();
    _mobs_init_speed = json_data["enemys"]["speed"].as<int>();

    power_health = json_data["game"]["power_health"].as<int>();
    power_speed = json_data["game"]["power_speed"].as<int>();
    power_damage = json_data["game"]["power_damage"].as<int>();

    _score_multipliyer = json_data["game"]["score_mulipliyer"].as<int>();

    _mediator.RegisterComponent<Position>();
    _mediator.RegisterComponent<AI>();
    _mediator.RegisterComponent<Vitesse>();
    _mediator.RegisterComponent<Shoot>();
    _mediator.RegisterComponent<Collision>();
    _mediator.RegisterComponent<Script>();
    _mediator.RegisterComponent<Health>();
    _mediator.RegisterComponent<Damage>();
    _mediator.RegisterComponent<Power>();
    _mediator.RegisterComponent<Bullet_type>();
    _mediator.RegisterComponent<Boss>();
    

    _movement_system = _mediator.RegisterSystem<MovementSystem>();
    collision_system = _mediator.RegisterSystem<CollisionSystem>();
    enemy_movement_system = _mediator.RegisterSystem<EnemySystem>();
    projectile_system = _mediator.RegisterSystem<ProjectileSystem>();
    _obstacle_system = _mediator.RegisterSystem<ObstacleSystem>();
    collectable_system =  _mediator.RegisterSystem<CollectableSystem>();
    enemyBullet_system = _mediator.RegisterSystem<Enemy_Bullet_System>();
    boss_system = _mediator.RegisterSystem<BossSystem>();

    //ennemy_bullet_system = _mediator.RegisterSystem<EnemyBulletSystem>();

    obstacleignature.set(_mediator.GetComponentType<Position>());
    obstacleignature.set(_mediator.GetComponentType<Collision>());

    //collectableSignature.set(_mediator.GetComponentType<Collision>());

    mvSignature.set(_mediator.GetComponentType<Position>());
    mvSignature.set(_mediator.GetComponentType<Vitesse>());

    collisionSignature.set(_mediator.GetComponentType<Collision>());

    enemybulletSignature.set(_mediator.GetComponentType<Position>());
    enemybulletSignature.set(_mediator.GetComponentType<Bullet_type>());
    enemybulletSignature.set(_mediator.GetComponentType<Script>());

    //collisionSignature.set(_mediator.GetComponentType<Health>());
    //collisionSignature.set(_mediator.GetComponentType<Damage>());
    //collisionSignature.set(_mediator.GetComponentType<Position>());

    enemySignature.set(_mediator.GetComponentType<Position>());
    enemySignature.set(_mediator.GetComponentType<AI>());
    enemySignature.set(_mediator.GetComponentType<Script>());
    enemySignature.set(_mediator.GetComponentType<Health>());
    // add the projectile to enable the enemies to shoot

    projectileSignature.set(_mediator.GetComponentType<Position>());
    projectileSignature.set(_mediator.GetComponentType<Shoot>());
    projectileSignature.set(_mediator.GetComponentType<Damage>());

    collectableSignature.set(_mediator.GetComponentType<Position>());
    collectableSignature.set(_mediator.GetComponentType<Power>());


    BossSignature.set(_mediator.GetComponentType<Position>());
    BossSignature.set(_mediator.GetComponentType<Script>());
    //BossSignature.set(_mediator.GetComponentType<Health>());
    BossSignature.set(_mediator.GetComponentType<Boss>());
    
    _mediator.SetSystemSignature<MovementSystem>(mvSignature);
    _mediator.SetSystemSignature<ObstacleSystem>(obstacleignature);
    _mediator.SetSystemSignature<EnemySystem>(enemySignature);
    _mediator.SetSystemSignature<ProjectileSystem>(projectileSignature);
    _mediator.SetSystemSignature<CollectableSystem>(collectableSignature);
    _mediator.SetSystemSignature<CollisionSystem>(collisionSignature);
    _mediator.SetSystemSignature<Enemy_Bullet_System>(enemybulletSignature);
    _mediator.SetSystemSignature<BossSystem>(BossSignature);

    // load lua script
    if (!_clients.empty()) {
        for (int i = 0; i < _clients.size(); i++) {
            //std::cout << "Client IP == " << _clients[i].getIp() << " ID == " << _clients[i].getID() << " " << _clients[i].getPort() << std::endl;
            _logic.AddPlayer(_clients[i].getID(), _clients[i].get_PosX(), _clients[i].get_PosY(), _players_init_health, player_dmg, _players_init_spedd);
        }
    }

    // load lua script 
    if (!levelScript.loadScript("scripts/game/game_content.lua")) {
        std::cerr << "Failed to load level script!" << std::endl;
        return;
    }
    // Initialiser le niveau
    levelScript.executeFunction("init_level", 1);

    levelScript.executeFunction("set_level", current_level);
}

int GameServer::GetClientId(udp::endpoint endpoint)
{
    for (int i = 0; i < _clients.size(); i++) {
        if (_clients[i].getIp() == _sender_endpoint.address().to_string() && _sender_endpoint.port() == _clients[i].getPort()) {
            return _clients[i].getID();
        }
    }
    return -1;
}

void GameServer::send_udp_message(udp::endpoint endpoint, std::vector<char> message)
{
    try {
        std::string ip = endpoint.address().to_string();
        int port = endpoint.port();
        _socket.async_send_to(boost::asio::buffer(message), endpoint,
            [this, message, ip, port](std::error_code ec, std::size_t bytes_sent) {
                if (!ec) {
                    //std::cout << "Message sent" << " (" << bytes_sent << " bytes) to address " << ip  << " on port " << port << std::endl;
                } else {
                    std::cerr << "Failed to send : " << ec.message() << std::endl;
                }
        });
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

std::pair<int, int> GameServer::generateUniquePosition(std::set<std::pair<int, int>>& existingPositions, int xMin, int xMax, int yMin, int yMax)
{
    std::pair<int, int> position;
    do {
        position.first = rand() % (xMax - xMin + 1) + xMin;
        position.second = rand() % (yMax - yMin + 1) + yMin;
    } while (existingPositions.find(position) != existingPositions.end());

    existingPositions.insert(position);
    return position;
}

void GameServer::Send_to_all_client(std::vector<char> msg)
{
    for (auto& client : _clients) {
        udp::endpoint client_endpoint(boost::asio::ip::make_address(client.getIp()), client.getPort());
        send_udp_message(client_endpoint, msg);
    }
}

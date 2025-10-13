/*
** EPITECH PROJECT, 2025
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** SoloPlay
*/

#include "../include/SoloPlay.hpp"

SoloPlay::SoloPlay(sf::RenderWindow & windows, Mediator & med) : _windows(windows), _mediator(med), _logic(med)
{
    //_mediator.Init();
    // Initialisation des composants ECS
    _mediator.RegisterComponent<Position>();
    _mediator.RegisterComponent<Sprite>();
    _mediator.RegisterComponent<Health>();
    _mediator.RegisterComponent<AnimationComponent>();
    _mediator.RegisterComponent<Boss>();   
    _mediator.RegisterComponent<AI>();
    _mediator.RegisterComponent<Vitesse>();
    _mediator.RegisterComponent<Shoot>();
    _mediator.RegisterComponent<Collision>();
    _mediator.RegisterComponent<Script>();
    _mediator.RegisterComponent<Health>();
    _mediator.RegisterComponent<Damage>();
    _mediator.RegisterComponent<Power>();
    _mediator.RegisterComponent<Bullet_type>();

    _render_system = _mediator.RegisterSystem<RenderSystem>();
    _animation_system = _mediator.RegisterSystem<AnimationSystem>();
    _movement_system = _mediator.RegisterSystem<MovementSystem>();
    collision_system = _mediator.RegisterSystem<CollisionSystem>();
    enemy_movement_system = _mediator.RegisterSystem<EnemySystem>();
    projectile_system = _mediator.RegisterSystem<ProjectileSystem>();
    _obstacle_system = _mediator.RegisterSystem<ObstacleSystem>();
    collectable_system =  _mediator.RegisterSystem<CollectableSystem>();
    enemyBullet_system = _mediator.RegisterSystem<Enemy_Bullet_System>();
    boss_system = _mediator.RegisterSystem<BossSystem>();

    _signatureDisplay.set(_mediator.GetComponentType<Position>());
    _signatureDisplay.set(_mediator.GetComponentType<Sprite>());

    _signatureAnimation.set(_mediator.GetComponentType<Sprite>());
    _signatureAnimation.set(_mediator.GetComponentType<AnimationComponent>());

    obstacleignature.set(_mediator.GetComponentType<Position>());
    obstacleignature.set(_mediator.GetComponentType<Collision>());

    //collectableSignature.set(_mediator.GetComponentType<Collision>());

    mvSignature.set(_mediator.GetComponentType<Position>());
    mvSignature.set(_mediator.GetComponentType<Vitesse>());
    mvSignature.set(_mediator.GetComponentType<Sprite>());

    collisionSignature.set(_mediator.GetComponentType<Collision>());

    enemybulletSignature.set(_mediator.GetComponentType<Position>());
    enemybulletSignature.set(_mediator.GetComponentType<Bullet_type>());
    enemybulletSignature.set(_mediator.GetComponentType<Script>());

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

    _mediator.SetSystemSignature<RenderSystem>(_signatureDisplay);
    _mediator.SetSystemSignature<AnimationSystem>(_signatureAnimation);
    _mediator.SetSystemSignature<MovementSystem>(mvSignature);
    _mediator.SetSystemSignature<ObstacleSystem>(obstacleignature);
    _mediator.SetSystemSignature<EnemySystem>(enemySignature);
    _mediator.SetSystemSignature<ProjectileSystem>(projectileSignature);
    _mediator.SetSystemSignature<CollectableSystem>(collectableSignature);
    _mediator.SetSystemSignature<CollisionSystem>(collisionSignature);
    _mediator.SetSystemSignature<Enemy_Bullet_System>(enemybulletSignature);
    _mediator.SetSystemSignature<BossSystem>(BossSignature);


    std::ifstream json_file("Config/server_config.json");

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
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    auto levels = json_data["levels"].as<std::vector<std::string>>();

    std::cout << "Niveaux disponibles :" << std::endl;
    for (const auto& level : levels) {
        _levels.push_back(level);
        std::cout << "- " << level << std::endl;
    }

    current_level = _levels[0];

    for (const auto& levelt : _levels) {
        std::cout << "Level Name: " << levelt << std::endl;
    }
    // load lua script 
    if (!levelScript.loadScript("scripts/game/game_content.lua")) {
        std::cerr << "Failed to load level script!" << std::endl;
        return;
    }
    // Initialiser le niveau
    levelScript.executeFunction("init_level", 1);
    _manager.Init();


    levelScript.executeFunction("set_level", current_level);
    level_start_time = std::chrono::steady_clock::now();

    _logic.AddPlayer(1, 500, 500, 100, 10, 50);
    _mediator.AddComponent(_logic.getEntity(1), _manager.getSkins()["skin1"]);

    soundManager.playGameMusic(current_level);
}

SoloPlay::~SoloPlay()
{
    std::cout << "Solo play ended\n";
}

void SoloPlay::handle_event(sf::Event event)
{
    static sf::Clock shootCooldownClock;
    const float shootCooldown = 0.2f;

    if (sf::Keyboard::isKeyPressed(keyMapDefault["Up"])) {
        auto& vit = _mediator.GetComponent<Vitesse>(_logic.getEntity(1));
        vit.vx = 0;
        vit.vy = - (vit.init_speed);
        std::cout << "Oui\n";
    }
    
    if (sf::Keyboard::isKeyPressed(keyMapDefault["Down"])) {
        auto& vit = _mediator.GetComponent<Vitesse>(_logic.getEntity(1));
        vit.vx = 0;
        vit.vy = (vit.init_speed);
    }
    
    if (sf::Keyboard::isKeyPressed(keyMapDefault["Left"])) {
        auto& vit = _mediator.GetComponent<Vitesse>(_logic.getEntity(1));
        vit.vx = -(vit.init_speed);
        vit.vy = 0;
    }
    
    if (sf::Keyboard::isKeyPressed(keyMapDefault["Right"])) {
        auto& vit = _mediator.GetComponent<Vitesse>(_logic.getEntity(1));
        vit.vx = (vit.init_speed);
        vit.vy = 0;
    }
    
    if (sf::Keyboard::isKeyPressed(keyMapDefault["Shoot"])) {
        if (shootCooldownClock.getElapsedTime().asSeconds() >= shootCooldown) {
            soundManager.playSound("shot");

            int type = -1;
            auto &bullet_type = _mediator.GetComponent<Bullet_type>(_logic.getEntity(1));
            type = bullet_type.type;

            // get the position of the player
            auto& pos = _mediator.GetComponent<Position>(_logic.getEntity(1));
            Entity projectile = _mediator.createEntity();
            auto & dmg = _mediator.GetComponent<Damage>(_logic.getEntity(1));
            _logic.AddProjectile(projectile, (pos.x + 100 / 2), pos.y, _logic.getEntity(1), dmg.damage, type);

            if (type == 0) {
                _mediator.AddComponent(projectile, _manager.getSpriteMap()["bullet"]);
            } if (type == 1) {
                _mediator.AddComponent(projectile, _manager.getSpriteMap()["bulletv2"]);
            } if (type == 2) {
                _mediator.AddComponent(projectile, _manager.getSpriteMap()["bulletv3"]);
            } if (type == 3) {
                _mediator.AddComponent(projectile, _manager.getSpriteMap()["bulletv4"]);
            } if (type == 4) {
                _mediator.AddComponent(projectile, _manager.getSpriteMap()["bulletv5"]);
            } if (type == 5) {
                _mediator.AddComponent(projectile, _manager.getSpriteMap()["bulletv6"]);
            }
            shootCooldownClock.restart();
        }
    }

    if (sf::Keyboard::isKeyPressed(keyMapDefault["Charge"])) {
            if (!isCharging) {
                // Commencer à charger
                chargeClock.restart();
                isCharging = true;
                soundManager.playSound("charge");
            }

            // Mise à jour de la barre de charge
            float chargeTime = chargeClock.getElapsedTime().asSeconds();
            float progress = std::min(chargeTime / chargeThreshold, 1.0f); // Limite à 1.0

            sf::Color dynamicColor = sf::Color(255 * (1 - progress), 255 * progress, 50); // Couleur dynamique
            chargeBar.setFillColor(dynamicColor);
            chargeBar.setSize(sf::Vector2f(chargeBarWidth * progress, chargeBarHeight));
        } else if (isCharging) {
            // Relâcher la touche de charge
            float chargeTime = chargeClock.getElapsedTime().asSeconds();
            isCharging = false;

            //soundManager.stopSong("charge"); // Stop le son de charge

            if (chargeTime >= chargeThreshold) {
                // Charge complète : attaque puissante
                soundManager.playSound("shot");


            } else {

            }
            // Réinitialiser la barre de charge
            chargeBar.setSize(sf::Vector2f(0, chargeBarHeight));
        }
}


/*
** EPITECH PROJECT, 2024
** test_network
** File description:
** protocol
*/

#include "../include/protocol.hpp"

// std::vector<char> BinaryProtocol::GetAuthMessage(std::string & message)
// {
//     MessageHeader header;
//     AuthMessage auth_data;

//     header.type = MessageType::AUTHENTICATION;
//     //header.length = s
//     header.length = sizeof(MessageHeader) + sizeof(AuthMessage);
//     header.compressed = 0;
//     auth_data.username_length = message.size();

//     // Combine header and data into a single buffer

//     std::cout << "Message len is " << message.size() << "\n";
//     strncpy(auth_data.username, message.c_str(), message.size());


//     std::cout << "After === " << auth_data.username << "\n";


//     std::vector<char> buffer(sizeof(header) + sizeof(auth_data));

//     memcpy(buffer.data(), &header, sizeof(header));
//     memcpy(buffer.data() + sizeof(header), &auth_data, sizeof(auth_data));

//     return buffer;
// }

std::vector<char> BinaryProtocol::GetAuthMessage(std::string& message)
{
    MessageHeader header;
    AuthMessage auth_data;

    header.type = MessageType::AUTHENTICATION;
    header.length = sizeof(MessageHeader) + sizeof(AuthMessage);
    header.compressed = 0;

    auth_data.username_length = message.size();

    // Initialiser auth_data.username avec des zéros
    memset(auth_data.username, 0, sizeof(auth_data.username));

    // Copier le message dans auth_data.username et null-terminer
    strncpy(auth_data.username, message.c_str(), sizeof(auth_data.username) - 1);

    // Afficher les informations pour débogage
    std::cout << "Message len is " << message.size() << "\n";
    std::cout << "After === " << auth_data.username << "\n";

    // Combiner header et data dans un seul buffer
    std::vector<char> buffer(sizeof(header) + sizeof(auth_data));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &auth_data, sizeof(auth_data));

    return buffer;
}


std::vector<char> BinaryProtocol::GetActionMessage(std::string& message)
{
    MessageHeader header;
    ActionMessage action_data;

    header.type = MessageType::GAME_ID;
    header.length = sizeof(ActionMessage);

    // Combine header and data into a single buffer
    strncpy(action_data.action, message.c_str(), sizeof(action_data.action));

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(action_data));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &action_data, sizeof(action_data));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());  // Données compressées

    return buffer;
}

std::vector<char> BinaryProtocol::GetStartMessage(std::string & message)
{
    MessageHeader header;
    SimpleMessage s_msg;

    header.type = MessageType::START;
    header.length = sizeof(SimpleMessage);

    // Combine header and data into a single buffer
    strncpy(s_msg.msg, message.c_str(), sizeof(s_msg.msg));
    std::vector<char> buffer(sizeof(header) + sizeof(s_msg));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &s_msg, sizeof(s_msg));

    return buffer;
}

std::vector<char> BinaryProtocol::GetStartFailureMessage(std::string & message)
{
    MessageHeader header;
    SimpleMessage s_msg;

    header.type = MessageType::START_FAILURE;
    header.length = sizeof(SimpleMessage);

    // Combine header and data into a single buffer
    strncpy(s_msg.msg, message.c_str(), sizeof(s_msg.msg));
    std::vector<char> buffer(sizeof(header) + sizeof(s_msg));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &s_msg, sizeof(s_msg));

    return buffer;
}

std::vector<char> BinaryProtocol::GetStartSuccessMessage(std::string & message)
{
    MessageHeader header;
    SimpleMessage s_msg;

    header.type = MessageType::START_SUCCESS;
    header.length = sizeof(SimpleMessage);

    // Combine header and data into a single buffer
    strncpy(s_msg.msg, message.c_str(), sizeof(s_msg.msg));
    std::vector<char> buffer(sizeof(header) + sizeof(s_msg));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &s_msg, sizeof(s_msg));

    return buffer;
}

std::vector<char> BinaryProtocol::GetAuthSuccessMessage(std::string& message, int id, int x, int y)
{
    MessageHeader header;
    Auth s_msg;

    header.type = MessageType::AUTHENTICATION_SUCCESS;
    header.length = sizeof(AuthMessage);

    // Combine header and data into a single buffer
    strncpy(s_msg.msg, message.c_str(), sizeof(s_msg.msg));
    s_msg.id = id;
    s_msg.posX = x;
    s_msg.posY = y;

    std::vector<char> buffer(sizeof(header) + sizeof(s_msg));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &s_msg, sizeof(s_msg));

    return buffer;
}

std::vector<char> BinaryProtocol::GetGameDataMessage(std::string& message)
{
    MessageHeader header;
    SimpleMessage s_msg;

    header.type = MessageType::GAME_DATA;
    header.length = sizeof(SimpleMessage);

    // Combine header and data into a single buffer
    strncpy(s_msg.msg, message.c_str(), sizeof(s_msg.msg));
    std::vector<char> buffer(sizeof(header) + sizeof(s_msg));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &s_msg, sizeof(s_msg));

    return buffer;    
}

std::vector<char> BinaryProtocol::GetNewClientMessage(std::string& message, int id, int x, int y)
{
    MessageHeader header;
    NewClient s_msg;

    header.type = MessageType::NEW_CLIENT;
    header.length = sizeof(NewClient);

    // Combine header and data into a single buffer
    strncpy(s_msg.name, message.c_str(), sizeof(s_msg.name));
    s_msg.id = id;
    s_msg.posX = x;
    s_msg.posY = y;

    std::vector<char> buffer(sizeof(header) + sizeof(s_msg));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &s_msg, sizeof(s_msg));

    return buffer;
}

std::vector<char> BinaryProtocol::GetNewClientListMessage(int* ids, int nbclient, std::vector<std::pair<int, int>> positions)
{
    MessageHeader header;
    Client_list_t list;

    header.type = MessageType::CLIENT_LIST;

    list.clients_length = sizeof(list.ids);
    list.nb_client = nbclient;

    for (int i = 0; i < 10; i++) {
        list.ids[i] = 0;
        list.posX[i] = 0;
        list.posY[i] = 0;
    }

    for (int i = 0; i < nbclient && i < 10; i++) {
        list.ids[i] = ids[i];
        list.posX[i] = positions[i].first;
        list.posY[i] = positions[i].second;
    }

    header.length = sizeof(list.clients_length) + sizeof(list.nb_client) + sizeof(list.ids) + sizeof(list.posX) + sizeof(list.posY);
    std::vector<char> buffer(sizeof(header) + header.length);

    std::memcpy(buffer.data(), &header, sizeof(header));

    std::memcpy(buffer.data() + sizeof(header), &list.clients_length, sizeof(list.clients_length));
    std::memcpy(buffer.data() + sizeof(header) + sizeof(list.clients_length), &list.nb_client, sizeof(list.nb_client));
    std::memcpy(buffer.data() + sizeof(header) + sizeof(list.clients_length) + sizeof(list.nb_client), list.ids, sizeof(list.ids));
    std::memcpy(buffer.data() + sizeof(header) + sizeof(list.clients_length) + sizeof(list.nb_client) + sizeof(list.ids), list.posX, sizeof(list.posX));
    std::memcpy(buffer.data() + sizeof(header) + sizeof(list.clients_length) + sizeof(list.nb_client) + sizeof(list.ids) + sizeof(list.posX), list.posY, sizeof(list.posY));
    return buffer;
}

std::vector<char> BinaryProtocol::GetClienClosetMessage(int id)
{
    MessageHeader header;
    Quit_client s_msg;

    header.type = MessageType::CLIENT_QUIT;
    header.length = sizeof(Quit_client);
    s_msg.id = id;
         
    std::vector<char> buffer(sizeof(header) + sizeof(s_msg));
    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &s_msg, sizeof(s_msg));

    return buffer;    
}

std::vector<char> BinaryProtocol::GetLeftMesaage(int id)
{
    MessageHeader header;

    Movement s_msg;

    header.type = MessageType::LEFT;
    header.length = sizeof(Movement);

    s_msg.id = id;

    std::vector<char> buffer(sizeof(header) + sizeof(s_msg));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &s_msg, sizeof(s_msg));

    return buffer;
}

std::vector<char> BinaryProtocol::GetRightMesaage(int id)
{
    MessageHeader header;

    Movement s_msg;

    header.type = MessageType::RIGHT;
    header.length = sizeof(Movement);

    s_msg.id = id;

    std::vector<char> buffer(sizeof(header) + sizeof(s_msg));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &s_msg, sizeof(s_msg));

    return buffer;
}

std::vector<char> BinaryProtocol::GetUPMesaage(int id)
{
    MessageHeader header;

    Movement s_msg;

    header.type = MessageType::UP;
    header.length = sizeof(Movement);

    s_msg.id = id;

    std::vector<char> buffer(sizeof(header) + sizeof(s_msg));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &s_msg, sizeof(s_msg));

    return buffer;
}

std::vector<char> BinaryProtocol::GetDownMesaage(int id)
{
    MessageHeader header;

    Movement s_msg;

    header.type = MessageType::DOWN;
    header.length = sizeof(Movement);

    s_msg.id = id;

    std::vector<char> buffer(sizeof(header) + sizeof(s_msg));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &s_msg, sizeof(s_msg));

    return buffer;
}

std::vector<char> BinaryProtocol::GetShootMesaage(int id)
{
    MessageHeader header;

    Movement s_msg;

    header.type = MessageType::SHOOT;
    header.length = sizeof(Movement);

    s_msg.id = id;

    std::vector<char> buffer(sizeof(header) + sizeof(s_msg));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &s_msg, sizeof(s_msg));
    return buffer;
}

std::vector<char> BinaryProtocol::GetCleintIDMessage(int id)
{
    MessageHeader header;
    Client_id s_msg;

    header.type = MessageType::CLIENT_ID;
    header.length = sizeof(Client_id);

    s_msg.id = id;

    std::vector<char> buffer(sizeof(header) + sizeof(s_msg));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &s_msg, sizeof(s_msg));

    return buffer;
}

std::vector<char> BinaryProtocol::GetClientUpdateAll(int *ids, int nb_client, int *x, int *y)
{
    MessageHeader header;
    Client_list_update list;

    header.type = MessageType::CLIENT_UPDATE;

    list.clients_length = sizeof(list.ids);
    list.nb_client = nb_client;

    for (int i = 0; i < 10; i++) {
        list.ids[i] = 0;
        list.posX[i] = 0;
        list.posY[i] = 0;
    }

    for (int i = 0; i < nb_client && i < 10; i++) {
        list.ids[i] = ids[i];
        list.posX[i] = x[i];
        list.posY[i] = y[i];
    }

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(list));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &list, sizeof(list));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());  // Données compressées

    return buffer;
}

std::vector<char>BinaryProtocol::GetEnemyListUpdate(int *ids, int nb_enemy, int *x, int *y)
{
    MessageHeader header;
    Enemy_list list;

    header.type = MessageType::ENEMY_SBIRES_UPDATE;

    list.enemy_length = nb_enemy;
    list.nb_client = nb_enemy;
    for (int i = 0; i < 100; i++) {
        list.ids[i] = 0;
        list.posX[i] = 0;
        list.posY[i] = 0;
    }

    for (int i = 0; i < nb_enemy && i < 100; i++) { 
        list.ids[i] = ids[i];
        list.posX[i] = x[i];
        list.posY[i] = y[i];
    }

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(list));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &list, sizeof(list));

    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;  // Longueur des données compressées
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());  // Données compressées
    return buffer;
}

std::vector<char> BinaryProtocol::GetprojectileList(int *ids, int nb_enemy, int *x, int *y, int *type)
{
    MessageHeader header;

    Shoot_info info;

    header.type = MessageType::SHOOT_CREATE;

    info.projectile_length = sizeof(info.id);
    info.nb_client = nb_enemy;

    for (int i = 0; i < 100; i++) {
        info.id[i] = 0;
        info.posX[i] = 0;
        info.posY[i] = 0;
        info.type[i] = 0;
    }

    for (int i = 0; i < nb_enemy && i < 100; i++) {
        info.id[i] = ids[i];
        info.posX[i] = x[i];
        info.posY[i] = y[i];
        info.type[i] = type[i];
    }

    //header.length = sizeof(info);

    // std::vector<char> buffer(sizeof(header) + header.length);
    // size_t offset = 0;
    // std::memcpy(buffer.data() + offset, &header, sizeof(header));
    // offset += sizeof(header);
    // std::memcpy(buffer.data() + offset, &info, sizeof(info));
    // return buffer;

     // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(info));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &info, sizeof(info));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());  // Données compressées

    return buffer;
}

std::vector<char> BinaryProtocol::GetprojectileListUpdate(int *ids, int nb_enemy, int *x, int *y, int *type)
{
    MessageHeader header;

    Shoot_info info;

    header.type = MessageType::SHOOT_UDPATE;

    info.projectile_length = sizeof(info.id);
    info.nb_client = nb_enemy;

    for (int i = 0; i < 100; i++) {
        info.id[i] = 0;
        info.posX[i] = 0;
        info.posY[i] = 0;
        info.type[i] = 0;
    }

    for (int i = 0; i < nb_enemy && i < 100; i++) {
        info.id[i] = ids[i];
        info.posX[i] = x[i];
        info.posY[i] = y[i];
        info.type[i] = type[i];
    }

    header.length = sizeof(info);

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(info));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &info, sizeof(info));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());  // Données compressées

    return buffer;
}


std::vector<char> BinaryProtocol::GetStartLevelMessage(std::string &msg)
{
    MessageHeader header;
    SimpleMessage auth_data;

    header.type = MessageType::START_LEVEL;
    header.length = sizeof(SimpleMessage);

    // Combine header and data into a single buffer
    strncpy(auth_data.msg, msg.c_str(), sizeof(auth_data.msg));
    std::vector<char> buffer(sizeof(header) + sizeof(auth_data));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &auth_data, sizeof(auth_data));

    return buffer;
}

std::vector<char> BinaryProtocol::GetEnemyCreate(int id, int nb_enemy, int x, int y, std::string & type)
{
    MessageHeader header;
    Enemy enemy;

    header.type = MessageType::SIMPLE_ENEMY_CREATE;
    header.length = sizeof(Enemy);

    enemy.ids = id;
    enemy.nb_client = nb_enemy;
    enemy.posX = x;
    enemy.posY = y;
    strncpy(enemy.type, type.c_str(), sizeof(enemy.type));

    header.length = sizeof(enemy);

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(enemy));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &enemy, sizeof(enemy));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());  // Données compressées

    return buffer;
}

std::vector<char> BinaryProtocol::GetEnemyDelete(int id)
{
    MessageHeader header;
    Enemy_delete enemy;

    header.type = MessageType::ENEMY_DELETE;
    header.length = sizeof(Enemy_delete);

    enemy.id = id;

    // std::vector<char> buffer(sizeof(header) + sizeof(enemy));

    // memcpy(buffer.data(), &header, sizeof(header));
    // memcpy(buffer.data() + sizeof(header), &enemy, sizeof(enemy));

    // return buffer;

     // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(enemy));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &enemy, sizeof(enemy));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());  // Données compressées

    return buffer;
}

std::vector<char> BinaryProtocol::GetScore(int score)
{
    MessageHeader header;
    Score sc;

    header.type = MessageType::SCORE;
    header.length = sizeof(Score);

    sc.score = score;

    std::vector<char> buffer(sizeof(header) + sizeof(score));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &score, sizeof(score));

    return buffer;
}

std::vector<char> BinaryProtocol::GetLevel(std::string level)
{
    MessageHeader header;
    header.compressed = 0;  // Indique que les données ne sont pas compressées
    SimpleMessage sc;

    header.type = MessageType::GET_LEVEL;
    header.length = sizeof(SimpleMessage);

    strncpy(sc.msg, level.c_str(), sizeof(sc.msg));

    std::vector<char> buffer(sizeof(header) + sizeof(sc));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &sc, sizeof(sc));

    return buffer;
}

std::vector<char> BinaryProtocol::GetObstacleCreate(int *id, int nb_obstacle, int *x, int *y, int type)
{
    if (!id || !x || !y || !type) {
        throw std::invalid_argument("Null pointer passed to GetObstacleCreate");
    }

    MessageHeader header;
    Obstacle info;

    header.type = MessageType::CREATE_OBS;
    info.ob_length = sizeof(Obstacle);
    info.nb_obstacle = nb_obstacle;

    for (int i = 0; i < 100; i++) {
        info.ids[i] = (i < nb_obstacle) ? id[i] : 0;
        info.posX[i] = (i < nb_obstacle) ? x[i] : 0;
        info.posY[i] = (i < nb_obstacle) ? y[i] : 0;
    }
    info.type = type;

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(info));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &info, sizeof(info));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());  // Données compressées

    return buffer;
}

std::vector<char> BinaryProtocol::GetPowerCreate(int id, int x, int y, std::string & type)
{
    MessageHeader header;
    Power_up_create info;
    header.type = MessageType::CREATE_POWER_UP;

    info.id = id;
    info.posX = x;
    info.posY = y;
    if (type == "Health")
        info.type = 1;
    else if (type == "Speed")
        info.type = 2;
    else if (type == "Damage")
        info.type = 3;
    else
        throw std::invalid_argument("Invalid type passed to GetPowerCreate");
    header.length = sizeof(info);

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(info));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &info, sizeof(info));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());

    return buffer;
}

std::vector<char> BinaryProtocol::GetPowerUpdate(int id, int x, int y)
{
    MessageHeader header;
    Power_up_update info;
    header.type = MessageType::UPDATE_POWER_UP;

    info.id = id;
    info.posX = x;
    info.posY = y;

    header.length = sizeof(info);

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(info));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &info, sizeof(info));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());  // Données compressées
    return buffer;
}

std::vector<char> BinaryProtocol::GetPlayerHealth(int *id, int *health, int nb_players)
{
    Players_health info;

    MessageHeader header;
    header.type = MessageType::PLAYER_HEALTH;

    info.nb_client = nb_players;

    for (int i = 0; i < 100; i++) {
        info.ids[i] = (i < nb_players) ? id[i] : 0;
        info.health[i] = (i < nb_players) ? health[i] : 0;
    }

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(info));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &info, sizeof(info));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());

    return buffer;
}


std::vector<char> BinaryProtocol::GetAllColllision(int *type, int *x, int *y, int nb_collison)
{
    All_collision info;

    MessageHeader header;
    header.type = MessageType::COLLISION;

    info.nb_collision = nb_collison;

    for (int i = 0; i < 100; i++) {
        info.type[i] = (i < nb_collison) ? type[i] : 0;
        info.x[i] = (i < nb_collison) ? x[i] : 0;
        info.y[i] = (i < nb_collison) ? y[i] : 0;
    }

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(info));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &info, sizeof(info));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());

    return buffer;
}

std::vector<char> BinaryProtocol::GetEnemyBulletCreate(int ids, int x, int y, int type, int nb_enemy)
{
    Protocol_Enemy_Bullet_create info;

    MessageHeader header;
    header.type = MessageType::ENEMY_BULLET_CREATE;

    info.nb_bullet = nb_enemy;
    info.ids = ids;
    info.type = type;
    info.x = x;
    info.y = y;

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(info));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &info, sizeof(info));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());

    return buffer;
}


std::vector<char> BinaryProtocol::GetEnemyBulletUpdate(int *ids, int *x, int *y, int nb_enemy)
{
    Protocol_Enemy_Bullet info;

    MessageHeader header;
    header.type = MessageType::ENEMY_BULLET_UPDATE;

    info.nb_bullet  = nb_enemy;

    for (int i = 0; i < 100; i++) {
        info.ids[i] = (i < nb_enemy) ? ids[i] : 0;
        info.type[i] = 0;
        info.x[i] = (i < nb_enemy) ? x[i] : 0;
        info.y[i] = (i < nb_enemy) ? y[i] : 0;
    }
    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(info));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &info, sizeof(info));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());

    return buffer;
}


std::vector<char> BinaryProtocol::GetBossCreate(int id, int x, int y, int nb_boss, std::string & type, int health)
{   
    BinaryProtocol::MessageHeader header;
    header.type = BinaryProtocol::MessageType::BOSS_CREATE;
    BinaryProtocol::Boss_create info;

    info.id = id;
    info.x = x;
    info.y = y;
    info.health = health;
    info.nb_boss = nb_boss;
    strncpy(info.type, type.c_str(), sizeof(info.type));

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(info));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &info, sizeof(info));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());

    return buffer;
}

std::vector<char> BinaryProtocol::GetBossUpdate(int id, int x, int y, int nb_boss, int health)
{
    BinaryProtocol::MessageHeader header;
    header.type = BinaryProtocol::MessageType::BOSS_UPDATE;
    BinaryProtocol::Boss_update info;

    info.id = id;
    info.x = x;
    info.y = y;
    info.health = health;
    // strncpy(info.type, type.c_str(), sizeof(info.type));

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(info));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &info, sizeof(info));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());

    return buffer;
}

std::vector<char> BinaryProtocol::GetBossIncomming(int id, int x, int y)
{
    BinaryProtocol::MessageHeader header;
    header.type = BinaryProtocol::MessageType::BOSS_INCOMMING;
    BinaryProtocol::Boss_incomming info;

    info.id = id;
    info.x = x;
    info.y = y;

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(info));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &info, sizeof(info));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());

    return buffer;
}

std::vector<char> BinaryProtocol::GetLevelTime(float time)
{
    BinaryProtocol::MessageHeader header;
    header.type = BinaryProtocol::MessageType::LEVEL_TIME;
    BinaryProtocol::Level_time info;
    header.compressed = 0;

    info.time = time;

    std::vector<char> uncompressed_data(sizeof(header) + sizeof(info));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &info, sizeof(info));
    return uncompressed_data;
}

std::vector<char> BinaryProtocol::GetBossBulletCreate(int ids, int x, int y, int type, int nb_enemy)
{
    Protocol_Enemy_Bullet_create info;

    MessageHeader header;
    header.type = MessageType::BOSS_SHOT;

    info.nb_bullet = nb_enemy;
    info.ids = ids;
    info.type = type;
    info.x = x;
    info.y = y;

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(info));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &info, sizeof(info));

    // Compression
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête en clair
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());

    return buffer;
}

std::vector<char> BinaryProtocol::GetChargeAttack(int id)
{
    MessageHeader header;

    Movement s_msg;

    header.type = MessageType::CHARGE_ATTACK;
    header.length = sizeof(Movement);

    s_msg.id = id;

    std::vector<char> buffer(sizeof(header) + sizeof(s_msg));

    memcpy(buffer.data(), &header, sizeof(header));
    memcpy(buffer.data() + sizeof(header), &s_msg, sizeof(s_msg));
    return buffer;
}

std::vector<char> BinaryProtocol::GetBossDeath(void)
{
    MessageHeader header;
    ActionMessage action_data;

    header.type = MessageType::BOSS_DEATH;
    header.length = sizeof(ActionMessage);
    header.compressed = 0;

    // Combine header and data into a single buffer
    strncpy(action_data.action, "finish", sizeof(action_data.action));

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(action_data));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &action_data, sizeof(action_data));


    return uncompressed_data;
}

std::vector<char> BinaryProtocol::GetTimer(float timer)
{
    BinaryProtocol::MessageHeader header;
    header.type = BinaryProtocol::MessageType::TIMER;
    BinaryProtocol::Level_time info;
    header.compressed = 0;

    info.time = timer;

    std::vector<char> uncompressed_data(sizeof(header) + sizeof(info));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &info, sizeof(info));
    return uncompressed_data;
}

std::vector<char> BinaryProtocol::NextLevel(void)
{
    MessageHeader header;
    ActionMessage action_data;

    header.type = MessageType::START_NEXT_LEVEL;
    header.length = sizeof(ActionMessage);

    // Combine header and data into a single buffer
    strncpy(action_data.action, "finish", sizeof(action_data.action));

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(action_data));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &action_data, sizeof(action_data));

    return uncompressed_data;
}

std::vector<char> BinaryProtocol::LevelInfo(std::string level, int current, int total)
{
    MessageHeader header;
    BinaryProtocol::NextLevel_Protocol action_data;

    header.type = MessageType::LEVEL_INFO;
    header.length = sizeof(NextLevel_Protocol);

    action_data.current = current;
    action_data.total = total;

    // Combine header and data into a single buffer
    strncpy(action_data.level, level.c_str(), sizeof(action_data.level));

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(action_data));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &action_data, sizeof(action_data));

    return uncompressed_data;
}

std::vector<char> BinaryProtocol::GameFinish(std::vector<std::pair<std::string, int>>& scoreboard)
{
    BinaryProtocol::Protocol_GameFinish info;

    // Remplir les données dans la structure
    info.playerCount = scoreboard.size();
    for (size_t i = 0; i < info.playerCount; ++i) {
        // Copier le nom du joueur (assurer un nom nul-terminé)
        std::strncpy(info.players[i].name, scoreboard[i].first.c_str(), sizeof(info.players[i].name) - 1);
        info.players[i].name[sizeof(info.players[i].name) - 1] = '\0';
        info.players[i].score = scoreboard[i].second;
    }

    // En-tête du message
    MessageHeader header;
    header.type = MessageType::GAME_FINISH;

    // Construire les données non compressées
    std::vector<char> uncompressed_data(sizeof(header) + sizeof(info));
    std::memcpy(uncompressed_data.data(), &header, sizeof(header));
    std::memcpy(uncompressed_data.data() + sizeof(header), &info, sizeof(info));

    // Compression des données
    uLong source_len = uncompressed_data.size();
    uLong dest_len = compressBound(source_len);
    std::vector<char> compressed_data(dest_len);

    if (compress(reinterpret_cast<Bytef*>(compressed_data.data()), &dest_len,
                 reinterpret_cast<const Bytef*>(uncompressed_data.data()), source_len) != Z_OK) {
        throw std::runtime_error("Compression failed");
    }

    compressed_data.resize(dest_len);
    header.length = dest_len;
    header.uncompressed_length = source_len;
    header.compressed = 1;

    // Construire le message final
    std::vector<char> buffer(sizeof(header) + compressed_data.size());
    std::memcpy(buffer.data(), &header, sizeof(header));  // En-tête
    std::memcpy(buffer.data() + sizeof(header), compressed_data.data(), compressed_data.size());

    return buffer;
}

/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** Core
*/

#include "../include/Core.hpp"

Core::Core(unsigned short server_port, std::string server_ip,  unsigned short lobby_port, std::string lobby_ip)
{
    _server_port = server_port;
    _server_ip = server_ip;
}

Core::~Core()
{
}

void Core::launch()
{
    io_context _io_context;
    io_context game_context;

    LobbyTCP lobby(_io_context, _server_ip, 12345, _server_ip, _server_port);

    std::thread lobby_thread([&]() {
        lobby.start_lobby();
        _io_context.run();
    });

    while (!lobby.is_game_server_ready()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    _io_context.stop();
    std::cout << "Lobby Action ended\n";
    lobby_thread.join();

    // Start the game server before stopping the lobby's io_context
    std::vector<ClientInfo> all = lobby.get_clients();

    Mediator med;
    med.Init();
    GameServer game_server(game_context, _server_ip, _server_port, all, med, lobby.GetLevels(), lobby.GetPlayers_uuids());

    std::thread game_thread([&]() {
        game_server.Init();
        game_server.start_game();
        game_context.run();
    });

    while (game_server.getGamefinish() != 1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    game_context.stop();
    std::cout << "Game server finish\n";
    _io_context.stop();
    game_thread.join();
}

void Core::restart()
{

}

void Core::launch_game_server(io_context& io_context, std::vector<ClientInfo> all, Level& level)
{
}

int main(int ac, char **av)
{
    try {
        if (ac != 3) {
            std::cout << "Invalid number of arguments\n";
            exit(84);
        }
        std::string server_ip = std::string(av[1]);
        int server_port = std::stoi(av[2]);
        Core _core(server_port, server_ip, 12345, server_ip);
        _core.launch();
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        exit(84);
    }
    
}

/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** main
*/

#include "../include/client.hpp"
#include "../include/Replay.hpp"

int main(int ac, char **av)
{
    try {
        if (ac == 3) {
            const std::string mode(av[1]);

            if (mode == "--replay") {
                // Mode Replay
                const std::string replay_dir(av[2]);

                Replay replay(replay_dir);
                replay.Init();
                replay.run();

            } else {
                const sf::IpAddress address(mode);
                char* end;
                unsigned long port = std::strtoul(av[2], &end, 0);

                // Vérification de la validité du port
                if (*end != '\0' || port > 65535) {
                    throw std::invalid_argument("Invalid port number");
                }

                Mediator mediator;
                mediator.Init();

                Client client(address, static_cast<unsigned short>(port), mediator);
                client.Init(mediator);
                client.run();
            }

        } else {
            std::cerr << "Invalid number of arguments.\n"
                      << "Usage:\n"
                      << "  ./rtype_client <ip> <port>\n"
                      << "  ./rtype_client --replay <path_to_directory>\n";
            return 84;
        }
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Argument error: " << e.what() << '\n';
        return 84;
    }
    catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << '\n';
        return 84;
    }
    return 0;
}


/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** Core
*/


// Core.hpp
#ifndef CORE_HPP_
    #define CORE_HPP_
    #include <iostream>
    #include "../../Server/include/network.hpp"

    /**
     * * @class Core
     * * @brief Brief description of Core.
     * */
    class Core {
        public:
            Core(unsigned short server_port, std::string server_ip,  unsigned short lobby_port, std::string lobby_ip);
            ~Core();
            void launch();
            void launch_game_server(io_context& io_context, std::vector<ClientInfo> all, Level & level);
            void restart();
        protected:
        private:
            unsigned short _server_port;
            std::string _server_ip;
            unsigned short _lobby_port;
            std::string _lobby_ip;
    };

#endif /* !CORE_HPP_ */

/*
** EPITECH PROJECT, 2024
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** Replay
*/

#ifndef REPLAY_HPP_
    #define REPLAY_HPP_
    #include <iostream>
    #include <ostream>
    #include <fstream>  
    #include <SFML/Graphics.hpp>
    #include <boost/filesystem.hpp>
    #include <map>

    namespace fs = boost::filesystem;

    class Replay {
        public:
            Replay(std::string replay_dir);
            ~Replay();
            void Init();
            void run();
            void loadFramesInCache(int start_frame);
            void clearCache();            

        protected:
        private:
            std::string _replay_dir;
            sf::Clock clock;
            sf::Event _event;
            sf::RenderWindow _window;
            std::map<std::string, sf::Sprite> _replay_sprites;
            std::map<int, sf::Texture> _frame_textures; // Cache des textures chargées
            std::map<int, sf::Sprite> _frame_sprites;  // Sprites correspondant aux textures
            int _current_frame = 0;                    // Frame en cours d'affichage
            int _total_frames = 0;                     // Nombre total de frames
            int _cache_size = 10;  
    };

#endif /* !REPLAY_HPP_ */

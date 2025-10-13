/*
** EPITECH PROJECT, 2025
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** client_replay
*/

#include "../include/client.hpp"

std::string Client::generate_filename(int frame_number)
{
    // Créer le dossier "replays" s'il n'existe pas
    boost::filesystem::path replays_dir("replays");
    if (!boost::filesystem::exists(replays_dir)) {
        boost::filesystem::create_directory(replays_dir);
    }

    // Initialiser game_replay_dir si vide
    if (game_replay_dir.empty()) {
        game_replay_dir = "replays/" + _game_session;

        std::cout << _game_session;
        std::cout << game_replay_dir;
    }

    // Créer le dossier spécifique à la session de jeu
    boost::filesystem::path session_dir(game_replay_dir);
    if (!boost::filesystem::exists(session_dir)) {
        boost::filesystem::create_directory(session_dir);
    }

    // Générer le nom de fichier pour le frame
    std::string game_session_frame = game_replay_dir + "/frame_" + std::to_string(frame_number) + ".png";
    return game_session_frame;
}

void Client::capture_frame(const std::string& filename)
{
    sf::Vector2u windowSize = _window.getSize();
    sf::Texture texture;
    
    texture.create(windowSize.x, windowSize.y);
    texture.update(_window);

    sf::Image screenshot = texture.copyToImage();

    if (!screenshot.saveToFile(filename)) {
        std::cerr << "Error: Could not save screenshot to " << filename << std::endl;
    } else {
        std::cout << "Screenshot saved to " << filename << std::endl;
    }
}

void Client::save_frames_to_files()
{
    for (size_t i = 0; i < captured_frames.size(); ++i) {
        std::string filename = generate_filename(saved_frame);
        if (!captured_frames[i].saveToFile(filename)) {
            std::cerr << "Error: Could not save frame " << i << " to file." << std::endl;
        }
        saved_frame++;
    }
    captured_frames.clear();
}



void Client::start_recording()
{
    is_recording = true;
    record_thread = std::thread(&Client::process_recording, this); // Sauvegarde dans un thread séparé
    std::cout << "Recording started in a separate thread." << std::endl;
}

void Client::stop_recording()
{
    is_recording = false;
    condition.notify_all(); // Réveille le thread de sauvegarde
    if (record_thread.joinable()) {
        record_thread.join(); // Attendre la fin de la sauvegarde
    }
    //std::cout << "Recording stopped." << std::endl;
}

void Client::record_session()
{
    static sf::Clock clock;
    int frame_interval_ms = 1000 / 20;

    if (clock.getElapsedTime().asMilliseconds() >= frame_interval_ms) {
        sf::Texture texture;
        texture.create(_window.getSize().x, _window.getSize().y);
        texture.update(_window);
        sf::Image screenshot = texture.copyToImage(); // Capture de l'écran
        clock.restart();

        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            if (frame_queue.size() < 100) {
                frame_queue.push(screenshot);
                condition.notify_one(); // Réveille le thread de sauvegarde
            } else {
                std::cerr << "Warning: Frame dropped due to full queue." << std::endl;
            }
        }
    }
}

void Client::process_recording()
{
    while (is_recording || !frame_queue.empty()) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        condition.wait(lock, [this]() { return !frame_queue.empty() || !is_recording; });

        while (!frame_queue.empty()) {
            sf::Image frame = frame_queue.front();
            frame_queue.pop();
            lock.unlock();

            // Sauvegarder sur disque
            std::string filename = generate_filename(saved_frame++);
            if (!frame.saveToFile(filename)) {
                std::cerr << "Error: Could not save frame to file: " << filename << std::endl;
            } else {
                std::cout << "Saved frame: " << filename << std::endl;
            }

            lock.lock();
        }
    }
}

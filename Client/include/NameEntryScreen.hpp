/*
** EPITECH PROJECT, 2025
** B-CPP-500-COT-5-1-rtype-jaures.agossou
** File description:
** NameEntryScreen
*/

#ifndef NAMEENTRYSCREEN_HPP_
    #define NAMEENTRYSCREEN_HPP_
    // NameEntryScreen.hpp
    #pragma once
    #include <SFML/Graphics.hpp>
    #include <string>
    #include <iostream>

    class NameEntryScreen {
    public:
        NameEntryScreen(sf::RenderWindow& window);
        bool handleEvent(sf::Event event);
        void update();
        void draw();
        bool isFinished() const;
        std::string getPlayerName() const;

    private:
        sf::RenderWindow& window;
        sf::Font font;
        sf::Text titleText;
        sf::Text inputText;
        sf::RectangleShape inputBox;
        sf::Text instructionText;
        sf::RectangleShape continueButton;
        sf::Text continueText;
        sf::Sprite back;
        sf::Texture texture;
        
        std::string playerName;
        bool isComplete;
        
        void initializeComponents();
        void updateInputBox();
        bool isValidName() const;
    };
#endif /* !NAMEENTRYSCREEN_HPP_ */

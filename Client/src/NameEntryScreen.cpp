
#include "../include/NameEntryScreen.hpp"

NameEntryScreen::NameEntryScreen(sf::RenderWindow& win) 
    : window(win), isComplete(false)
{
    initializeComponents();
}

void NameEntryScreen::initializeComponents() {
    // Chargement de la police
    font.loadFromFile("Client/ressources/INVASION2000.TTF"); // Ajustez le chemin selon votre structure

    // Configuration du titre
    titleText.setFont(font);
    titleText.setString("Enter Your Name");
    titleText.setCharacterSize(40);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(
        (window.getSize().x - titleText.getGlobalBounds().width) / 2.f,
        100.f
    );

    // Configuration de la zone de texte
    inputBox.setSize(sf::Vector2f(400.f, 50.f));
    inputBox.setFillColor(sf::Color(50, 50, 50));
    inputBox.setOutlineThickness(2.f);
    inputBox.setOutlineColor(sf::Color::White);
    inputBox.setPosition(
        (window.getSize().x - inputBox.getSize().x) / 2.f,
        200.f
    );

    // Configuration du texte de saisie
    inputText.setFont(font);
    inputText.setCharacterSize(30);
    inputText.setFillColor(sf::Color::White);
    updateInputBox();

    // Configuration des instructions
    instructionText.setFont(font);
    instructionText.setString("Enter at least 3 characters and press Enter");
    instructionText.setCharacterSize(20);
    instructionText.setFillColor(sf::Color(200, 200, 200));
    instructionText.setPosition(
        (window.getSize().x - instructionText.getGlobalBounds().width) / 2.f,
        260.f
    );

    // Configuration du bouton Continue
    continueButton.setSize(sf::Vector2f(200.f, 50.f));
    continueButton.setFillColor(sf::Color(100, 100, 100));
    continueButton.setPosition(
        (window.getSize().x - continueButton.getSize().x) / 2.f,
        350.f
    );

    continueText.setFont(font);
    continueText.setString("Continue");
    continueText.setCharacterSize(24);
    continueText.setFillColor(sf::Color::White);
    continueText.setPosition(
        continueButton.getPosition().x + (continueButton.getSize().x - continueText.getGlobalBounds().width) / 2.f,
        continueButton.getPosition().y + (continueButton.getSize().y - continueText.getGlobalBounds().height) / 2.f
    );
    
    if (!texture.loadFromFile("Client/ressources/game_background.png")) {
        std::cout << "Cannot load" << "Client/ressources/game_background.png" << std::endl;
        exit(0);
    }
    back.setTexture(texture);
}

void NameEntryScreen::updateInputBox() {
    inputText.setString(playerName + "_");
    inputText.setPosition(
        inputBox.getPosition().x + 10.f,
        inputBox.getPosition().y + (inputBox.getSize().y - inputText.getGlobalBounds().height) / 2.f
    );
}

bool NameEntryScreen::handleEvent(sf::Event event) {
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 8) {  // Backspace
            if (!playerName.empty()) {
                playerName.pop_back();
            }
        }
        else if (event.text.unicode == 13) {  // Enter
            if (isValidName()) {
                isComplete = true;
            }
        }
        else if (event.text.unicode < 128 && playerName.length() < 15) {
            playerName += static_cast<char>(event.text.unicode);
        }
        updateInputBox();
    }
    else if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (continueButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            if (isValidName()) {
                isComplete = true;
            }
        }
    }

    return false;
}

void NameEntryScreen::update() {
    // Mise à jour de l'apparence du bouton
    if (isValidName()) {
        continueButton.setFillColor(sf::Color(0, 150, 0));
    } else {
        continueButton.setFillColor(sf::Color(100, 100, 100));
    }
}

void NameEntryScreen::draw() {
    window.draw(back);
    window.draw(titleText);
    window.draw(inputBox);
    window.draw(inputText);
    window.draw(instructionText);
    window.draw(continueButton);
    window.draw(continueText);
}

bool NameEntryScreen::isValidName() const {
    return playerName.length() >= 3;
}

bool NameEntryScreen::isFinished() const {
    return isComplete;
}

std::string NameEntryScreen::getPlayerName() const {
    return playerName;
}

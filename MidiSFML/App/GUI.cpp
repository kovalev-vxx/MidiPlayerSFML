//
//  GUI.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 16.06.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "GUI.hpp"

GUI::GUI(MidiPlayer& player, sf::RenderWindow& window, sf::Font& font){
    _player = &player;
    _window = &window;
    _playHovered = false;
    _backHovered = false;

    _title.setFont(font);
    _title.setString(_player->getSong().getTitle());
    _title.setPosition(25, _window->getSize().y - 100);
    _title.setCharacterSize(35);

    _timer.setFont(font);
    _timer.setPosition(25, 100);
    _timer.setCharacterSize(35);

    // Back button (top-left)
    _backBtn.setSize(sf::Vector2f(160, 50));
    _backBtn.setPosition(25, 25);
    _backBtn.setFillColor(sf::Color(50, 50, 50));

    _backBtnText.setFont(font);
    _backBtnText.setCharacterSize(26);
    _backBtnText.setFillColor(sf::Color::White);
    _backBtnText.setString("< MENU");

    // Play/Pause button (next to back)
    _playBtn.setSize(sf::Vector2f(160, 50));
    _playBtn.setPosition(200, 25);
    _playBtn.setFillColor(sf::Color(50, 50, 50));

    _playBtnText.setFont(font);
    _playBtnText.setCharacterSize(26);
    _playBtnText.setFillColor(sf::Color::White);
    _playBtnText.setString("PLAY");
};

void GUI::updateButtonColors() {
    _playBtn.setFillColor(_playHovered ? sf::Color(80, 80, 80) : sf::Color(50, 50, 50));
    _backBtn.setFillColor(_backHovered ? sf::Color(80, 80, 80) : sf::Color(50, 50, 50));
}

GUIAction GUI::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f pos((float)event.mouseMove.x, (float)event.mouseMove.y);
        _playHovered = _playBtn.getGlobalBounds().contains(pos);
        _backHovered = _backBtn.getGlobalBounds().contains(pos);
        updateButtonColors();
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f pos((float)event.mouseButton.x, (float)event.mouseButton.y);
        if (_playBtn.getGlobalBounds().contains(pos)) {
            return GUIAction::TOGGLE_PLAY;
        }
        if (_backBtn.getGlobalBounds().contains(pos)) {
            return GUIAction::BACK;
        }
    }
    return GUIAction::NONE;
}

void GUI::draw(){
    _seconds = _player->getPlayingOffset().asMilliseconds()/1000;
    _timer.setString(std::to_string(_seconds) + " SEC");

    if(_player->getStatus() == sf::SoundStream::Playing){
        _playBtnText.setString("PAUSE");
    } else {
        _playBtnText.setString("PLAY");
    }

    // Center button text
    _backBtnText.setPosition(
        _backBtn.getPosition().x + (_backBtn.getSize().x - _backBtnText.getLocalBounds().width) / 2.f,
        _backBtn.getPosition().y + 10
    );
    _playBtnText.setPosition(
        _playBtn.getPosition().x + (_playBtn.getSize().x - _playBtnText.getLocalBounds().width) / 2.f,
        _playBtn.getPosition().y + 10
    );

    _window->draw(_backBtn);
    _window->draw(_backBtnText);
    _window->draw(_playBtn);
    _window->draw(_playBtnText);
    _window->draw(_timer);
    _window->draw(_title);
};

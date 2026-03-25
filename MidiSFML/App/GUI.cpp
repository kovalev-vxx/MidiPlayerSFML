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
    _title.setFont(font);
    _title.setString(_player->getSong().getTitle());
    _title.setPosition(25, _window->getSize().y-100);
    _title.setCharacterSize(35);
    
    _timer.setFont(font);
    _timer.setPosition(25, 100);
    _timer.setCharacterSize(35);
    
    _status.setFont(font);
    _status.setPosition(25, 150);
    _status.setCharacterSize(35);
    _status.setString("PAUSE");
};

void GUI::draw(){
    _seconds = _player->getPlayingOffset().asMilliseconds()/1000;
    _timer.setString(std::to_string(_seconds) + " SEC");
    if(_player->getStatus() == sf::SoundStream::Playing){
        _status.setString("PLAY");
    } else {
        _status.setString("PAUSE");
    }
    _window->draw(_timer);
    _window->draw(_title);
    _window->draw(_status);
};



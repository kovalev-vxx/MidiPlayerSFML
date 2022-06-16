//
//  GUI.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 16.06.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef GUI_hpp
#define GUI_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "../MidiPlayer/MidiPlayer.hpp"
#include <string>
#include <iostream>

class GUI{
private:
    sf::RenderWindow* _window;
    MidiPlayer* _player;
    int _minutes;
    int _seconds;
    sf::Text _title;
    sf::Text _timer;
    sf:: Text _status;
    
public:
    GUI(MidiPlayer& player, sf::RenderWindow& window, sf::Font& font);
    void draw();
};

#endif /* GUI_hpp */

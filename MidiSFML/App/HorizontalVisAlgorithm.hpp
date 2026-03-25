//
//  HorizontalVisAlgorithm.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 09.06.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef HorizontalVisAlgorithm_hpp
#define HorizontalVisAlgorithm_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include "../MidiPlayer/Song.hpp"
#include "../MidiPlayer/MidiPlayer.hpp"
#include "IVisAlgorithm.hpp"
#include "sfMidi.h"


class HorizontalVisAlgorithm: public IVisAlgorithm{
private:
    std::vector<sf::RectangleShape> _rects;
    sf::RenderWindow* _window;
    MidiPlayer* _player;
    int _nowTime;
    int _lastTime;
    int _dt;
    int _start;
    sf::RectangleShape _separator;
    sf::RectangleShape _blendRect;
public:
    HorizontalVisAlgorithm(MidiPlayer& player, sf::RenderWindow& window);
    void draw() override;
};

#endif /* HorizontalVisAlgorithm_hpp */

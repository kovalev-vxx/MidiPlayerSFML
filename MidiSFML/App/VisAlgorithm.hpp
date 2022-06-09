//
//  VisAlgorithm.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 09.06.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef VisAlgorithm_hpp
#define VisAlgorithm_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include "../MidiPlayer/Song.hpp"


class VisAlgorith{
private:
    std::vector<sf::RectangleShape> _rects;
    int _nowTime;
    int _lastTime;
    int _dt;
    int _start;
public:
    VisAlgorith(Song& song, int windowWidth, int windowHeight);
    std::vector<sf::RectangleShape> updateRects(int offset);
    std::vector<sf::RectangleShape> getRects();
};

#endif /* VisAlgorithm_hpp */

//
//  Song.hpp
//  termSFML
//
//  Created by Valery Kovalev on 12.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef Song_hpp
#define Song_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include "SongLine.hpp"
#include <string>

class Song{
private:
    std::vector<SongLine> _songLines;
    int _tempo;
    std::string _title;
    double _volume;
public:
    Song(std::vector<SongLine> songLines, int tempo, std::string title, double volume);
    Song(std::vector<SongLine> songLines, int tempo, double volume);
    Song(std::vector<SongLine> songLines, int tempo);
    std::vector<SongLine> getLines();
    int getTempo();
    std::string getTitle();
    double getVolume();
    void changeVolume(double value);
};


#endif /* Song_hpp */

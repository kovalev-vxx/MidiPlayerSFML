//
//  Song.cpp
//  termSFML
//
//  Created by Valery Kovalev on 12.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "Song.hpp"


Song::Song(std::vector<SongLine> songLines, std::string title, double volume){
    _songLines = songLines;
    _title = title;
    if(volume<0 | volume>1){
        throw std::invalid_argument("The volume must be between 0 and 1");
    }
    _volume = volume;
}

Song::Song(std::vector<SongLine> songLines, double volume){
    _songLines = songLines;
    _title = "Untitle";
    if(volume<0 | volume>1){
        throw std::invalid_argument("The volume must be between 0 and 1");
    }
    _volume = volume;
}

Song::Song(std::vector<SongLine> songLines, std::string title){
    _songLines = songLines;
    _title = title;
    _volume = 1;
}

Song::Song(std::vector<SongLine> songLines){
    _songLines = songLines;
    _title = "Untitle";
    _volume = 1;
}

std::vector<SongLine> Song::getLines(){
    return _songLines;
}

std::string Song::getTitle(){
    return _title;
}

double Song::getVolume(){
    return _volume;
}

void Song::changeVolume(double value){
    if(value<0 | value>1){
        throw std::invalid_argument("The volume must be between 0 and 1");
    }
    _volume = value;
}

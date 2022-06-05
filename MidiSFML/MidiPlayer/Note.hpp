//
//  note.hpp
//  termSFML
//
//  Created by Valery Kovalev on 11.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef mNote_hpp
#define mNote_hpp

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <map>
#include <string>

class Note{
private:
    int _midiValue;
    std::map<std::string, int> NOTES = {
            {"A", 0},
            {"A#", 1},
            {"B", 2},
            {"C", 3},
            {"C#", 4},
            {"D", 5},
            {"D#", 6},
            {"E", 7},
            {"F", 8},
            {"F#", 9},
            {"G", 10},
            {"G#", 11},
    };
    int _absoluteTime; //miliseconds
    double _volume;
public:
    int getMidiValue();
    int getAbsoluteTime();
    Note(int midiValue, int absoluteTime);
    Note(std::string realNote, int absoluteTime);
    void setVolume(double value);
    double getVolume();
};

#endif /* mNote_hpp */


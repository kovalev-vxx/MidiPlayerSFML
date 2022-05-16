//
//  SongLine.hpp
//  termSFML
//
//  Created by Valery Kovalev on 11.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef SongLine_hpp
#define SongLine_hpp

#include <stdio.h>
#include "Chord.hpp"
#include <iostream>
#include <vector>
#include <map>

class SongLine{
private:
    std::map<int, Chord> _chordsOn; // Key - playback time in milliseconds
    std::map<int, Chord> _chordsOff; // Key - stop playback time in
    int _instrumentId;
    double _duration;
    double _volume;
public:
    SongLine(std::map<int, Chord> chordsOn, std::map<int, Chord>, double volume, int instrumentId);
    std::map<Chord, int> getChordsOn();
    std::map<Chord, int> getChordsOff();
    double getDuration();
    double getVolume();
    void changeVolume(double value);
    int getInstrumentId();
};

#endif /* SongLine_hpp */

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
    SongLine(std::map<int, Chord> chordsOn, std::map<int, Chord> chordsOff, double volume, int instrumentId);
    SongLine(std::map<int, Chord> chordsOn, std::map<int, Chord> chordsOff, double volume);
    SongLine(std::map<int, Chord> chordsOn, std::map<int, Chord> chordsOff, int instrumentId);
    SongLine(std::map<int, Chord> chordsOn, std::map<int, Chord> chordsOff);
    std::map<int, Chord> getChordsOn();
    std::map<int, Chord> getChordsOff();
    double getDuration();
    double getVolume();
    void changeVolume(double value);
    int getInstrumentId();
};

#endif /* SongLine_hpp */

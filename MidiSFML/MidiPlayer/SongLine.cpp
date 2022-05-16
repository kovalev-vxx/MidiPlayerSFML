//
//  SongLine.cpp
//  termSFML
//
//  Created by Valery Kovalev on 11.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "SongLine.hpp"

SongLine::SongLine(std::map<int, Chord> chordsOn, std::map<int, Chord> chordsOff, double volume, int instrumentId){
    _instrumentId = instrumentId;
    _chordsOn = chordsOn;
    _chordsOff  = chordsOff;
    _volume = volume;
};

double SongLine::getDuration(){
    return _duration;
}

std::map<int, Chord> SongLine::getChordsOn(){
    return _chordsOn;
}
std::map<int, Chord> SongLine::getChordsOff(){
    return _chordsOff;
}

double SongLine::getVolume(){
    return _volume;
}

void SongLine::changeVolume(double value){
    if(value<0 | value>1){
        throw std::invalid_argument("The volume must be between 0 and 1");
    }
    _volume = value;
}

int SongLine::getInstrumentId(){
    return _instrumentId;
}

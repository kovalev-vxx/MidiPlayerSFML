//
//  SongLine.cpp
//  termSFML
//
//  Created by Valery Kovalev on 11.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "SongLine.hpp"

SongLine::SongLine(std::vector<Note> notesOn, std::vector<Note> notesOff, double volume, int instrumentId){
    _instrumentId = instrumentId;
    _notesOn = notesOn;
    _notesOff  = notesOff;
    if(volume<0 | volume>1){
        throw std::invalid_argument("The volume must be between 0 and 1");
    }
    _volume = volume;
    sortNotesByAbsoluteTime();
};

SongLine::SongLine(std::vector<Note> notesOn, std::vector<Note> notesOff, double volume){
    _instrumentId = 0;
    _notesOn = notesOn;
    _notesOff  = notesOff;
    if(volume<0 | volume>1){
        throw std::invalid_argument("The volume must be between 0 and 1");
    }
    _volume = volume;
    sortNotesByAbsoluteTime();
}
SongLine::SongLine(std::vector<Note> notesOn, std::vector<Note> notesOff, int instrumentId){
    _instrumentId = instrumentId;
    _notesOn = notesOn;
    _notesOff  = notesOff;
    _volume = 1;
    sortNotesByAbsoluteTime();
}

SongLine::SongLine(std::vector<Note> notesOn, std::vector<Note> notesOff){
    _instrumentId = 0;
    _notesOn = notesOn;
    _notesOff  = notesOff;
    _volume = 1;
    sortNotesByAbsoluteTime();
}

void SongLine::sortNotesByAbsoluteTime(){
    std::sort(_notesOn.begin(), _notesOn.end(), SongLine::notesCompare);
    std::sort(_notesOff.begin(), _notesOff.end(), SongLine::notesCompare);
}

double SongLine::getDuration(){
    return _duration;
}

std::vector<Note> SongLine::getNotesOn(){
    return _notesOn;
}

std::vector<Note> SongLine::getNotesOff(){
    return _notesOff;
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


bool SongLine::notesCompare(Note n1, Note n2){
    return n1.getAbsoluteTime()<n2.getAbsoluteTime();
}


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
#include <iostream>
#include <vector>
#include <map>
#include "Note.hpp"
#include <algorithm>

class SongLine{
private:
    std::vector<Note> _notesOn; // Key - playback time in milliseconds
    std::vector<Note> _notesOff; // Key - stop playback time in
    int _instrumentId;
    double _duration;
    double _volume;
    void sortNotesByAbsoluteTime();
public:
    SongLine(std::vector<Note> notesOn, std::vector<Note> notesOff, double volume, int instrumentId);
    SongLine(std::vector<Note> notesOn, std::vector<Note> notesOff, double volume);
    SongLine(std::vector<Note> notesOn, std::vector<Note> notesOff, int instrumentId);
    SongLine(std::vector<Note> notesOn, std::vector<Note> notesOff);
    std::vector<Note> getNotesOn();
    std::vector<Note> getNotesOff();
    double getDuration();
    double getVolume();
    void changeVolume(double value);
    int getInstrumentId();
    static bool notesCompare(Note n1, Note n2);
};

#endif /* SongLine_hpp */

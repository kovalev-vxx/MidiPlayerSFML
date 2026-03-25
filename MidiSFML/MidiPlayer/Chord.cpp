//
//  chord.cpp
//  termSFML
//
//  Created by Valery Kovalev on 11.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "Chord.hpp"

Chord::Chord(std::vector<Note> notes, float volume){
    _notes = notes;
    if(volume<0 | volume>1){
        throw std::invalid_argument("The volume must be between 0 and 1");
    }
    _volume = volume;
}

Chord::Chord(std::vector<Note> notes){
    _notes = std::move(notes);
    _volume = 1;
}

Chord::Chord(){
    _notes = {};
    _volume = 1;
}

std::vector<Note> Chord::getNotes(){
    return _notes;
}


void Chord::changeVolume(double value){
    if(value<0 | value>1){
        throw std::invalid_argument("The volume must be between 0 and 1");
    }
    _volume = value;
}

double Chord::getVolume(){
    return _volume;
}

void Chord::addNote(Note note){
    _notes.emplace_back(note);
}

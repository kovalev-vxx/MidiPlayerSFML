//
//  note.cpp
//  termSFML
//
//  Created by Valery Kovalev on 11.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "Note.hpp"

Note::Note(int midiValue, int absoluteTime){
    _midiValue = midiValue;
    _absoluteTime = absoluteTime;
    _volume = 1;
};

Note::Note(std::string realNote, int absoluteTime) {
    std::string octave;
    octave = realNote[realNote.length()-1];
    realNote.pop_back();
    std::string note=realNote;
    _absoluteTime = absoluteTime;
    _midiValue = 21+(12*std::stoi(octave))+NOTES[realNote];
    _volume = 1;
}

int Note::getMidiValue(){
    return _midiValue;
}

int Note::getAbsoluteTime(){
    return _absoluteTime;
}

void Note::setVolume(double value){
    if(value<0 | value>1){
        throw std::invalid_argument("The volume must be between 0 and 1");
    }
    _volume = value;
}


double Note::getVolume(){
    return _volume;
}

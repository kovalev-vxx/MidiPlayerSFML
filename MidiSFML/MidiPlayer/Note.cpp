//
//  note.cpp
//  termSFML
//
//  Created by Valery Kovalev on 11.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "mNote.hpp"

Note::Note(int midiValue, int absoluteTime){
    _midiValue = midiValue;
    _absoluteTime = absoluteTime;
};

Note::Note(std::string realNote, int absoluteTime) {
    std::string octave;
    octave = realNote[realNote.length()-1];
    realNote.pop_back();
    std::string note=realNote;
    _absoluteTime = absoluteTime;
    _midiValue = 21+(12*std::stoi(octave))+NOTES[realNote];
}

int Note::getMidiValue(){
    return _midiValue;
}

int Note::getAbsoluteTime(){
    return _absoluteTime;
}

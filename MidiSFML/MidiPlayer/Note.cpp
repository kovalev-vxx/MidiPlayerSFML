//
//  note.cpp
//  termSFML
//
//  Created by Valery Kovalev on 11.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "mNote.hpp"

Note::Note(int midiValue){
    _midiValue = midiValue;
};

Note::Note(std::string realNote) {
    std::string octave;
    octave = realNote[realNote.length()-1];
    realNote.pop_back();
    std::string note=realNote;
    _midiValue = 21+(12*std::stoi(octave))+NOTES[realNote];
}

int Note::getMidiValue(){
    return _midiValue;
}

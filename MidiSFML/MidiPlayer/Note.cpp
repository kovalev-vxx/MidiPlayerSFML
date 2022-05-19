//
//  note.cpp
//  termSFML
//
//  Created by Valery Kovalev on 11.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "Note.hpp"

Note::Note(int midiValue){
    _midiValue = midiValue;
};

Note::Note(std::string realNote) {
    std::string wordOfNote;
    int numOfNote;
    int octave;

    for (auto it = NOTES.begin(); it != NOTES.end(); it++)
    {
        if (realNote.find(it->first) < realNote.size()) {
            wordOfNote = it->first;
            numOfNote = it->second;
        }
    }

    octave = std::stoi(realNote.substr(realNote.find(wordOfNote[wordOfNote.size() - 1]) + 1));

    _midiValue = 21 + (12 + octave) + numOfNote;
}

int Note::getMidiValue(){
    return _midiValue;
}

//
//  chord.hpp
//  termSFML
//
//  Created by Valery Kovalev on 11.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef Chord_hpp
#define Chord_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include "Note.hpp"

class Chord{
    private:
        std::vector<Note> _notes;
        double _volume;
    public:
        Chord(std::vector<Note> notes, float volume);
        Chord(std::vector<Note> notes);
        Chord();
        std::vector<Note> getNotes();
        void changeVolume(double value);
        void addNote(Note note);
        double getVolume();
};

#endif /* Chord_hpp */

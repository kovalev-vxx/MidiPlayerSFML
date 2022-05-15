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

int Note::getMidiValue(){
    return _midiValue;
}

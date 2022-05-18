//
//  note.hpp
//  termSFML
//
//  Created by Valery Kovalev on 11.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef mNote_hpp
#define mNote_hpp

#include <stdio.h>
#include <iostream>
#include <algorithm>

class Note{
private:
    int _midiValue;
public:
    int getMidiValue();
    Note(int midiValue);
    Note(std::string realNote);
};

#endif /* Note_hpp */

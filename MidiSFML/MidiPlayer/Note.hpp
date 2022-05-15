//
//  note.hpp
//  termSFML
//
//  Created by Valery Kovalev on 11.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef Note_hpp
#define Note_hpp

#include <stdio.h>

class Note{
    private:
        int _midiValue;
    public:
        int getMidiValue();
        Note(int midi_value);
};

#endif /* Note_hpp */

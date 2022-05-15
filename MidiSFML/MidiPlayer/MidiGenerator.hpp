//
//  MidiGenerator.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 15.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef MidiGenerator_hpp
#define MidiGenerator_hpp

#include <stdio.h>
#include "Song.hpp"

class MidiGenerator{
public:
    void generateMidi(Song song);
};

#endif /* MidiGenerator_hpp */

#ifndef MidiGenerator_hpp
#define MidiGenerator_hpp

#include <stdio.h>
#include "Song.hpp"
#include "MidiFile.h"
#include "file.hpp"
#include <iostream>
#include <string>

class MidiGenerator{
public:
    std::string generateMidi(Song &song);
};

#endif /* MidiGenerator_hpp */

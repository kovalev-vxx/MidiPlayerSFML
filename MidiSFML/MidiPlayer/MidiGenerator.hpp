#ifndef MidiGenerator_hpp
#define MidiGenerator_hpp

#include <stdio.h>
#include "Song.hpp"

class MidiGenerator{
public:
    void generateMidi(Song song);
};

#endif /* MidiGenerator_hpp */

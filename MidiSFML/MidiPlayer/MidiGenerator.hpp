#ifndef MidiGenerator_hpp
#define MidiGenerator_hpp

#include <stdio.h>
#include <iostream>
#include <string>

#include "Song.hpp"
#include "MidiFile.h"
#include "file.hpp"
#include "note.hpp"
#include "message.hpp"
#include "instrument.hpp"

class MidiGenerator{
public:
    std::string generateMidi(Song &song);
};

#endif /* MidiGenerator_hpp */

#ifndef MidiGenerator_hpp
#define MidiGenerator_hpp

#include <stdio.h>
#include "Song.hpp"
#include "file.hpp"
#include <iostream>
#include <string>
#include "note.hpp"
#include "message.hpp"
#include "instrument.hpp"


class MidiGenerator{
public:
    std::string generateMidi(Song &song, std::string exportDir);
};

#endif /* MidiGenerator_hpp */

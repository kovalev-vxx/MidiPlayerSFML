#ifndef MidiGenerator_hpp
#define MidiGenerator_hpp

#include <stdio.h>
#include <iostream>
#include <string>

#include "Song.hpp"
#include "midifile/MidiFile.h"
#include "cxxmidi/file.hpp"
#include "cxxmidi/note.hpp"
#include "cxxmidi/message.hpp"
#include "cxxmidi/instrument.hpp"

class MidiGenerator{
public:
    std::string generateMidi(Song &song);
};

#endif /* MidiGenerator_hpp */

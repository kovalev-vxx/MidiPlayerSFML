#ifndef MidiGenerator_hpp
#define MidiGenerator_hpp

#include <stdio.h>
#include "Song.hpp"
#include "cxxmidi/file.hpp"
#include <iostream>
#include <string>
#include "Note.hpp"
#include "cxxmidi/message.hpp"
#include "cxxmidi/instrument.hpp"


class MidiGenerator{
public:
    std::string generateMidi(Song &song, std::string exportDir);
};

#endif /* MidiGenerator_hpp */

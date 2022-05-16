#ifndef MidiGenerator_hpp
#define MidiGenerator_hpp

#include <stdio.h>
#include "Song.hpp"
#include "MidiFile.h"
#include "file.hpp"
#include <iostream>
#include <string>

class MidiGenerator{
private:
    cxxmidi::File _file;
public:
//    void generateMidi(Song song);
    void generateMidi();
    std::string generateTestMidi(Song &song);
};

#endif /* MidiGenerator_hpp */

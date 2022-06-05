//
//  Parser.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 15.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef Parser_hpp
#define Parser_hpp

#include <stdio.h>
#include "Song.hpp"
#include <iostream>
#include <string>
#include "MidiFile.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

#include "Song.hpp"
#include "instrument.hpp"
#include "file.hpp"

std::string getContent(std::string line, std::string type);

int getInstrumentId(std::string usingInstrumental);

class Parser{
private:
    double _volumeOfSong = 1.0;
    std::string midiName = "res/midis/BOURREE-1.mid";
public:
    Parser();
//    Song parseFromTxt(std::string filePath);
    Song parseFromMidi(std::string filePath);

    void setVolumeOfSong(double volume);
};
#endif /* Parser_hpp */

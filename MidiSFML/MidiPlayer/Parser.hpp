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
#include <iostream>
#include <string>
#include <fstream>

#include "Song.hpp"
#include "cxxmidi/instrument.hpp"

std::string getContent(std::string line, std::string type);

int getInstrumentalId(std::string usingInstrumental);

class Parser{
private:
    std::string _title;
    int _tempo;
    double _volume = 1.0;

    int _instrumentalId;
public:
    Parser();
    Song parseFromTxt(std::string filePath);
    Song parseFromMidi(std::string filePath);

    void setVolume(double volume);
};

#endif /* Parser_hpp */

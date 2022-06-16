//
//  MidiPlayer.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 16.06.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef MidiPlayer_hpp
#define MidiPlayer_hpp

#include <iostream>
#include <utility>
#include <stdio.h>
#include "sfMidi.h"
#include "Song.hpp"
#include "Parser.hpp"

class MidiPlayer: public sfmidi::Midi{
private:
    Song _song;
public:
    MidiPlayer(const std::string& soundFont, const std::string& fileDir, const std::string& fileName);
    Song getSong();
    
};

#endif /* MidiPlayer_hpp */

//
//  MidiPlayer.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 16.06.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "MidiPlayer.hpp"


MidiPlayer::MidiPlayer(const std::string& soundFont, const std::string& fileDir, const std::string& fileName): sfmidi::Midi(std::move(soundFont), std::move(fileDir+fileName)){
    Parser parser;
    _song = parser.parseFromMidi(fileName, fileDir);
}

Song MidiPlayer::getSong(){
    return _song;
}

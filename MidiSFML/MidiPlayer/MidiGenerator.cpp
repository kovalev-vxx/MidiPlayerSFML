//
//  MidiGenerator.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 15.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "MidiGenerator.hpp"
#include "note.hpp"
#include "message.hpp"
#include "instrument.hpp"


//cxxmidi::File file;

cxxmidi::Message msg;


void MidiGenerator::generateMidi(){
    const uint32_t dt = cxxmidi::converters::Us2dt(
        100000,  // 0.1s
        500000,  // default tempo [us/quarternote]
        500);    // default time division [us/quarternote]

    cxxmidi::File file;
    
    // create 3 tracks
    for (int t = 0; t < 3; t++) {
      cxxmidi::Track &track = file.AddTrack();
        track.push_back(cxxmidi::Event(0, cxxmidi::Message::kProgramChange, cxxmidi::Instrument(1)));
      for (int i = 0; i < 10; i++) {
          
        track.push_back(
            cxxmidi::Event(0,// deltatime
                           cxxmidi::Message::kNoteOn,     // message type
                           cxxmidi::Note(60+i),// note
                           100 + i));// velocity [0...127]
        track.push_back(
            cxxmidi::Event(1000,                            // deltatime
                           cxxmidi::Message::kNoteOn,     // message type
                           cxxmidi::Note(60+i),  // note
                           0));  // velocity=0 => note off
      }
        
      track.push_back(cxxmidi::Event(0,  // deltatime
                                     cxxmidi::Message::kMeta,
                                     cxxmidi::Message::kEndOfTrack));
        
        std::cout << track[track.size()-1].GetName() << std::endl;
    }
    file.SaveAs("MYMIDI.mid");
}


std::string MidiGenerator::generateTestMidi(Song &song){
    cxxmidi::File file;
    
//    for (SongLine &line: song.getLines()){
//        for (auto &chord:line.g)
//    }
}



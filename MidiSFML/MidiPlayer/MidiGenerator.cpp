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
#include "Chord.hpp"


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
          std::cout << 0 << " "<< 0.5 << " " << 1000 << std::endl;
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
    
    
    for (SongLine &line: song.getLines()){
        cxxmidi::Track &track = file.AddTrack();
        track.push_back(cxxmidi::Event(0, cxxmidi::Message::kProgramChange, cxxmidi::Instrument(line.getInstrumentId())));
        
        std::vector<Chord> chords;
        std::vector<int> timesOn;
        std::vector<int> timesOff;
        
        for (auto& chordOn:line.getChordsOn()){
            timesOn.emplace_back(chordOn.first);
            chords.emplace_back(chordOn.second);
        }
        
        for (auto& chordOff:line.getChordsOff()){
            timesOff.emplace_back(chordOff.first);
        }
        
        int lastTimeOn = 0;
        int lastTimeOff = 0;
        
        for (int i =0; i<chords.size();i++){
            int dtOn = timesOn[i]-lastTimeOff;
            int dtOff = timesOff[i]-lastTimeOn;

            
            for (Note &note:chords[i].getNotes()){
                track.push_back(
                    cxxmidi::Event(dtOn,// deltatime
                                   cxxmidi::Message::kNoteOn,     // message type
                                   cxxmidi::Note(note.getMidiValue()),// note
                                   (int)127*chords[i].getVolume()*line.getVolume()*song.getVolume()));// velocity [0...127]
                track.push_back(
                    cxxmidi::Event(dtOff,                            // deltatime
                                   cxxmidi::Message::kNoteOn,     // message type
                                   cxxmidi::Note(note.getMidiValue()),  // note
                                   0));  // velocity=0 => note off
            }
            
            lastTimeOn += timesOn[i];
            lastTimeOff += timesOff[i];
        }
        
        track.push_back(cxxmidi::Event(0,  // deltatime
                                       cxxmidi::Message::kMeta,
                                       cxxmidi::Message::kEndOfTrack));
        
    }
    std::string fileName = "res/generatedMidis/"+song.getTitle()+".mid";
    file.SaveAs(fileName.c_str());
    return fileName;
}




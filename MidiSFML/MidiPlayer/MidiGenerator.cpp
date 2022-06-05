//
//  MidiGenerator.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 15.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "MidiGenerator.hpp"
#include "cxxmidi/note.hpp"
#include "cxxmidi/message.hpp"
#include "cxxmidi/instrument.hpp"


std::string MidiGenerator::generateMidi(Song &song){
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
                    cxxmidi::Event(cxxmidi::converters::Us2dt(dtOn*1000,  // 0.5s
                                                              500000,  // tempo [us/quarternote]
                                                              500),// deltatime
                                   cxxmidi::Message::kNoteOn,     // message type
                                   cxxmidi::Note(note.getMidiValue()),// note
                                   (int)127*chords[i].getVolume()*line.getVolume()*song.getVolume()));// velocity [0...127]
                track.push_back(
                    cxxmidi::Event(cxxmidi::converters::Us2dt(dtOff*1000,  // 0.5s
                                                              500000,  // tempo [us/quarternote]
                                                              500),                            // deltatime
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




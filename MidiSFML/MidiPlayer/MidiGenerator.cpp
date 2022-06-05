//
//  MidiGenerator.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 15.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "MidiGenerator.hpp"



std::string MidiGenerator::generateMidi(Song &song){
    cxxmidi::File file;
    
    for (SongLine &line: song.getLines()){
        cxxmidi::Track &track = file.AddTrack();

        track.push_back(cxxmidi::Event(0, cxxmidi::Message::kProgramChange, cxxmidi::Instrument(line.getInstrumentId())));
        std::vector<std::pair<Note, bool>> notes;
        
        for (auto& noteOn:line.getNotesOn()){
            notes.emplace_back(std::make_pair(noteOn, true));
        }
        
        for (auto& noteOff:line.getNotesOff()){
            notes.emplace_back(std::make_pair(noteOff, false));
        }
        std::cout << notes.size() << std::endl;
        
        std::sort(notes.begin(), notes.end(), [](std::pair<Note, bool> n1, std::pair<Note, bool> n2){
            return n1.first.getAbsoluteTime() < n2.first.getAbsoluteTime();
        });
        
        int lastTime = 0;
        
        for (int i =0; i<notes.size();i++){
            int dt = notes[i].first.getAbsoluteTime()-lastTime;

            if (notes[i].second){
                track.push_back(
                    cxxmidi::Event(dt,// deltatime
                                   cxxmidi::Message::kNoteOn,     // message type
                                   cxxmidi::Note(notes[i].first.getMidiValue()),// note
                                   (int)127*notes[i].first.getVolume()*line.getVolume()*song.getVolume()));// velocity [0...127]
            } else {
                track.push_back(
                    cxxmidi::Event(dt,// deltatime
                                   cxxmidi::Message::kNoteOn,     // message type
                                   cxxmidi::Note(notes[i].first.getMidiValue()),// note
                                   0));// velocity [0...127]
            }
            
            lastTime = notes[i].first.getAbsoluteTime();
            
        }
        
        track.push_back(cxxmidi::Event(0,  // deltatime
                                       cxxmidi::Message::kMeta,
                                       cxxmidi::Message::kEndOfTrack));
        
    }
    std::string fileName = "res/generatedMidis/"+song.getTitle()+".mid";
    file.SaveAs(fileName.c_str());
    return fileName;
}




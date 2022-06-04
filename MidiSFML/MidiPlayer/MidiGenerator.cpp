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
        
        int lastTimeOff = 0;
        
        for (int i =0; i<chords.size();i++){
            int dtOn = timesOn[i]-lastTimeOff;
            int dtOff = timesOff[i]-timesOn[i];
            
            std::cout << i << " " << dtOn << " " << dtOff << std::endl;
            
//            std::cout  << "dtOn" << dtOn << std::endl;
//            std::cout  << "dOn" << timesOn[i] << std::endl;
            
//            std::cout  << "dtOff" << dtOff << std::endl;
//            std::cout  << "dOff" << timesOff[i] << std::endl;
            
            for(int j=0; j<chords[i].getNotes().size();j++){
                if (j==0){
                    std::cout << j << " CHORD: " << i << " ADDED ON: " << dtOn << " note:" << chords[i].getNotes()[j].getMidiValue() << std::endl;
                    track.push_back(
                        cxxmidi::Event(dtOn,// deltatime
                                       cxxmidi::Message::kNoteOn,     // message type
                                       cxxmidi::Note(chords[i].getNotes()[j].getMidiValue()),// note
                                       (int)127*chords[i].getVolume()*line.getVolume()*song.getVolume()));// velocity [0...127]
                } else {
                    std::cout << j << " CHORD: " << i << " ADDED ON: " << 0 << " note:" << chords[i].getNotes()[j].getMidiValue() << std::endl;
                    track.push_back(
                        cxxmidi::Event(0,// deltatime
                                       cxxmidi::Message::kNoteOn,     // message type
                                       cxxmidi::Note(chords[i].getNotes()[j].getMidiValue()),// note
                                       (int)127*chords[i].getVolume()*line.getVolume()*song.getVolume()));// velocity [0...127]
                }
            }
            
            for(int j=0; j<chords[i].getNotes().size();j++){
                if (j==0){
                    std::cout << j << " CHORD: " << i << " ADDED Off: " << dtOff << " note:" << chords[i].getNotes()[j].getMidiValue() << std::endl;
                    track.push_back(
                        cxxmidi::Event(dtOff,// deltatime
                                       cxxmidi::Message::kNoteOn,     // message type
                                       cxxmidi::Note(chords[i].getNotes()[j].getMidiValue()),// note
                                       0));// velocity [0...127]
                } else {
                    std::cout << j << " CHORD: " << i << " ADDED Off: " << 0 << " note:" << chords[i].getNotes()[j].getMidiValue() << std::endl;
                    track.push_back(
                        cxxmidi::Event(0,// deltatime
                                       cxxmidi::Message::kNoteOn,     // message type
                                       cxxmidi::Note(chords[i].getNotes()[j].getMidiValue()),// note
                                       0));// velocity [0...127]
                }
            }
            lastTimeOff = timesOff[i];
        }
        
        track.push_back(cxxmidi::Event(0,  // deltatime
                                       cxxmidi::Message::kMeta,
                                       cxxmidi::Message::kEndOfTrack));
        
    }
    std::string fileName = "res/generatedMidis/"+song.getTitle()+".mid";
    file.SaveAs(fileName.c_str());
    return fileName;
}




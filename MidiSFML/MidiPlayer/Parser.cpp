//
//  Parser.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 15.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "Parser.hpp"

std::string getContent(std::string line, std::string type) {
    int first;
    int last;
    int charCount;

    if (type == "tag") {
        first = line.find_first_of('<') + 1;
        last = line.find_first_of('>');
    }
    else if (type == "body") {
        first = line.find_first_of('>') + 1;
        last = line.find_last_of('<');
    }
    else if (type == "1") {
        first = line.find_first_of('"') + 1;
        last = line.substr(first).find_first_of('"') + first;
    }
    else if (type == "2") {
        last = line.find_last_of('"');
        first = line.substr(0, last).find_last_of('"') + 1;
    }

    charCount = last - first;
    return line.substr(first, charCount);
}

Parser::Parser() {
}

Song Parser::parseFromTxt(std::string filePath){
}



Song Parser::parseFromMidi(std::string fileName, std::string fileDir){
    smf::MidiFile midifile;
    std::cout << fileDir+fileName << std::endl;
    midifile.read(fileDir+fileName);
    midifile.doTimeAnalysis();
    midifile.linkNotePairs();
    midifile.setMillisecondTicks();
    
    
    if (!midifile.status()) {
       std::cerr << "Error reading MIDI file" << std::endl;
    }
    
    std::vector<SongLine> songLines;

    for (int i=0; i<midifile.getNumTracks(); i++){
        std::vector<Note> notesOn;
        std::vector<Note> notesOff;
        int instrumentId = 0;
        
        for(int j=0;j<midifile[i].size();j++){
            if (midifile[i][j].isTimbre()) {
                instrumentId = midifile[i][j].getP1();
            }
            double seconds = midifile[i][j].seconds*1000;
            int midiValue = (int)midifile[i][j][1];
            bool isNoteOn = midifile[i][j].isNoteOn();
            bool isNoteOff = midifile[i][j].isNoteOff();
            if (isNoteOn){
                notesOn.push_back(Note(midiValue,seconds));
                notesOn.back().setVolume((double)(int)midifile[i][j][2]/127);
            }
            if (isNoteOff){
                notesOff.push_back(Note(midiValue,seconds));
            }
        }
        songLines.push_back(SongLine(notesOn, notesOff, instrumentId));
    }
    for (int i=0;i<4;i++){
        fileName.pop_back();
    }
    return Song(songLines, fileName);
}

void Parser::setVolumeOfSong(double volume) {
    if(volume < 0 || 1 < volume){
        throw std::invalid_argument("The volume must be between 0 and 1");
    }
    this->_volumeOfSong = volume;
}

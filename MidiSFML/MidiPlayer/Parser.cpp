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

int getInstrumentId(std::string usingInstrumental) {
    cxxmidi::Instrument instrument;
    std::string lookingInstrumental;
    int instrumentalId;
    int i = 0;

    while (true) {
        lookingInstrumental = instrument.GetName(i);

        std::transform(lookingInstrumental.begin(), lookingInstrumental.end(), lookingInstrumental.begin(), tolower);
        std::transform(usingInstrumental.begin(), usingInstrumental.end(), usingInstrumental.begin(), tolower);

        if (lookingInstrumental.find(usingInstrumental) < lookingInstrumental.size()) {
            instrumentalId = i;
            break;
        }
        i++;
    }

    return instrumentalId;
}

Parser::Parser() {
}

Song Parser::parseFromTxt(std::string filePath){
    std::string title;
    int tempo;
    int minuteToMilliseconds = 60 * 1000;
    double oneDuration;
    double timeForChord = 0.0;

    int instrumentId;
    std::map<int, Chord> chordsOn;
    std::map<int, Chord> chordsOff;

    std::vector<SongLine> songLines;

    std::string str;
    std::string tag;
    std::string body;
    std::ifstream fs;
    fs.exceptions(std::ifstream::badbit | std::ifstream::failbit);

    try {
        // open file
        fs.open(filePath);
        // process each line
        while (!fs.eof()) {
            str = "";
            getline(fs, str);

            tag = getContent(str, "tag");
            body = getContent(str, "body");
            // For Song
            if (tag.find("title") < tag.size())
                title = body;
            else if (tag.find("tempo") < tag.size()) {
                tempo = stoi(body);
                oneDuration = static_cast<double>(minuteToMilliseconds) / tempo;
            }
            // For SongLines
            else if (tag.find("line instrument") < tag.size())
                instrumentId = getInstrumentId(getContent(tag, "1"));
            else if (tag.find("chord") < tag.size()) {
                // Create a range notes
                std::vector<Note> notes;
                std::string note;
                // listOfNote have a string of notes
                std::string stringOfNote = body;
                std::vector<std::string> listOfNotes;

                while (true) {
                    note = stringOfNote.substr(0, stringOfNote.find_first_of(','));
                    stringOfNote = stringOfNote.substr(stringOfNote.find_first_of(',') + 2);
                    listOfNotes.push_back(note);

                    if (stringOfNote.find_first_of(',') > stringOfNote.size()) {
                        note = stringOfNote.substr(0, stringOfNote.find_first_of(','));
                        listOfNotes.push_back(note);
                        break;
                    }
                }
                // Create a chord
                if (getContent(tag, "2") == "real_note")
                    // ex: A4 to 69
           
                    for (std::string i : listOfNotes)
                        notes.emplace_back(Note(i));
                else
                    for (std::string i : listOfNotes)
                        notes.emplace_back(Note(std::stoi(i)));

                Chord chord(notes);

                // Count a duration for the chord
                double chordDuration;
                std::string duration = getContent(tag, "1");

                if (duration.find('/') < duration.size()) {
                    int numerator = std::stoi(duration.substr(0, duration.find_first_of('/')));
                    int denominator = std::stoi(duration.substr(duration.find_first_of('/') + 1));
                    chordDuration = (numerator * oneDuration) / static_cast<double>(denominator);
                }
                else
                    chordDuration = static_cast<double>(std::stoi(getContent(tag, "1")) * oneDuration);

                // Create a chordsOn and a chordsOff
                chordsOn[timeForChord] = chord;
                timeForChord += chordDuration;
                chordsOff[timeForChord] = chord;
            }
            else if (tag.find("pause") < tag.size()) {
                // Add pause for a SongLine
                double pause;
                std::string duration = getContent(tag, "1");

                if (duration.find('/') < duration.size()) {
                    int numerator = std::stoi(duration.substr(0, duration.find_first_of('/')));
                    int denominator = std::stoi(duration.substr(duration.find_first_of('/') + 1));
                    pause = (numerator * oneDuration) / static_cast<double>(denominator);
                }
                else
                    pause = static_cast<double>(std::stoi(getContent(tag, "1")) * oneDuration);

                timeForChord += pause;
            }
            else if ((tag.find("/line") < tag.size()) && (tag.find("/lines") > tag.size())) {
                // Create a range SongLines
                songLines.emplace_back(SongLine(chordsOn, chordsOff, instrumentId));
                chordsOn.clear();
                chordsOff.clear();
            }
        }

        return Song(songLines, tempo, title, _volumeOfSong);
    }
    catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
        throw std::invalid_argument("File wasn't opened.");
    }
}


void Parser::setVolumeOfSong(double volume) {
    if(volume < 0 | volume > 1){
        throw std::invalid_argument("The volume must be between 0 and 1.");
    }
    _volumeOfSong = volume;
}

//Song Parser::parseFromMidi(std::string filePath){
////    cxxmidi::File file = cxxmidi::File(filePath.c_str());
//    cxxmidi::File file = cxxmidi::File(midiName.c_str());
//
//    try{
//        for (int i=0; i<file.size()-1;i++){
//            auto &track = file[i];
////            std::cout << "TRACK NAME: " << track.GetName() << std::endl;
//            for (int j=0; j<track.size();j++){
//                auto &event = track[j];
////                std::cout << event.GetType() << std::endl;
//            }
//        }
//    }catch(...){
//        std::cout << "ERROR" << std::endl;
//    }
//}


std::pair<std::map<int, Chord>, std::map<int, Chord>> Parser::parseFromMidi(std::string filePath){
    std::cout << "PARSER" << std::endl;
    std::map<int, Chord> starts;
    std::map<int, Chord> ends;
    smf::MidiFile _midifile;
    _midifile.read(filePath);
    _midifile.doTimeAnalysis();
    _midifile.linkNotePairs();
    _midifile.setMillisecondTicks();
    
    if (!_midifile.status()) {
       std::cerr << "Error reading MIDI file" << std::endl;
    }
    
    int tempo;
    for (int i=0; i<_midifile[0].size(); i++) {
        if (_midifile[0][i].isTempo()){
            tempo = _midifile[0][i].getTempoBPM();
            std::cout << "TEMPO " << _midifile[0][i].getTempoBPM() << std::endl;
        }
    }
    
    int track = 1;
    int last_tick = 0;
    double last_second = 0;
    for (int i=0; i<_midifile[track].size(); i++) {
        int tick = _midifile[track][i].tick;
        double seconds = _midifile[track][i].seconds*1000;
        double duration = _midifile[track][i].getDurationInSeconds();
        int midiValue = (int)_midifile[track][i][1];
        bool isNoteOn = _midifile[track][i].isNoteOn();
        bool isNoteOff = _midifile[track][i].isNoteOff();
        if (isNoteOn){
            std::cout << "SEC:" << seconds << " DUR: " << duration << " MIDI: " << midiValue << std::endl;
        }
        if (isNoteOn){
            if (starts[seconds].getNotes().size()){
                starts[seconds].addNote(Note(midiValue));
            } else {
                starts[seconds] = Chord({Note(midiValue)});
            }

        }
        if (isNoteOff){
            if (ends[seconds].getNotes().size()){
                ends[seconds].addNote(Note(midiValue));
            } else {
                ends[seconds] = Chord({Note(midiValue)});
            }

        }
    
    
        
//        if (isNoteOn){
//            if (starts[seconds*1000].getNotes()){
//                starts[seconds*1000].addNote(Note(midiValue));
//            } else {
//                starts[seconds*1000] = Chord();
//                starts[seconds*1000].changeDuration(calcTempo(duration, tempo));
//                starts[seconds*1000].addNote(Note(midiValue));
//            }
//        }
//
//        if (isNoteOff){
//            if (ends[seconds*1000].getDuration()){
//                ends[seconds*1000].addNote(Note(midiValue));
//            } else {
//                ends[seconds*1000] = Chord();
//                ends[seconds*1000].changeDuration(calcTempo(duration, tempo));
//                ends[seconds*1000].addNote(Note(midiValue));
//            }
//        }
    }
    return std::make_pair(starts, ends);
}

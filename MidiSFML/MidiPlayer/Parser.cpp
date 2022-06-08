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
    int oneDuration;
    int timeForChord = 0;

    int instrumentId;
    std::vector<Note> notesOn;
    std::vector<Note> notesOff;

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
                oneDuration = minuteToMilliseconds / tempo;
            }
            // For SongLines
            else if (tag.find("line instrument") < tag.size())
                instrumentId = std::stoi(getContent(tag, "1"));
            else if (tag == "line")
                instrumentId = 1;
            else if (tag.find("chord") < tag.size()) {
                // listOfNote have a string of notes
                std::vector<std::string> listOfNotes;

                std::string note;
                std::string sep = ", ";
                size_t sep_size = sep.size();

                while (true) {
                    note = body.substr(0, body.find(sep));
                    if (note.size() != 0)
                        listOfNotes.push_back(note);
                    if (note.size() == body.size())
                        break;
                    else
                        body = body.substr(note.size() + sep_size);
                }

                // Count a duration for the chord
                int chordDuration;
                std::string duration = getContent(tag, "1");

                if (duration.find('/') < duration.size()) {
                    int numerator = std::stoi(duration.substr(0, duration.find_first_of('/')));
                    int denominator = std::stoi(duration.substr(duration.find_first_of('/') + 1));
                    chordDuration = (numerator * oneDuration) / denominator;
                }
                else
                    chordDuration = std::stoi(getContent(tag, "1")) * oneDuration;

                // Create a start of SongLine
                if (getContent(tag, "2") == "real_note")
                    // ex: A4 to 69
                    for (std::string &i : listOfNotes)
                        notesOn.emplace_back(Note(i, timeForChord)); // double to int
                else
                    for (std::string &i : listOfNotes)
                        notesOn.emplace_back(Note(std::stoi(i), timeForChord));

                timeForChord += chordDuration;

                // Create an end of SongLine
                if (getContent(tag, "2") == "real_note")
                    // ex: A4 to 69
                    for (std::string &i : listOfNotes)
                        notesOn.emplace_back(Note(i, timeForChord)); // double to int
                else
                    for (std::string &i : listOfNotes)
                        notesOn.emplace_back(Note(std::stoi(i), timeForChord));
            }
            else if (tag.find("pause") < tag.size()) {
                // Add pause for a SongLine
                int pause;
                std::string duration = getContent(tag, "1");

                if (duration.find('/') < duration.size()) {
                    int numerator = std::stoi(duration.substr(0, duration.find_first_of('/')));
                    int denominator = std::stoi(duration.substr(duration.find_first_of('/') + 1));
                    pause = (numerator * oneDuration) / denominator;
                }
                else
                    pause = std::stoi(getContent(tag, "1")) * oneDuration;

                timeForChord += pause;
            }
            else if ((tag.find("/line") < tag.size()) && (tag.find("/lines") > tag.size())) {
                // Create a range SongLines
                songLines.emplace_back(SongLine(notesOn, notesOff, instrumentId));
                notesOn.clear();
                notesOff.clear();
            }
        }

        return Song(songLines, title, _volumeOfSong);
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

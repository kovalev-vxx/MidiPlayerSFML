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

int getInstrumentalId(std::string usingInstrumental) {
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
            // in song
            if (tag.find("title") < tag.size())
                _title = body;
            else if (tag.find("tempo") < tag.size())
                _tempo = stoi(body);
            // in songLine
            else if (tag.find("line instrument") < tag.size())
                _instrumentalId = getInstrumentalId(getContent(tag, "1"));
            else if (tag.find("chord") < tag.size()) {
                std::vector<Note> notes;
                std::string note;
                bool check = true;

                // listOfNote have string of notes
                std::string stringOfNote = body;
                std::vector<std::string> listOfNotes;

                while (check) {
                    note = stringOfNote.substr(0, stringOfNote.find_first_of(','));
                    stringOfNote = stringOfNote.substr(stringOfNote.find_first_of(", ") + 1);
                    listOfNotes.push_back(note);

                    if (stringOfNote.find_first_of(',') > stringOfNote.size()) {
                        note = stringOfNote.substr(0, stringOfNote.find_first_of(','));
                        stringOfNote = stringOfNote.substr(stringOfNote.find_first_of(", ") + 1);
                        listOfNotes.push_back(note);
                        check = false;
                    }
                }

                if (getContent(tag, "2") == "real_note") {
                    // ex: A4 to 69
                    for (std::string i : listOfNotes) {
                        notes.emplace_back(Note(i));
                    }
                }
                else {
                    for (std::string i : listOfNotes) {
                        notes.emplace_back(Note(std::stoi(i)));
                    }
                }
                /*duration = getContent(tag, "1");*/
                // in <line>
                Chord chord(notes);

                std::map<int, Chord> chordsOn;
                std::map<int, Chord> chordsOff;

                int minuteToMilliseconds = (60 * 1000);
                double oneDuration;
                double chordDuration;

                std::string duration = getContent(tag, "1");
                oneDuration = static_cast<double>(minuteToMilliseconds) / _tempo;

                if (duration.find('/') < duration.size()) {
                    int numerator = std::stoi(duration.substr(0, duration.find_first_of('/')));
                    int denominator = std::stoi(duration.substr(duration.find_first_of('/') + 1));
                    chordDuration = numerator / static_cast<double>(denominator);
                }
                else
                    chordDuration = static_cast<double>(std::stoi(getContent(tag, "1")) * oneDuration);

//                chordsOn[] =
            }
            else if (tag.find("pause") < tag.size()) {

            }
        }
    }
    catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
        throw std::invalid_argument("File wasn't opened.");
    }
}


Song Parser::parseFromMidi(std::string filePath){
    
}

void Parser::setVolume(double volume) {
    if(volume < 0 | volume > 1){
        throw std::invalid_argument("The volume must be between 0 and 1.");
    }
    _volume = volume;
}

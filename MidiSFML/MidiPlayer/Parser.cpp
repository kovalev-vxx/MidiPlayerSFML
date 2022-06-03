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


Song Parser::parseFromMidi(std::string filePath){
    
}

void Parser::setVolumeOfSong(double volume) {
    if(volume < 0 | volume > 1){
        throw std::invalid_argument("The volume must be between 0 and 1.");
    }
    _volumeOfSong = volume;
}

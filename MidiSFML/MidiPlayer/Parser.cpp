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
    /*else if (type == "2") {
        last = line.find_last_of('"');
        first = line.substr(0, last).find_last_of('"') + 1;
    }*/

    charCount = last - first;
    return line.substr(first, charCount);
}

int getInstrumentalId(std::string instrumental) {
    if (instrumental == "piano")
        return 0;
    else if (instrumental == "guitar")
        return 24;
    else if (instrumental == "sax")
        return 66;
    else {
        std::cout << "Set default instrumental id" << std::endl;
        return 1;
    }
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

            if (tag.find("title") < tag.size())
                _title = body;
            else if (tag.find("tempo") < tag.size())
                _tempo = stoi(body);
            else if (tag.find("line instrument") < tag.size())
                _InstrumentalId = getInstrumentalId(getContent(tag, "1"));
            else if (tag.find("chord") < tag.size()) {
                duration = getContent(tag, "1");

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

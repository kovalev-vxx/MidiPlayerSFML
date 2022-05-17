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
        last = line.substr(first).find_first_of("");
    }
    else if (type == "2") {
        first = line.find_first_of("mode=") + 2;
        last = line.find_last_of('"');
    }
    charCount = last - first;
    return line.substr(first, charCount);
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
            else if (tag.find("line instrument") < tag.size()) {

            }
            else if (tag.find("chord") < tag.size()) {

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

//
//  Parser.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 15.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef Parser_hpp
#define Parser_hpp

#include <stdio.h>
#include "Song.hpp"
#include <iostream>
#include <string>

class Parser{
private:
public:
    Parser();
    Song parseFromTxt(std::string filePath);
};
#endif /* Parser_hpp */

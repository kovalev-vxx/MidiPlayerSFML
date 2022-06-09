//
//  App.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 17.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef App_hpp
#define App_hpp

#include <stdio.h>
#include <stack>
#include <SFML/Graphics.hpp>
#include "../Utils/fileSystem.hpp"
#include "../MidiPlayer/MidiGenerator.hpp"
#include "../MidiPlayer/Parser.hpp"
#include "VisAlgorithm.hpp"
#include "sfMidi.h"
 
class AppState;
 
class App
{
    private:
    
    sf::RenderWindow _window;
    sf::Sprite _background;
    fileSystem _fs;
    MidiGenerator _generator;
    Parser _parser;
    Song _song;
    std::string _filePath;
    std::string _synthPath;
    
    public:
    
    
    App(int argc, char const** argv);
    void config();
    void start();
};

#endif /* App_hpp */

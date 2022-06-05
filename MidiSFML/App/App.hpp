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
#include "TextureManager.hpp"
#include "../Utils/fileSystem.hpp"
 
class AppState;
 
class App
{
    private:
    
    void loadTextures();
    
    public:
 
    std::stack<AppState*> states;
 
    sf::RenderWindow window;
    TextureManager texmgr;
    sf::Sprite background;
    fileSystem fs;
 
    void pushState(AppState* state);
    void popState();
    void changeState(AppState* state);
    AppState* peekState();
 
    void gameLoop();
    App(int argc, char const** argv);
    ~App();
};

#endif /* App_hpp */

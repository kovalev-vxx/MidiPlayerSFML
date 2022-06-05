//
//  App.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 17.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "App.hpp"

 
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
 
#include "App.hpp"
#include "AppState.hpp"

void App::loadTextures()
{
    texmgr.loadTexture("background", "res/bg.png");
}
 
void App::pushState(AppState* state)
{
    this->states.push(state);
 
    return;
}
 
void App::popState()
{
    delete this->states.top();
    this->states.pop();
 
    return;
}
 
void App::changeState(AppState* state)
{
    if(!this->states.empty())
        popState();
    pushState(state);
 
    return;
}
 
AppState* App::peekState()
{
    if(this->states.empty()) return nullptr;
    return this->states.top();
}
 
void App::gameLoop()
{
    sf::Clock clock;
 
    while(this->window.isOpen())
    {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();
 
        if(peekState() == nullptr) continue;
        peekState()->handleInput();
        peekState()->update(dt);
        this->window.clear(sf::Color::Black);
        peekState()->draw(dt);
        this->window.display();
    }
}
 
App::App(int argc, char const** argv)
{
    this->fs.setRoot(argv[0]);
    this->texmgr.setFileSystem(this->fs);
    this->loadTextures();
    this->window.create(sf::VideoMode(800, 600), "Midi Player");
    this->window.setFramerateLimit(60);
    this->background.setTexture(this->texmgr.getRef("background"));
    
}
 
App::~App()
{
    while(!this->states.empty()) popState();
}

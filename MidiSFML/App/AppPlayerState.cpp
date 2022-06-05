//
//  AppPlayerState.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 17.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "AppPlayerState.hpp"

void AppPlayerState::draw(const float dt)
{
    this->app->window.clear(sf::Color::Black);
//    this->app->window.draw(this->app->background);
 
    return;
}
 
void AppPlayerState::update(const float dt)
{
    return;
}
 
void AppPlayerState::handleInput()
{
    sf::Event event;
 
    while(this->app->window.pollEvent(event))
    {
        switch(event.type)
        {
            /* Close the window */
            case sf::Event::Closed:
            {
                this->app->window.close();
                break;
            }
            /* Resize the window */
            case sf::Event::Resized:
            {
                gameView.setSize(event.size.width, event.size.height);
                guiView.setSize(event.size.width, event.size.height);
//                this->app->background.setPosition(this->app->window.mapPixelToCoords(sf::Vector2i(0, 0), this->guiView));
//                this->app->background.setScale(
//                    float(event.size.width) / float(this->app->background.getTexture()->getSize().x),
//                    float(event.size.height) / float(this->app->background.getTexture()->getSize().y));
                break;
            }
            default: break;
        }
    }
 
    return;
}
 
AppPlayerState::AppPlayerState(App* app)
{
    this->app = app;
    sf::Vector2f pos = sf::Vector2f(this->app->window.getSize());
    this->guiView.setSize(pos);
    this->gameView.setSize(pos);
    pos *= 0.5f;
    this->guiView.setCenter(pos);
    this->gameView.setCenter(pos);
}

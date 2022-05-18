//
//  AppStartState.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 17.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "AppStartState.hpp"
#include "AppState.hpp"

void AppStartState::draw(const float dt)
{
    this->app->window.setView(this->view);
 
    this->app->window.clear(sf::Color::Black);
    this->app->window.draw(this->app->background);

    return;
}
 
void AppStartState::update(const float dt)
{
}

void AppStartState::handleInput()
{
    sf::Event event;
 
    while(this->app->window.pollEvent(event))
    {
        switch(event.type)
        {
            /* Close the window */
            case sf::Event::Closed:
            {
                app->window.close();
                break;
            }
            /* Resize the window */
            case sf::Event::Resized:
            {
                this->view.setSize(event.size.width, event.size.height);
//                this->app->background.setPosition(this->app->window.mapPixelToCoords(sf::Vector2i(0, 0)));
//                this->app->background.setScale(
//                    float(event.size.width) / float(this->app->background.getTexture()->getSize().x),
//                    float(event.size.height) / float(this->app->background.getTexture()->getSize().y));
//                break;
            }
            case sf::Event::KeyPressed:
            {
                if(event.key.code == sf::Keyboard::Escape) this->app->window.close();
                    else if(event.key.code == sf::Keyboard::Space) this->loadgame();
                    break;
            }
            default: break;
        }
    }
 
    return;
}

AppStartState::AppStartState(App* app)
{
    this->app = app;
    sf::Vector2f pos = sf::Vector2f(this->app->window.getSize());
    this->view.setSize(pos);
    pos *= 0.5f;
    this->view.setCenter(pos);
}

void AppStartState::loadgame()
{
    this->app->pushState(new AppStartState(this->app));
 
    return;
}

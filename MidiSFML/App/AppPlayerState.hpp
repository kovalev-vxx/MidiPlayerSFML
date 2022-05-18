//
//  AppPlayerState.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 17.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef AppPlayerState_hpp
#define AppPlayerState_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "AppState.hpp"

class AppPlayerState: public AppState{
private:
    sf::View gameView;
    sf::View guiView;
public:
    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();
 
    AppPlayerState(App* app);
};
#endif /* AppPlayerState_hpp */

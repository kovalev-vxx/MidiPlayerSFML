//
//  AppStartState.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 17.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef AppStartState_hpp
#define AppStartState_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "AppState.hpp"

class AppStartState: public AppState{
private:
    sf::View view;
    void loadgame();
public:
    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();
 
    AppStartState(App* app);
};

#endif /* AppStartState_hpp */

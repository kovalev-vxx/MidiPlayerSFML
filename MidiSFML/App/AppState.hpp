//
//  AppState.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 17.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef AppState_hpp
#define AppState_hpp

#include <stdio.h>
#include "App.hpp"

class AppState
{
    public:
 
    App* app;
 
    virtual void draw(const float dt) = 0;
    virtual void update(const float dt) = 0;
    virtual void handleInput() = 0;
};

#endif /* AppState_hpp */

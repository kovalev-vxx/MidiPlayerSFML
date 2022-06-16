//
//  IVisAlgorithm.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 16.06.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef IVisAlgorithm_hpp
#define IVisAlgorithm_hpp

#include <stdio.h>
#include <vector>
#include <SFML/Graphics.hpp>

class IVisAlgorithm{
public:
    virtual void draw() = 0;
};

#endif /* IVisAlgorithm_hpp */

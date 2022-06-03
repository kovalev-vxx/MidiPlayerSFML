//
//  TextureManager.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 18.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "TextureManager.hpp"

void TextureManager::loadTexture(const std::string& name, const std::string& filename)
{
    /* Load the texture */
    sf::Texture tex;
    tex.loadFromFile(_fs->getRoot()+filename);

    /* Add it to the list of textures */
    this->textures[name] = tex;

    return;
}

sf::Texture& TextureManager::getRef(const std::string& texture)
{
    
    return this->textures.at(texture);
    
}


void TextureManager::setFileSystem(fileSystem &fs){
    this->_fs = &fs;
}

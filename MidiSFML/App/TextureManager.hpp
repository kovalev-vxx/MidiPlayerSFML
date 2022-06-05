//
//  TextureManager.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 18.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef TextureManager_hpp
#define TextureManager_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include "../Utils/fileSystem.hpp"

class TextureManager
{
    private:

    /* Array of textures used */
    std::map<std::string, sf::Texture> textures;
    fileSystem* _fs;

    public:

    /* Add a texture from a file */
    void loadTexture(const std::string& name, const std::string &filename);

    /* Translate an id into a reference */
    sf::Texture& getRef(const std::string& texture);

    void setFileSystem(fileSystem &fs);

    /* Constructor */
    TextureManager()
    {
    }
};

#endif /* TextureManager_hpp */

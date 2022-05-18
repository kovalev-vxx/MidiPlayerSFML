//
//  fileSystem.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 18.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "fileSystem.hpp"



fileSystem::fileSystem(std::string root){
    _root = std::filesystem::path(root + "/");
    
}

void fileSystem::setRoot(std::string path){
    _root = std::filesystem::path(path).parent_path();
    _root = std::filesystem::path(_root+"/");
}


std::string fileSystem::getRoot(){
    return _root;
}

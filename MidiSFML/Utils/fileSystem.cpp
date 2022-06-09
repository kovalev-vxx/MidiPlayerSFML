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


std::string fileSystem::pathToResousers(){
    return std::filesystem::path(_root + "res/");;
}

std::string fileSystem::pathToSynths(){
    return std::filesystem::path(pathToResousers() + "synths/");
}

std::string fileSystem::pathToMidis(){
    return std::filesystem::path(pathToResousers() + "midis/");
}

std::string fileSystem::pathToGeneratedMidis(){
    return std::filesystem::path(pathToResousers() + "generatedMidis/");
}

std::vector<std::string> fileSystem::fileList(std::string path){
    std::vector<std::string> files;
    for (const auto & entry : std::filesystem::directory_iterator(path))
        files.push_back(entry.path().filename());
    return files;
}

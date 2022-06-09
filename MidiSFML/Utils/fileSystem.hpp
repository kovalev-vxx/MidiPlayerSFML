//
//  fileSystem.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 18.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef fileSystem_hpp
#define fileSystem_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>

class fileSystem{
private:
    std::string _root;
public:
    fileSystem(){};
    void setRoot(std::string path);
    fileSystem(std::string root);
    std::string getRoot();
    std::string pathToResousers();
    std::string pathToSynths();
    std::string pathToMidis();
    std::string pathToGeneratedMidis();
    std::vector<std::string> fileList(std::string path);
    
};


#endif /* fileSystem_hpp */

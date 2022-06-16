//
//  App.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 17.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "App.hpp"

 
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
 
#include "App.hpp"

 
App::App(int argc, char const** argv)
{
    _fs.setRoot(argv[0]);
    _window.create(sf::VideoMode(1920, 1080, 32), "Midi Player");
    
}


void App::config(){
    std::cout << R"(
 _______________________________________________________________
|| | | ||| | ||| | | ||| | ||| | | ||| | ||| | | ||| | ||| | | ||
||_|_|_|||_|_|||_|_|_|||_|_|||_|_|_|||_|_|||_|_|_|||_|_|||_|_|_||
| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |
|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|
)" << std::endl;
    std::cout << "Welcome to MidiPlayer!" << std::endl;
    
    int choice = 0;
    std::string playMode;
    
    std::cout << "\nCHOOSE: " << std::endl;
    std::cout << "1. Play from midi" << std::endl;
    std::cout << "2. Play from txt" << std::endl;
    
    
    while(playMode==""){
        int key_in;
        std::cin >> key_in;
        switch (key_in) {
            case 1:
                playMode="midi";
                break;
            case 2:
                playMode="txt";
                break;
            default:
                std::cout << "TRY AGAIN!" << std::endl;
        }
    }
    
    if(playMode=="midi"){
        std::cout << "CHOOSE FILE:" << std::endl;
        auto files = _fs.fileList(_fs.pathToMidis());
        int start = 1;
        for (auto& file:files){
            std::cout << start << ". " << file << std::endl;
            start++;
        }
        std::string file;
        while (file==""){
            int key_in = 0;
            std::cin >> key_in;
            if(key_in>=1 && key_in<=files.size()){
                file = files[key_in-1];
                
            } else {
                std::cout << "TRY AGAIN!" << std::endl;
            }
        }
        _fileName = file;
        _fileDir = _fs.pathToMidis();
        _song = _parser.parseFromMidi(_fileName, _fileDir);
    }
    
    if(playMode=="txt"){
        std::cout << "CHOOSE FILE:" << std::endl;
        auto files = _fs.fileList(_fs.pathToTxt());
        int start = 1;
        for (auto& file:files){
            std::cout << start << ". " << file << std::endl;
            start++;
        }
        std::string file;
        while (file==""){
            int key_in = 0;
            std::cin >> key_in;
            if(key_in>=1 && key_in<=files.size()){
                file = files[key_in-1];
                
            } else {
                std::cout << "TRY AGAIN!" << std::endl;
            }
        }
        _song = _parser.parseFromTxt(_fs.pathToTxt()+file);
        std::filesystem::path path = _generator.generateMidi(_song, _fs.pathToGeneratedMidis());
        _fileName = path.filename();
        _fileDir = path.parent_path();
        _fileDir+="/";
    }
    
    
    
    
    std::cout << "CHOOSE SYNTH:" << std::endl;
    int start = 1;
    auto files = _fs.fileList(_fs.pathToSynths());
    for (auto& file:files){
        std::cout << start << ". " << file << std::endl;
        start++;
    }
    while(_synthPath==""){
        int key_in = 0;
        std::cin >> key_in;
        if(key_in>=1 && key_in<=files.size()){
            _synthPath = _fs.pathToSynths()+files[key_in-1];
        } else {
            std::cout << "TRY AGAIN!" << std::endl;
        }
    }
    
    std::cout << "\nCHOOSE: " << std::endl;
    std::cout << "1. Vertical mode" << std::endl;
    std::cout << "2. Horizontal mode" << std::endl;
    
    
    while(_visMode==""){
        int key_in;
        std::cin >> key_in;
        switch (key_in) {
            case 1:
                _visMode="ver";
                break;
            case 2:
                _visMode="hor";
                break;
            default:
                std::cout << "TRY AGAIN!" << std::endl;
        }
    }
}


void App::start(){
    sf::Font font;
    if (!font.loadFromFile(_fs.getFont()))
    {
        std::cout << "Font error";
        return 1;
    }
    MidiPlayer player = MidiPlayer(_synthPath, _fileDir, _fileName);
    GUI gui = GUI(player, _window, font);
    HorizontalVisAlgorithm h_vis = HorizontalVisAlgorithm(player, _window);
    VerticalVisAlgorithm v_vis = VerticalVisAlgorithm(player, _window);
    
    if (player.hasError()) {
      std::cout<<player.getError();
      return 1;
    }
    double gain = 1.0;
    player.setGain(gain);
    
    
    while (_window.isOpen()) {
      sf::Event sfEvent;
        
          while (_window.pollEvent(sfEvent)) {
            if (sfEvent.type == sf::Event::Closed)
              _window.close();

              if (sfEvent.type == sf::Event::KeyPressed) {
              switch (sfEvent.key.code) {
                    case sf::Keyboard::Escape:
                  {
                    _window.close();
                    break;
                  }

                case sf::Keyboard::F12:
                  {
                    sf::Image screen = _window.capture();
                    screen.saveToFile("screenshot.png");

                    break;
                  }

                  case sf::Keyboard::Up:
                  {
                      if (gain<1){
                          gain+=0.05;
                          std::cout << gain << std::endl;
                          player.setGain(gain);
                      }

                      break;

                  }
                  case sf::Keyboard::Down:
                  {
                      if (gain>=0.05){
                          gain-=0.05;
                          std::cout << gain << std::endl;
                          player.setGain(gain);
                      }
                      break;

                  }

                case sf::Keyboard::Space:
                  {
                    if (player.getStatus() != sf::SoundStream::Playing)
                      player.play();
                    else
                      player.pause();
                    break;
                  }
              }
            }
          }
        

        
        _window.clear();
        if(_visMode=="ver"){
            v_vis.draw();
        }
        if(_visMode=="hor"){
            h_vis.draw();
        }
        gui.draw();
        _window.display();
    }

    player.stop();

    return 0;
}

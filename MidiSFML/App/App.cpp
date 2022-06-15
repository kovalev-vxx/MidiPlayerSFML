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
        _filePath = _fs.pathToMidis()+file;
        _song = _parser.parseFromMidi(file, _fs.pathToMidis());
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
        _filePath = _generator.generateMidi(_song, _fs.pathToGeneratedMidis());
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
    
}


void App::start(){
    auto WIDHT = _window.getSize().x;
    auto HEIGHT = _window.getSize().y;
    VisAlgorith vis = VisAlgorith(_song, WIDHT, HEIGHT);
    sf::RectangleShape line;
    line.setSize(sf::Vector2f(5, HEIGHT));
    line.setPosition(sf::Vector2f(WIDHT/2, 0));
    
    
    sf::RectangleShape rectOff;
    rectOff.setSize(sf::Vector2f(WIDHT/2, HEIGHT));
    rectOff.setPosition(sf::Vector2f(0, 0));
    rectOff.setFillColor(sf::Color(127,127,127));
    
    sfmidi::Midi player(_synthPath, _filePath);
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

                case sf::Keyboard::Left:
                  {
                    sf::Int32 playingOffset =
                      player.getPlayingOffset().asMilliseconds();
                    if (playingOffset > 4000) {
                      player.setPlayingOffset
                        (sf::milliseconds(playingOffset - 4000));
                    }
                    else
                      player.setPlayingOffset(sf::Time::Zero);

                    break;
                  }

                case sf::Keyboard::Right:
                  {
                    sf::Int32 playingOffset =
                      player.getPlayingOffset().asMilliseconds();

                    player.setPlayingOffset
                      (sf::milliseconds(playingOffset + 4000));

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
            
        _window.draw(line);



        int nowTime = player.getPlayingOffset().asMilliseconds();
        for (auto& noteRect:vis.updateRects(nowTime)){
            _window.draw(noteRect);
        }

        
        _window.draw(rectOff, sf::BlendMultiply);

        _window.display();
    }

    player.stop();

    return 0;
}

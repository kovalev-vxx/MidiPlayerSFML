////////////////////////////////
// sfMidiTest 1.0.2           //
// Copyright � Kerli Low 2012 //
////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// License:                                                                 //
// sfMidiTest                                                               //
// Copyright (c) 2012 Kerli Low                                             //
// kerlilow@gmail.com                                                       //
//                                                                          //
// This software is provided 'as-is', without any express or implied        //
// warranty. In no event will the authors be held liable for any damages    //
// arising from the use of this software.                                   //
//                                                                          //
// Permission is granted to anyone to use this software for any purpose,    //
// including commercial applications, and to alter it and redistribute it   //
// freely, subject to the following restrictions:                           //
//                                                                          //
//    1. The origin of this software must not be misrepresented; you must   //
//    not claim that you wrote the original software. If you use this       //
//    software in a product, an acknowledgment in the product documentation //
//    would be appreciated but is not required.                             //
//                                                                          //
//    2. Altered source versions must be plainly marked as such, and must   //
//    not be misrepresented as being the original software.                 //
//                                                                          //
//    3. This notice may not be removed or altered from any source          //
//    distribution.                                                         //
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <SFML/Graphics.hpp>
#include "sfMidi.h"
#include "MidiPlayer/MidiGenerator.hpp"
#include "MidiPlayer/Song.hpp"
#include "App/App.hpp"
#include "App/AppStartState.hpp"
#include "MidiPlayer/Parser.hpp"


int main(int argc, char const** argv)
{
//    App app = App(argc, argv);
//    app.pushState(new AppStartState(&app));
//    app.gameLoop();
//    return 0;
  sf::RenderWindow sfApp(sf::VideoMode(800, 600, 32), "SFML Window");
    fileSystem fs;
    fs.setRoot(argv[0]);

  sfApp.setFramerateLimit(60);

  sf::Sprite spr_bg;

  sf::Texture tex_bg;
  tex_bg.loadFromFile(fs.getRoot()+"res/bg.png");
  spr_bg.setTexture(tex_bg);
  spr_bg.setPosition(40.0f, 100.0f);

    
    Parser parser;
    Song s = parser.parseFromTxt("song.txt");
    MidiGenerator generator;
    generator.generateMidi(s);
    
    
    std::map<int, Chord> chordsOn = {{0, Chord({Note("C3"), Note("E3"), Note("G3")})},
        {1000, Chord({Note("E3"), Note("G3"), Note("B3")})},
        {2000, Chord({Note("C3"), Note("E3"), Note("G3")})},
        {3000, Chord({Note("E3"), Note("G3"), Note("B3")})}
    };
    
    std::map<int, Chord> chordsOff = {{1000, Chord({Note("C3"), Note("E3"), Note("G3")})},
        {2000, Chord({Note("E3"), Note("G3"), Note("B3")})},
        {3000, Chord({Note("C3"), Note("E3"), Note("G3")})},
        {4000, Chord({Note("E3"), Note("G3"), Note("B3")})}
    };
    
    std::map<int, Chord> chordsOn1 = {
        {0, Chord({Note("C4")})},
        {1000, Chord({Note("E4")})},
        {2000, Chord({Note("G4")})},
        {3000, Chord({Note("B4")})},
    };
    
    std::map<int, Chord> chordsOff1 = {
        {250, Chord({Note("C4")})},
        {2000, Chord({Note("E4")})},
        {3000, Chord({Note("G4")})},
        {4000, Chord({Note("B4")})},
    };
    
    
    SongLine songLine = SongLine(chordsOn, chordsOff);
    SongLine songLine1 = SongLine(chordsOn1, chordsOff1);
    Song mySong = Song({songLine, songLine1}, 120);
    
    generator.generateMidi(mySong);
    


    sfmidi::Midi testMidi(fs.getRoot()+"res/synths/Touhou.sf2", fs.getRoot()+"res/midis/Pirates of the Caribbean - He's a Pirate (3).mid");
    if (testMidi.hasError()) {
      std::cout<<testMidi.getError();
      return 1;
    }
    double gain = 1.0;
      testMidi.setGain(gain);
  testMidi.play();

  while (sfApp.isOpen()) {
    sf::Event sfEvent;
    while (sfApp.pollEvent(sfEvent)) {
      if (sfEvent.type == sf::Event::Closed)
        sfApp.close();

        if (sfEvent.type == sf::Event::KeyPressed) {
        switch (sfEvent.key.code) {
              case sf::Keyboard::Escape:
            {
              sfApp.close();
              break;
            }

          case sf::Keyboard::F12:
            {
              sf::Image screen = sfApp.capture();
              screen.saveToFile("screenshot.png");

              break;
            }

            case sf::Keyboard::Up:
            {
                if (gain<1){
                    gain+=0.05;
                    std::cout << gain << std::endl;
                    testMidi.setGain(gain);
                }

                break;

            }
            case sf::Keyboard::Down:
            {
                if (gain>=0.05){
                    gain-=0.05;
                    std::cout << gain << std::endl;
                    testMidi.setGain(gain);
                }
                break;

            }

          case sf::Keyboard::Left:
            {
              sf::Int32 playingOffset =
                testMidi.getPlayingOffset().asMilliseconds();
              if (playingOffset > 4000) {
                testMidi.setPlayingOffset
                  (sf::milliseconds(playingOffset - 4000));
              }
              else
                testMidi.setPlayingOffset(sf::Time::Zero);

              break;
            }

          case sf::Keyboard::Right:
            {
              sf::Int32 playingOffset =
                testMidi.getPlayingOffset().asMilliseconds();

              testMidi.setPlayingOffset
                (sf::milliseconds(playingOffset + 4000));

              break;
            }

          case sf::Keyboard::Space:
            {
              if (testMidi.getStatus() != sf::SoundStream::Playing)
                testMidi.play();
              else
                testMidi.pause();
              break;
            }
        }
      }
    }

    sfApp.clear();

    sfApp.draw(spr_bg);

    sfApp.display();
  }

  testMidi.stop();

  return 0;
}

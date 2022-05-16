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


int main()
{
  sf::RenderWindow sfApp(sf::VideoMode(800, 600, 32), "SFML Window");

  sfApp.setFramerateLimit(60);

  sf::Sprite spr_bg;
  
  sf::Texture tex_bg;
  tex_bg.loadFromFile("res/bg.png");
  spr_bg.setTexture(tex_bg);
    MidiGenerator m;
    m.generateMidi();
  spr_bg.setPosition(40.0f, 100.0f);
    
    
    SongLine sl = SongLine({{0, Chord({Note(50)}, 1)}, {1000, Chord({Note(55)}, 1)}}, {{1000, Chord({Note(50)}, 1)}, {2000, Chord({Note(55)}, 0.5)}}, 1, 1);
    
    SongLine sl2 = SongLine({{0, Chord({Note(62)}, 1)}, {1000, Chord({Note(67)}, 1)}}, {{1000, Chord({Note(62)}, 1)}, {2000, Chord({Note(67)}, 0.5)}}, 1, 1);
    
    Song song = Song({sl, sl2}, 120, "Hello", 0.1);

    sfmidi::Midi testMidi("res/synths/Touhou.sf2",
                          m.generateTestMidi(song));
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

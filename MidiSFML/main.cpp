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
#include <chrono>


int main(int argc, char const** argv)
{
//    App app = App(argc, argv);
//    app.pushState(new AppStartState(&app));
//    app.gameLoop();
//    return 0;


    fileSystem fss;
    fss.setRoot(argv[0]);
    std::cout << fss.getRoot() << std::endl;
    std::cout << fss.pathToResousers() << std::endl;
    std::cout << fss.pathToSynths() << std::endl;
    std::cout << fss.pathToMidis() << std::endl;
    std::cout << fss.pathToGeneratedMidis() << std::endl;
    Parser parser;
    MidiGenerator generator;
    
    
    int WIDHT = 1280;
    int HEIGHT = 720;

    std::vector<sf::RectangleShape> noteRects;
    
    struct tNote{
        int midiValue;
        int duration;
        int timeOn;
        int timeOff;
        bool played;
    };
    
    std::vector<tNote> notesWithDurations;
    
    for(auto& line:test.getLines()){
        for(int i=0; i<line.getNotesOn().size();i++){
            tNote n;
            n.midiValue = line.getNotesOn()[i].getMidiValue();
            n.timeOn = line.getNotesOn()[i].getAbsoluteTime();
            n.timeOff = line.getNotesOff()[i].getAbsoluteTime();
            n.duration = n.timeOff-n.timeOn;
            n.played = false;
            notesWithDurations.push_back(n);
        }
    }
    
    Song test = parser.parseFromMidi("BOURREE-1.mid", fss.pathToMidis());
    std::cout << test.getTitle() << std::endl;
//    for(auto& line:test.getLines()){
//        for(int i=0; i<line.getNotesOn().size();i++){
//            int midiValue = line.getNotesOn()[i].getMidiValue();
//            int startTime = line.getNotesOn()[i].getAbsoluteTime();
//            int duration = line.getNotesOff()[i].getAbsoluteTime()- line.getNotesOn()[i].getAbsoluteTime();
//            sf::RectangleShape rect;
//            rect.setSize(sf::Vector2f(WIDHT/127, duration));
//            rect.setPosition(sf::Vector2f(WIDHT/127*midiValue, startTime));
//            rect.setFillColor(sf::Color(255, 0, 0));
//            rect.setOutlineColor(sf::Color(0,0,0));
//            noteRects.push_back(rect);
//        }
//    }
    

    
//    for(auto& line:test.getLines()){
//        for(int i=0; i<line.getNotesOn().size();i++){
//            int midiValue = line.getNotesOn()[i].getMidiValue();
//            int startTime = line.getNotesOn()[i].getAbsoluteTime();
//            int duration = line.getNotesOff()[i].getAbsoluteTime()- line.getNotesOn()[i].getAbsoluteTime();
//            sf::RectangleShape rect;
//            rect.setSize(sf::Vector2f(duration/WIDHT*100, HEIGHT/127));
//            rect.setPosition(sf::Vector2f(startTime/WIDHT*100, HEIGHT/127*midiValue));
//            rect.setFillColor(sf::Color(255, 0, 0));
//            rect.setOutlineColor(sf::Color(0,0,0));
//            noteRects.push_back(rect);
//        }
//    }

    
    
    
    
//    std::string genSong = generator.generateMidi(test, fss.pa);
//    return 0;
    
    
    sf::RenderWindow sfApp(sf::VideoMode(WIDHT, HEIGHT, 32), "SFML Window");
    
//    sfApp.getSize()
      fileSystem fs;
      fs.setRoot(argv[0]);


    sf::Sprite spr_bg;

    sf::Texture tex_bg;
    tex_bg.loadFromFile(fs.getRoot()+"res/bg.png");
    spr_bg.setTexture(tex_bg);
    spr_bg.setPosition(40.0f, 100.0f);
    
    sfApp.setFramerateLimit(60);
    


    sfmidi::Midi testMidi(fss.pathToSynths()+"Touhou.sf2", fss.pathToMidis()+"BOURREE-1.mid");
    if (testMidi.hasError()) {
      std::cout<<testMidi.getError();
      return 1;
    }
    double gain = 1.0;
      testMidi.setGain(gain);
//  testMidi.play();
    sf::Clock clock;
    double offset = 0;
    int lastPlayerOffset = 0;
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
      offset = testMidi.getPlayingOffset().asMilliseconds()-lastPlayerOffset;
      for (auto& noteRect:noteRects){
          sfApp.draw(noteRect);
//          noteRect.setPosition(sf::Vector2f(noteRect.getPosition().x-offset, noteRect.getPosition().y));
      }
      lastPlayerOffset = testMidi.getPlayingOffset().asMilliseconds();

      
//      std::cout << testMidi.getPlayingOffset().asMilliseconds() << std::endl;
//      for(auto& notes:notesWithSamePlayTime){
//          if(notes.first<=lastPlayerOffset){
//              std::cout << "TIME: " << notes.first << std::endl;
//              for(auto& note:notes.second){
//                  std::cout << "n: " << note.getMidiValue() << std::endl;
//              }
//          }
//      }
      
      
//      for(auto& line:test.getLines()){
//          for(auto& note: line.getNotesOn()){
//              if(note.getAbsoluteTime()<=lastPlayerOffset){
//                  std::cout << "TIME: " << note.getAbsoluteTime() << " " << note.getMidiValue()  << std::endl;
//              }
//          }
//      }
      
      for(auto& note:notesWithDurations){
          if(lastPlayerOffset>=note.timeOn && lastPlayerOffset<=note.timeOff && note.played == false){
              std::cout << "TIME: " << note.timeOff << " " << note.midiValue   << std::endl;
              note.played = true;
          }
      }


      sfApp.display();
      
      
  }

  testMidi.stop();

  return 0;
}

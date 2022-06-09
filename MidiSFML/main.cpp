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
#include <thread>


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
    
    
    int WIDHT = 1920;
    int HEIGHT = 1080;

    std::vector<sf::RectangleShape> noteRects;
    
    struct tNote{
        int midiValue;
        int duration;
        int timeOn;
        int timeOff;
        bool played;
        sf::Color color;
    };
    
    std::vector<tNote> notesWithDurations;
    std::map<int, int> offsets;
    int totalDuration = 0;
    
//    Song songFromTxt = parser.parseFromTxt(fss.getRoot()+"song.txt");
//    generator.generateMidi(songFromTxt, fss.getRoot());
    
    Song test = parser.parseFromMidi("toto-africa.mid", fss.pathToMidis());
    std::cout << test.getTitle() << std::endl;
    
    double coof = 1;
    
    int ins = 0;
    int ins_cout = test.getLines().size();
    
    sf::Clock c;
    
    std::cout << "NOTES START: " << c.restart().asMilliseconds() << std::endl;

    for(auto& line:test.getLines()){
        
        std::vector<int> timesOff;
        for (auto& noteOff: line.getNotesOff()){
            timesOff.push_back(noteOff.getAbsoluteTime());
        }
        
        int note_n = 0;
        for(auto& noteOn: line.getNotesOn()){
            tNote n;
            n.midiValue = noteOn.getMidiValue();
            n.timeOn = noteOn.getAbsoluteTime();
            n.timeOff = *(timesOff.begin()+note_n);
            n.duration = n.timeOff-n.timeOn;
            n.played = false;
            n.color = sf::Color(230, 255/ins_cout*ins, 255/ins_cout*ins);
            notesWithDurations.push_back(n);
            note_n+=1;
        }
        ins+=1;
    }
 
    


    
    std::cout << "NOTES END: " << c.restart().asMilliseconds() << std::endl;
    std::cout << "SORT START: " << c.restart().asMilliseconds() << std::endl;
    
    std::sort(notesWithDurations.begin(), notesWithDurations.end(), [](tNote n1, tNote n2){
        return n1.timeOn < n2.timeOn;
    });
    
    std::cout << "SORT END: " << c.restart().asMilliseconds() << std::endl;
    

    //VERTICAL
//    for(auto& note:notesWithDurations){
//        sf::RectangleShape rect;
//        std::cout << "NOTE: " << note.midiValue << " timeOn: " << note.timeOn << std::endl;
//        rect.setSize(sf::Vector2f(WIDHT/127, note.duration/2));
//        rect.setPosition(sf::Vector2f(WIDHT/127*note.midiValue, (note.timeOn-notesWithDurations[0].timeOn+HEIGHT/2)));
//        rect.setFillColor(note.color);
//        rect.setOutlineColor(sf::Color(0,0,0));
//        noteRects.push_back(rect);
//    }
    
    
    std::cout << "RECT START: " << c.restart().asMilliseconds() << std::endl;
    //GORIZONTAL
    for(auto& note:notesWithDurations){
        sf::RectangleShape rect;
//        std::cout << "NOTE: " << note.midiValue << " timeOn: " << note.timeOn << std::endl;
        rect.setSize(sf::Vector2f(note.duration/2, HEIGHT/127));
        rect.setPosition(sf::Vector2f(note.timeOn-notesWithDurations[0].timeOn+WIDHT/2, (HEIGHT/127*note.midiValue)));
        rect.setFillColor(note.color);
        rect.setOutlineColor(sf::Color(0,0,0));
        noteRects.push_back(rect);
    }
    std::cout << "RECT END: " << c.restart().asMilliseconds() << std::endl;
    
    
    
    
    

    
    
    //VERTICAL
//    sf::RectangleShape line;
//    line.setSize(sf::Vector2f(WIDHT, 5));
//    line.setPosition(sf::Vector2f(0, HEIGHT/2));
//
//
//    sf::RectangleShape rectOff;
//    rectOff.setSize(sf::Vector2f(WIDHT, HEIGHT/2));
//    rectOff.setPosition(sf::Vector2f(0, 0));
//    rectOff.setFillColor(sf::Color(127,127,127));
    
    //GORIZONTAL
    sf::RectangleShape line;
    line.setSize(sf::Vector2f(5, HEIGHT));
    line.setPosition(sf::Vector2f(WIDHT/2, 0));
    
    
    sf::RectangleShape rectOff;
    rectOff.setSize(sf::Vector2f(WIDHT/2, HEIGHT));
    rectOff.setPosition(sf::Vector2f(0, 0));
    rectOff.setFillColor(sf::Color(127,127,127));
    

    
    
    sf::RenderWindow sfApp(sf::VideoMode(WIDHT, HEIGHT, 32), "SFML Window");
    
      fileSystem fs;
      fs.setRoot(argv[0]);


    sf::Sprite spr_bg;

    sf::Texture tex_bg;
    tex_bg.loadFromFile(fs.getRoot()+"res/bg.png");
    spr_bg.setTexture(tex_bg);
    spr_bg.setPosition(40.0f, 100.0f);
    
    sfApp.setFramerateLimit(100);
    
    
    


    sfmidi::Midi testMidi(fss.pathToSynths()+"Touhou.sf2", fss.pathToMidis()+"toto-africa.mid");
    if (testMidi.hasError()) {
      std::cout<<testMidi.getError();
      return 1;
    }
    double gain = 1.0;
      testMidi.setGain(gain);
//  testMidi.play();
    int nowTime = 0;
    int lastTime = 0;
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
      
      sfApp.draw(line);



      nowTime = testMidi.getPlayingOffset().asMilliseconds();
      int dt = nowTime-lastTime;
      //VERTICAL
//      for (auto& noteRect:noteRects){
//          sfApp.draw(noteRect);
//          if(testMidi.getStatus() == sf::SoundStream::Playing && nowTime>notesWithDurations[0].timeOn){
//              noteRect.setPosition(sf::Vector2f(noteRect.getPosition().x, noteRect.getPosition().y-dt));
//          }
//      }
      
      //GORIZONTAL
      for (auto& noteRect:noteRects){
          sfApp.draw(noteRect);
          if(testMidi.getStatus() == sf::SoundStream::Playing && nowTime>notesWithDurations[0].timeOn){
              noteRect.setPosition(sf::Vector2f(noteRect.getPosition().x-dt, noteRect.getPosition().y));
          }
      }
      
      if (nowTime != lastTime){
          lastTime = nowTime;
      }
      
      
      sfApp.draw(rectOff, sf::BlendMultiply);

      sfApp.display();
  }

  testMidi.stop();

  return 0;
}

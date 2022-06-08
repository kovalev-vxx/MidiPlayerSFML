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
    std::map<int, int> offsets;
    int totalDuration = 0;
    
    Song test = parser.parseFromMidi("clair_after_gtp.mid", fss.pathToMidis());
    std::cout << test.getTitle() << std::endl;
    
    double coof = 1;
    
    for(auto& line:test.getLines()){
        for(int i=0; i<line.getNotesOn().size();i++){
            tNote n;
            n.midiValue = line.getNotesOn()[i].getMidiValue();
            n.timeOn = line.getNotesOn()[i].getAbsoluteTime();
            n.timeOff = line.getNotesOff()[i].getAbsoluteTime();
            n.duration = n.timeOff-n.timeOn;
            n.played = false;
            offsets[n.timeOn] = n.duration;
            if(n.duration<offsets[n.timeOn]){
                offsets[n.timeOn] = n.duration;
            }
            totalDuration+=n.duration;
            notesWithDurations.push_back(n);
        }
    }
    
    
    for (auto& n: notesWithDurations){
        std::cout << "ON: " <<n.timeOn << " V: " << n.midiValue << " D: " << n.duration << " OFFSET: " << offsets[n.timeOn] << std::endl;
    }
    
//
//    for(auto& line:test.getLines()){
//        int s = 0;
//        for(int i=0; i<line.getNotesOn().size();i++){
//            if (i==0){
//                s = line.getNotesOn()[0].getAbsoluteTime();
//            }
//            int midiValue = line.getNotesOn()[i].getMidiValue();
//            int startTime = line.getNotesOn()[i].getAbsoluteTime();
//            int duration = line.getNotesOff()[i].getAbsoluteTime()- line.getNotesOn()[i].getAbsoluteTime();
//            sf::RectangleShape rect;
//            rect.setSize(sf::Vector2f(WIDHT/127, duration*HEIGHT/totalDuration*2));
//            rect.setPosition(sf::Vector2f(WIDHT/127*midiValue, (startTime-s)*HEIGHT/totalDuration*2));
//            rect.setFillColor(sf::Color(255, 0, 0));
//            rect.setOutlineColor(sf::Color(0,0,0));
//            noteRects.push_back(rect);
//        }
//    }
    
    for(auto& note:notesWithDurations){
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(WIDHT/127, note.duration/2));
        rect.setPosition(sf::Vector2f(WIDHT/127*note.midiValue, (note.timeOn-notesWithDurations[0].timeOn+HEIGHT/2)));
        rect.setFillColor(sf::Color(255, 0, 0));
        rect.setOutlineColor(sf::Color(0,0,0));
        noteRects.push_back(rect);
    }

    
    
    std::cout << totalDuration << std::endl;
//    for(auto& line:test.getLines()){
//        for(int i=0; i<line.getNotesOn().size();i++){
//            int midiValue = line.getNotesOn()[i].getMidiValue();
//            int startTime = line.getNotesOn()[i].getAbsoluteTime();
//            int duration = line.getNotesOff()[i].getAbsoluteTime()- line.getNotesOn()[i].getAbsoluteTime();
//            sf::RectangleShape rect;
//            rect.setSize(sf::Vector2f(duration, HEIGHT/127));
//            rect.setPosition(sf::Vector2f(startTime, HEIGHT/127*midiValue));
//            rect.setFillColor(sf::Color(255, 0, 0));
//            rect.setOutlineColor(sf::Color(0,0,0));
//            noteRects.push_back(rect);
//        }
//    }
//
    
    sf::RectangleShape line;
    line.setSize(sf::Vector2f(WIDHT, 5));
    line.setPosition(sf::Vector2f(0, HEIGHT/2));
    

    
    
    sf::RenderWindow sfApp(sf::VideoMode(WIDHT, HEIGHT, 32), "SFML Window");
    
      fileSystem fs;
      fs.setRoot(argv[0]);


    sf::Sprite spr_bg;

    sf::Texture tex_bg;
    tex_bg.loadFromFile(fs.getRoot()+"res/bg.png");
    spr_bg.setTexture(tex_bg);
    spr_bg.setPosition(40.0f, 100.0f);
    
    sfApp.setFramerateLimit(60);
    


    sfmidi::Midi testMidi(fss.pathToSynths()+"Essential Keys-sforzando-v9.6.sf2", fss.pathToMidis()+"clair_after_gtp.mid");
    if (testMidi.hasError()) {
      std::cout<<testMidi.getError();
      return 1;
    }
    double gain = 1.0;
      testMidi.setGain(gain);
//  testMidi.play();
    sf::Clock clock;
    double offset = 0;
    int lastTimeon = 0;
    int now = 0;
    int last = 0;
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



      now = testMidi.getPlayingOffset().asMilliseconds();
      
//      for(auto& note:notesWithDurations){
//          if(now>=note.timeOn && now<=note.timeOff && note.played == false){
//
//              int size = 0;
//              if (note.timeOn != lastTimeon){
//                  std::cout << "TIME: " << note.timeOff << " " << note.midiValue  << "OFFSET:" << offsets[note.timeOn] << " NEW OFFSER: " << note.timeOff- note.timeOn << std::endl;
//                  std::cout << "CHANGE" << std::endl;
//                  offset = offsets[note.timeOn];
//                  for (auto& noteRect:noteRects){
////                      std::cout << noteRect.getPosition().x << " " << noteRect.getPosition().y << std::endl;
////                      std::cout << "NOW: " << now << " LAST: " << last << " dif " << now-last << " y: " << noteRect.getPosition().y << " SIZE:" <<noteRect.getSize().y<< std::endl;
//                      int dif = now-lastTimeon;
//                      dif = note.timeOff- note.timeOn;
//                      size+=noteRect.getSize().y;
//                      noteRect.setPosition(sf::Vector2f(noteRect.getPosition().x, noteRect.getPosition().y-dif));
//                  }
//                  lastTimeon = note.timeOn;
//
//                  std::cout << "TOTAL-SIZE:" << size << std::endl;
//              }
//              note.played = true;
//
//          }
//      }
      
      float dt = now-last;
      std::cout << now-last << std::endl;
      for (auto& noteRect:noteRects){
          sfApp.draw(noteRect);
          if(testMidi.getStatus() == sf::SoundStream::Playing && now>notesWithDurations[0].timeOn){
//              std::cout << dt << std::endl;
              noteRect.setPosition(sf::Vector2f(noteRect.getPosition().x, noteRect.getPosition().y-dt));
          }
      }
      
      if (now != last){
          last = now;
      }
      
      


      sfApp.display();
  }

  testMidi.stop();

  return 0;
}

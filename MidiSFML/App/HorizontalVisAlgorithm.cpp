//
//  HorizontalVisAlgorithm.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 09.06.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "HorizontalVisAlgorithm.hpp"


HorizontalVisAlgorithm::HorizontalVisAlgorithm(MidiPlayer& player, sf::RenderWindow& window){
    _window = &window;
    auto windowHeight = _window->getSize().y;
    auto windowWidth = _window->getSize().x;
    _player = &player;
    
    _nowTime = 0;
    _lastTime = 0;
    _dt = 0;
    struct tNote{
        int midiValue;
        int duration;
        int timeOn;
        int timeOff;
        bool played;
        sf::Color color;
    };
    
    std::vector<tNote> notesWithDurations;
    
    int ins = 0;
    int ins_cout = _player->getSong().getLines().size();
    for(auto& line:_player->getSong().getLines()){
        
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
            n.color = sf::Color(200, 255/ins_cout*ins, 255/ins_cout*ins);
            if(ins%2==0){
                n.color = sf::Color(255/ins_cout*ins, 200, 255/ins_cout*ins);
            }
            if(ins%3==0){
                n.color = sf::Color(255/ins_cout*ins, 255/ins_cout*ins, 200);
            }
            
            notesWithDurations.push_back(n);
            note_n+=1;
        }
        ins+=1;
    }
    
    std::sort(notesWithDurations.begin(), notesWithDurations.end(), [](tNote n1, tNote n2){
        return n1.timeOn < n2.timeOn;
    });
    
    _start = notesWithDurations.front().timeOn;
    
    
    for(auto& note:notesWithDurations){
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(note.duration/2, windowHeight/127));
        rect.setPosition(sf::Vector2f(note.timeOn-_start+windowWidth/2, (windowHeight/127*note.midiValue)));
        rect.setFillColor(note.color);
        rect.setOutlineColor(sf::Color(0,0,0));
        _rects.push_back(rect);
    }
    
    _separator.setSize(sf::Vector2f(5, windowHeight));
    _separator.setPosition(sf::Vector2f(windowWidth/2, 0));
    

    _blendRect.setSize(sf::Vector2f(windowWidth/2, windowHeight));
    _blendRect.setPosition(sf::Vector2f(0, 0));
    _blendRect.setFillColor(sf::Color(127,127,127));
}


void HorizontalVisAlgorithm::draw(){
    _nowTime = _player->getPlayingOffset().asMilliseconds();
    
    _dt = _nowTime-_lastTime;
    if(_nowTime>=_start){
        for (auto& noteRect:_rects){
            noteRect.setPosition(sf::Vector2f(noteRect.getPosition().x-_dt, noteRect.getPosition().y));
        }
    }
    if (_nowTime != _lastTime){
        _lastTime = _nowTime;
    }
    
    for (auto& noteRect:_rects){
        _window->draw(noteRect);
    }
    
    _window->draw(_separator);
    _window->draw(_blendRect, sf::BlendMultiply);
}



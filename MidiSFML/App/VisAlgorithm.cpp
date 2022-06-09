//
//  VisAlgorithm.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 09.06.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "VisAlgorithm.hpp"


VisAlgorith::VisAlgorith(Song& song, int windowWidth, int windowHeight){
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
    int ins_cout = song.getLines().size();
    for(auto& line:song.getLines()){
        
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
}


std::vector<sf::RectangleShape> VisAlgorith::updateRects(int nowTime){
    _dt = nowTime-_lastTime;
    if(nowTime>=_start){
        for (auto& noteRect:_rects){
            noteRect.setPosition(sf::Vector2f(noteRect.getPosition().x-_dt, noteRect.getPosition().y));
        }
    }
    if (nowTime != _lastTime){
        _lastTime = nowTime;
    }
    return _rects;
}

std::vector<sf::RectangleShape> VisAlgorith::getRects(){
    return _rects;
}



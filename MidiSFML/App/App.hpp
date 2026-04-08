//
//  App.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 17.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#ifndef App_hpp
#define App_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "../Utils/fileSystem.hpp"
#include "../MidiPlayer/MidiGenerator.hpp"
#include "HorizontalVisAlgorithm.hpp"
#include "VerticalVisAlgorithm.hpp"
#include "ThreeDVisAlgorithm.hpp"
#include "../MidiPlayer/MidiPlayer.hpp"
#include <filesystem>
#include "IVisAlgorithm.hpp"
#include "GUI.hpp"
#include "Menu.hpp"
#include <memory>

enum class AppState {
    MENU,
    PLAYING
};

class App
{
    private:

    sf::RenderWindow _window;
    sf::Font _font;
    fileSystem _fs;

    AppState _state;
    std::unique_ptr<Menu> _menu;

    std::unique_ptr<MidiPlayer> _player;
    std::unique_ptr<GUI> _gui;
    std::unique_ptr<HorizontalVisAlgorithm> _hVis;
    std::unique_ptr<VerticalVisAlgorithm> _vVis;
    std::unique_ptr<ThreeDVisAlgorithm> _3dVis;
    std::string _visMode;
    double _gain;

    void startPlayback(const MenuResult& result);
    void stopPlayback();

    public:

    App(int argc, char const** argv);
    void run();
};

#endif /* App_hpp */

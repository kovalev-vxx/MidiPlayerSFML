//
//  Menu.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 26.03.2026.
//

#ifndef Menu_hpp
#define Menu_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "../Utils/fileSystem.hpp"
#include "../MidiPlayer/Parser.hpp"
#include "../MidiPlayer/Song.hpp"
#include "../MidiPlayer/MidiGenerator.hpp"

enum class MenuScreen {
    PLAY_MODE,
    FILE_SELECT,
    SYNTH_SELECT,
    VIS_MODE,
    DONE
};

struct MenuResult {
    std::string fileName;
    std::string fileDir;
    std::string synthPath;
    std::string visMode;
    Song song;
    bool ready = false;
};

class Menu {
private:
    sf::RenderWindow* _window;
    sf::Font* _font;
    fileSystem* _fs;
    Parser _parser;
    MidiGenerator _generator;

    MenuScreen _screen;
    int _selectedIndex;
    int _scrollOffset;
    int _hoveredIndex;
    std::vector<std::string> _items;
    std::string _title;

    std::string _playMode;
    MenuResult _result;

    std::vector<std::string> _midiFiles;
    std::vector<std::string> _txtFiles;
    std::vector<std::string> _synthFiles;

    sf::Text _titleText;
    sf::Text _bannerText;
    sf::Text _hintText;

    static constexpr float ITEMS_START_Y = 300.f;
    static constexpr float ITEM_HEIGHT = 50.f;

    int maxVisibleItems() const;
    void ensureSelectedVisible();
    int itemIndexAtY(float y) const;
    void selectItem();
    void loadScreen();
    void drawItems();
    void drawScrollbar();

public:
    Menu(sf::RenderWindow& window, sf::Font& font, fileSystem& fs);
    void reset();
    void handleEvent(const sf::Event& event);
    void draw();
    bool isDone() const;
    MenuResult getResult() const;
};

#endif /* Menu_hpp */

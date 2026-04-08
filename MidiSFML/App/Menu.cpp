//
//  Menu.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 26.03.2026.
//

#include "Menu.hpp"
#include <filesystem>
#include <algorithm>

Menu::Menu(sf::RenderWindow& window, sf::Font& font, fileSystem& fs) {
    _window = &window;
    _font = &font;
    _fs = &fs;

    _titleText.setFont(font);
    _titleText.setCharacterSize(50);
    _titleText.setFillColor(sf::Color::White);

    _bannerText.setFont(font);
    _bannerText.setCharacterSize(18);
    _bannerText.setFillColor(sf::Color(150, 150, 150));
    _bannerText.setString(
        " _______________________________________________________________\n"
        "|| | | ||| | ||| | | ||| | ||| | | ||| | ||| | | ||| | ||| | | ||\n"
        "||_|_|_|||_|_|||_|_|_|||_|_|||_|_|_|||_|_|||_|_|_|||_|_|||_|_|_||\n"
        "| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | |\n"
        "|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|"
    );
    _bannerText.setPosition(
        (_window->getSize().x - _bannerText.getLocalBounds().width) / 2.f,
        30
    );

    _hintText.setFont(font);
    _hintText.setCharacterSize(22);
    _hintText.setFillColor(sf::Color(120, 120, 120));
    _hintText.setString("Up/Down/Scroll - navigate    Enter/Click - select    Esc - back");

    reset();
}

void Menu::reset() {
    _screen = MenuScreen::PLAY_MODE;
    _selectedIndex = 0;
    _scrollOffset = 0;
    _hoveredIndex = -1;
    _playMode = "";
    _result = MenuResult();
    loadScreen();
}

int Menu::maxVisibleItems() const {
    float availableHeight = _window->getSize().y - 120 - ITEMS_START_Y;
    return std::max(1, (int)(availableHeight / ITEM_HEIGHT));
}

void Menu::ensureSelectedVisible() {
    int maxVis = maxVisibleItems();
    if (_selectedIndex < _scrollOffset) {
        _scrollOffset = _selectedIndex;
    }
    if (_selectedIndex >= _scrollOffset + maxVis) {
        _scrollOffset = _selectedIndex - maxVis + 1;
    }
}

int Menu::itemIndexAtY(float y) const {
    if (y < ITEMS_START_Y) return -1;
    int rel = (int)((y - ITEMS_START_Y) / ITEM_HEIGHT);
    int idx = rel + _scrollOffset;
    if (idx < 0 || idx >= (int)_items.size()) return -1;
    if (rel >= maxVisibleItems()) return -1;
    return idx;
}

void Menu::loadScreen() {
    _selectedIndex = 0;
    _scrollOffset = 0;
    _hoveredIndex = -1;
    _items.clear();

    switch (_screen) {
        case MenuScreen::PLAY_MODE:
            _title = "CHOOSE PLAY MODE";
            _items.push_back("Play from MIDI");
            _items.push_back("Play from TXT");
            break;

        case MenuScreen::FILE_SELECT:
            if (_playMode == "midi") {
                _title = "CHOOSE MIDI FILE";
                _midiFiles = _fs->fileList(_fs->pathToMidis());
                _items = _midiFiles;
            } else {
                _title = "CHOOSE TXT FILE";
                _txtFiles = _fs->fileList(_fs->pathToTxt());
                _items = _txtFiles;
            }
            break;

        case MenuScreen::SYNTH_SELECT:
            _title = "CHOOSE SYNTH";
            _synthFiles = _fs->fileList(_fs->pathToSynths());
            _items = _synthFiles;
            break;

        case MenuScreen::VIS_MODE:
            _title = "CHOOSE VISUALIZATION";
            _items.push_back("Vertical mode");
            _items.push_back("Horizontal mode");
            break;

        case MenuScreen::DONE:
            break;
    }
}

void Menu::selectItem() {
    if (_items.empty()) return;

    switch (_screen) {
        case MenuScreen::PLAY_MODE:
            _playMode = (_selectedIndex == 0) ? "midi" : "txt";
            _screen = MenuScreen::FILE_SELECT;
            loadScreen();
            break;

        case MenuScreen::FILE_SELECT:
            if (_playMode == "midi") {
                _result.fileName = _midiFiles[_selectedIndex];
                _result.fileDir = _fs->pathToMidis();
                _result.song = _parser.parseFromMidi(_result.fileName, _result.fileDir);
            } else {
                std::string file = _txtFiles[_selectedIndex];
                _result.song = _parser.parseFromTxt(_fs->pathToTxt() + file);
                std::filesystem::path path = _generator.generateMidi(_result.song, _fs->pathToGeneratedMidis());
                _result.fileName = path.filename();
                _result.fileDir = path.parent_path().string() + "/";
            }
            _screen = MenuScreen::SYNTH_SELECT;
            loadScreen();
            break;

        case MenuScreen::SYNTH_SELECT:
            _result.synthPath = _fs->pathToSynths() + _synthFiles[_selectedIndex];
            _screen = MenuScreen::VIS_MODE;
            loadScreen();
            break;

        case MenuScreen::VIS_MODE:
            _result.visMode = (_selectedIndex == 0) ? "ver" : "hor";
            _result.ready = true;
            _screen = MenuScreen::DONE;
            break;

        case MenuScreen::DONE:
            break;
    }
}

void Menu::handleEvent(const sf::Event& event) {
    if (_screen == MenuScreen::DONE) return;

    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::Up:
                if (_selectedIndex > 0) {
                    _selectedIndex--;
                    ensureSelectedVisible();
                }
                break;

            case sf::Keyboard::Down:
                if (_selectedIndex < (int)_items.size() - 1) {
                    _selectedIndex++;
                    ensureSelectedVisible();
                }
                break;

            case sf::Keyboard::Return:
                selectItem();
                break;

            case sf::Keyboard::Escape: {
                switch (_screen) {
                    case MenuScreen::PLAY_MODE:
                        break;
                    case MenuScreen::FILE_SELECT:
                        _screen = MenuScreen::PLAY_MODE;
                        loadScreen();
                        break;
                    case MenuScreen::SYNTH_SELECT:
                        _screen = MenuScreen::FILE_SELECT;
                        loadScreen();
                        break;
                    case MenuScreen::VIS_MODE:
                        _screen = MenuScreen::SYNTH_SELECT;
                        loadScreen();
                        break;
                    case MenuScreen::DONE:
                        break;
                }
                break;
            }

            default:
                break;
        }
    }
    else if (event.type == sf::Event::MouseMoved) {
        int idx = itemIndexAtY((float)event.mouseMove.y);
        _hoveredIndex = idx;
        if (idx >= 0) {
            _selectedIndex = idx;
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            int idx = itemIndexAtY((float)event.mouseButton.y);
            if (idx >= 0) {
                _selectedIndex = idx;
                selectItem();
            }
        }
    }
    else if (event.type == sf::Event::MouseWheelScrolled) {
        int maxScroll = std::max(0, (int)_items.size() - maxVisibleItems());
        if (event.mouseWheelScroll.delta < 0) {
            _scrollOffset = std::min(_scrollOffset + 3, maxScroll);
        } else if (event.mouseWheelScroll.delta > 0) {
            _scrollOffset = std::max(_scrollOffset - 3, 0);
        }
    }
}

void Menu::draw() {
    // Banner
    _window->draw(_bannerText);

    // Title
    _titleText.setString(_title);
    _titleText.setPosition(
        (_window->getSize().x - _titleText.getLocalBounds().width) / 2.f,
        200
    );
    _window->draw(_titleText);

    // Menu items
    drawItems();

    // Scrollbar if needed
    if ((int)_items.size() > maxVisibleItems()) {
        drawScrollbar();
    }

    // Hint at bottom
    _hintText.setPosition(
        (_window->getSize().x - _hintText.getLocalBounds().width) / 2.f,
        _window->getSize().y - 60
    );
    _window->draw(_hintText);
}

void Menu::drawItems() {
    int maxVis = maxVisibleItems();
    int endIdx = std::min(_scrollOffset + maxVis, (int)_items.size());

    for (int i = _scrollOffset; i < endIdx; i++) {
        int row = i - _scrollOffset;

        sf::Text itemText;
        itemText.setFont(*_font);
        itemText.setCharacterSize(35);

        std::string prefix = (i == _selectedIndex) ? "> " : "  ";
        itemText.setString(prefix + _items[i]);

        if (i == _selectedIndex) {
            itemText.setFillColor(sf::Color(100, 200, 255));
        } else if (i == _hoveredIndex) {
            itemText.setFillColor(sf::Color(170, 220, 240));
        } else {
            itemText.setFillColor(sf::Color(200, 200, 200));
        }

        itemText.setPosition(
            (_window->getSize().x - itemText.getLocalBounds().width) / 2.f,
            ITEMS_START_Y + row * ITEM_HEIGHT
        );
        _window->draw(itemText);
    }

    // Scroll indicators
    if (_scrollOffset > 0) {
        sf::Text upArrow;
        upArrow.setFont(*_font);
        upArrow.setCharacterSize(28);
        upArrow.setFillColor(sf::Color(120, 120, 120));
        upArrow.setString("^ ^ ^");
        upArrow.setPosition(
            (_window->getSize().x - upArrow.getLocalBounds().width) / 2.f,
            ITEMS_START_Y - 35
        );
        _window->draw(upArrow);
    }

    if (endIdx < (int)_items.size()) {
        sf::Text downArrow;
        downArrow.setFont(*_font);
        downArrow.setCharacterSize(28);
        downArrow.setFillColor(sf::Color(120, 120, 120));
        downArrow.setString("v v v");
        downArrow.setPosition(
            (_window->getSize().x - downArrow.getLocalBounds().width) / 2.f,
            ITEMS_START_Y + maxVis * ITEM_HEIGHT + 5
        );
        _window->draw(downArrow);
    }
}

void Menu::drawScrollbar() {
    int totalItems = (int)_items.size();
    int maxVis = maxVisibleItems();
    float listHeight = maxVis * ITEM_HEIGHT;

    // Track
    sf::RectangleShape track;
    track.setSize(sf::Vector2f(6, listHeight));
    track.setPosition(_window->getSize().x - 40, ITEMS_START_Y);
    track.setFillColor(sf::Color(60, 60, 60));
    _window->draw(track);

    // Thumb
    float thumbRatio = (float)maxVis / totalItems;
    float thumbHeight = std::max(30.f, listHeight * thumbRatio);
    float scrollRange = listHeight - thumbHeight;
    int maxScroll = totalItems - maxVis;
    float thumbY = ITEMS_START_Y;
    if (maxScroll > 0) {
        thumbY += scrollRange * ((float)_scrollOffset / maxScroll);
    }

    sf::RectangleShape thumb;
    thumb.setSize(sf::Vector2f(6, thumbHeight));
    thumb.setPosition(_window->getSize().x - 40, thumbY);
    thumb.setFillColor(sf::Color(120, 120, 120));
    _window->draw(thumb);
}

bool Menu::isDone() const {
    return _screen == MenuScreen::DONE;
}

MenuResult Menu::getResult() const {
    return _result;
}

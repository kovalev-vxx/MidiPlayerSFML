//
//  App.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 17.05.2022.
//  Copyright © 2022 Valery Kovalev. All rights reserved.
//

#include "App.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>


App::App(int argc, char const** argv)
{
    _fs.setRoot(argv[0]);
    _window.create(sf::VideoMode(1920, 1080, 32), "Midi Player");
    _state = AppState::MENU;
    _gain = 1.0;

    if (!_font.loadFromFile(_fs.getFont()))
    {
        std::cout << "Font error" << std::endl;
        return;
    }

    _menu = std::make_unique<Menu>(_window, _font, _fs);
}

void App::startPlayback(const MenuResult& result) {
    _player = std::make_unique<MidiPlayer>(result.synthPath, result.fileDir, result.fileName);

    if (_player->hasError()) {
        std::cout << _player->getError() << std::endl;
        _player.reset();
        _menu->reset();
        _state = AppState::MENU;
        return;
    }

    _gui = std::make_unique<GUI>(*_player, _window, _font);
    _hVis = std::make_unique<HorizontalVisAlgorithm>(*_player, _window);
    _vVis = std::make_unique<VerticalVisAlgorithm>(*_player, _window);
    _visMode = result.visMode;

    _gain = 1.0;
    _player->setGain(_gain);

    _state = AppState::PLAYING;
}

void App::stopPlayback() {
    if (_player) {
        _player->stop();
    }
    _player.reset();
    _gui.reset();
    _hVis.reset();
    _vVis.reset();
    _menu->reset();
    _state = AppState::MENU;
}

void App::run() {
    while (_window.isOpen()) {
        sf::Event sfEvent;

        while (_window.pollEvent(sfEvent)) {
            if (sfEvent.type == sf::Event::Closed) {
                _window.close();
                continue;
            }

            if (_state == AppState::MENU) {
                _menu->handleEvent(sfEvent);

                if (_menu->isDone()) {
                    startPlayback(_menu->getResult());
                }
            }
            else if (_state == AppState::PLAYING) {
                GUIAction action = _gui->handleEvent(sfEvent);
                if (action == GUIAction::TOGGLE_PLAY) {
                    if (_player->getStatus() != sf::SoundStream::Playing)
                        _player->play();
                    else
                        _player->pause();
                } else if (action == GUIAction::BACK) {
                    stopPlayback();
                }

                if (sfEvent.type == sf::Event::KeyPressed) {
                    switch (sfEvent.key.code) {
                        case sf::Keyboard::Escape:
                            stopPlayback();
                            break;

                        case sf::Keyboard::F12: {
                            sf::Image screen = _window.capture();
                            screen.saveToFile("screenshot.png");
                            break;
                        }

                        case sf::Keyboard::Up:
                            if (_gain < 1) {
                                _gain += 0.05;
                                _player->setGain(_gain);
                            }
                            break;

                        case sf::Keyboard::Down:
                            if (_gain >= 0.05) {
                                _gain -= 0.05;
                                _player->setGain(_gain);
                            }
                            break;

                        case sf::Keyboard::Space:
                            if (_player->getStatus() != sf::SoundStream::Playing)
                                _player->play();
                            else
                                _player->pause();
                            break;

                        default:
                            break;
                    }
                }
            }
        }

        _window.clear();

        if (_state == AppState::MENU) {
            _menu->draw();
        }
        else if (_state == AppState::PLAYING) {
            if (_visMode == "ver") {
                _vVis->draw();
            }
            if (_visMode == "hor") {
                _hVis->draw();
            }
            _gui->draw();
        }

        _window.display();
    }

    if (_player) {
        _player->stop();
    }
}

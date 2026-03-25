# MidiPlayerSFML

MIDI-плеер на SFML + FluidSynth с визуализацией нот.

## Зависимости

Загрузить SoundFont-инструменты в `MidiSFML/res/synths/`:

https://drive.google.com/file/d/1WFxPySbZe9__ImUMtvuS020tcaqRuPkc/view?usp=sharing

Установить инструменты сборки (macOS):

```bash
brew install cmake pkg-config glib
```

## Сборка и запуск

```bash
cmake -B build
cmake --build build -j$(sysctl -n hw.ncpu)
./build/MidiPlayerSFML
```

SFML и FluidSynth скачиваются и собираются локально в `build/` при первой сборке.

При запуске программа предложит в терминале выбрать MIDI-файл, синтезатор и режим визуализации.

## Управление

| Клавиша | Действие                  |
|---------|---------------------------|
| Space   | Пауза / воспроизведение   |
| Up      | Увеличить громкость       |
| Down    | Уменьшить громкость       |
| F12     | Скриншот                  |
| Escape  | Выход                     |

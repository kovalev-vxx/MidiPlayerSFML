cmake_minimum_required(VERSION 3.20)
project(MidiPlayerSFML LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# --- SFML 2.6.2 (local build) ---
FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        2.6.2
    GIT_SHALLOW    TRUE
)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(sfml)

# --- FluidSynth 2.3.6 (local build) ---
FetchContent_Declare(
    fluidsynth
    GIT_REPOSITORY https://github.com/FluidSynth/fluidsynth.git
    GIT_TAG        v2.3.6
    GIT_SHALLOW    TRUE
)
set(enable-libsndfile OFF CACHE BOOL "" FORCE)
set(enable-dbus OFF CACHE BOOL "" FORCE)
set(enable-sdl2 OFF CACHE BOOL "" FORCE)
set(enable-readline OFF CACHE BOOL "" FORCE)
set(enable-aufile OFF CACHE BOOL "" FORCE)
set(enable-network OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Populate FluidSynth source, then patch for CMake 4.x compatibility
FetchContent_Populate(fluidsynth)

# Fix gentables cmake_minimum_required for CMake 4.x
set(_gentables_cmake "${fluidsynth_SOURCE_DIR}/src/gentables/CMakeLists.txt")
file(READ "${_gentables_cmake}" _gentables_content)
string(REPLACE "cmake_minimum_required(VERSION 3.1)" "cmake_minimum_required(VERSION 3.5)" _gentables_content "${_gentables_content}")
file(WRITE "${_gentables_cmake}" "${_gentables_content}")

add_subdirectory(${fluidsynth_SOURCE_DIR} ${fluidsynth_BINARY_DIR})

# --- Sources ---
set(SOURCES
    MidiSFML/main.cpp
    MidiSFML/App/App.cpp
    MidiSFML/App/GUI.cpp
    MidiSFML/App/Menu.cpp
    MidiSFML/App/HorizontalVisAlgorithm.cpp
    MidiSFML/App/VerticalVisAlgorithm.cpp
    MidiSFML/MidiPlayer/Chord.cpp
    MidiSFML/MidiPlayer/MidiGenerator.cpp
    MidiSFML/MidiPlayer/MidiPlayer.cpp
    MidiSFML/MidiPlayer/Note.cpp
    MidiSFML/MidiPlayer/Parser.cpp
    MidiSFML/MidiPlayer/Song.cpp
    MidiSFML/MidiPlayer/SongLine.cpp
    MidiSFML/Utils/fileSystem.cpp
    MidiSFML/libs/sfMidi/Midi.cpp
    MidiSFML/libs/sfMidi/Loader.cpp
    MidiSFML/libs/sfMidi/Music.cpp
    MidiSFML/libs/midifile/Binasc.cpp
    MidiSFML/libs/midifile/MidiEvent.cpp
    MidiSFML/libs/midifile/MidiEventList.cpp
    MidiSFML/libs/midifile/MidiFile.cpp
    MidiSFML/libs/midifile/MidiMessage.cpp
    MidiSFML/libs/midifile/Options.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
    MidiSFML
    MidiSFML/libs
    MidiSFML/libs/sfMidi
    MidiSFML/libs/midifile
    MidiSFML/libs/cxxmidi
    MidiSFML/libs/cxxmidi/output
    MidiSFML/libs/cxxmidi/guts
    MidiSFML/MidiPlayer
    MidiSFML/App
    MidiSFML/Utils
    ${fluidsynth_BINARY_DIR}/include
    ${fluidsynth_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-audio
    sfml-system
    sfml-window
    libfluidsynth
)

# Copy resources next to executable
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/MidiSFML/res
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)

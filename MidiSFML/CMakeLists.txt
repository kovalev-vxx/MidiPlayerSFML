# Generated on 2022-6-3 16:26:50 by version 0.9.0 of mulle-xcode-to-cmake
# Command line:
#    mulle-xcode-to-cmake -b export /Users/kovalev.vxx/Code/XCode/MidiSFML/MidiSFML.xcodeproj


cmake_minimum_required (VERSION 3.4)
project(. C)


##
## MidiSFML Files
##

# uncomment this for mulle-objc to search libraries first
# set( CMAKE_FIND_FRAMEWORK "LAST")

set( STATIC_DEPENDENCIES
)

if( NOT SKIP_FIND_FOUNDATION_LIBRARY)
   find_library( FOUNDATION_LIBRARY Foundation)
   message( STATUS "FOUNDATION_LIBRARY is ${FOUNDATION_LIBRARY}")
endif()
if( NOT SKIP_FIND_FLUIDSYNTH_3_1_0_LIBRARY)
   find_library( FLUIDSYNTH_3_1_0_LIBRARY fluidsynth.3.1.0)
   message( STATUS "FLUIDSYNTH_3_1_0_LIBRARY is ${FLUIDSYNTH_3_1_0_LIBRARY}")
endif()

set( DEPENDENCIES
${FLUIDSYNTH_3_1_0_LIBRARY}
${FOUNDATION_LIBRARY}
)

set( SOURCES
        App/AnimationHandler.cpp
        App/App.cpp
        App/AppPlayerState.cpp
        App/AppStartState.cpp
        App/AppState.cpp
        App/TextureManager.cpp
        MidiPlayer/Chord.cpp
        MidiPlayer/MidiGenerator.cpp
        MidiPlayer/MidiPlayer.cpp
        MidiPlayer/Note.cpp
        MidiPlayer/Parser.cpp
        MidiPlayer/Song.cpp
        MidiPlayer/SongLine.cpp
        Utils/fileSystem.cpp
        libs/midifile/Binasc.cpp
        libs/midifile/MidiEvent.cpp
        libs/midifile/MidiEventList.cpp
        libs/midifile/MidiFile.cpp
        libs/midifile/MidiMessage.cpp
        libs/midifile/Options.cpp
        libs/sfMidi/Loader.cpp
        libs/sfMidi/Midi.cpp
        libs/sfMidi/Music.cpp
        main.cpp
)


##
## MidiSFML
##

add_executable(.
${SOURCES}
${PUBLIC_HEADERS}
${PROJECT_HEADERS}
${PRIVATE_HEADERS}
${RESOURCES}
)

target_link_libraries(.
${STATIC_DEPENDENCIES}
${DEPENDENCIES}
)

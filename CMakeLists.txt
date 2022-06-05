cmake_minimum_required(VERSION 3.16.3)
project(MidiPlayerSFML)

set(CMAKE_CXX_STANDARD 17)

include_directories(MidiSFML/libs)
include_directories(MidiSFML/libs/fluidsynth)
include_directories(MidiSFML/libs/sfMidi)
include_directories(MidiSFML/MidiPlayer)

add_executable(MidiPlayerSFML
        MidiSFML/libs/cxxmidi/guts/endianness.hpp
        MidiSFML/libs/cxxmidi/guts/player_base.hpp
        MidiSFML/libs/cxxmidi/guts/simulator.hpp
        MidiSFML/libs/cxxmidi/guts/utils.hpp

        MidiSFML/libs/cxxmidi/output/abstract.hpp
        MidiSFML/libs/cxxmidi/output/default.hpp
        MidiSFML/libs/cxxmidi/output/null.hpp
        MidiSFML/libs/cxxmidi/output/windows.hpp
        MidiSFML/libs/cxxmidi/output/linux/alsa.hpp

        MidiSFML/libs/cxxmidi/player/player_async.hpp
        MidiSFML/libs/cxxmidi/player/player_sync.hpp

        MidiSFML/libs/cxxmidi/converters.hpp
        MidiSFML/libs/cxxmidi/event.hpp
        MidiSFML/libs/cxxmidi/file.hpp
        MidiSFML/libs/cxxmidi/instrument.hpp
        MidiSFML/libs/cxxmidi/message.hpp
        MidiSFML/libs/cxxmidi/note.hpp
        MidiSFML/libs/cxxmidi/sysex.hpp
        MidiSFML/libs/cxxmidi/track.hpp

        MidiSFML/libs/fluidsynth/audio.h
        MidiSFML/libs/fluidsynth/event.h
        MidiSFML/libs/fluidsynth/gen.h
        MidiSFML/libs/fluidsynth/ladspa.h
        MidiSFML/libs/fluidsynth/log.h
        MidiSFML/libs/fluidsynth/midi.h
        MidiSFML/libs/fluidsynth/misc.h
        MidiSFML/libs/fluidsynth/mod.h
        MidiSFML/libs/fluidsynth/seq.h
        MidiSFML/libs/fluidsynth/seqbind.h
        MidiSFML/libs/fluidsynth/settings.h
        MidiSFML/libs/fluidsynth/sfont.h
        MidiSFML/libs/fluidsynth/shell.h
        MidiSFML/libs/fluidsynth/synth.h
        MidiSFML/libs/fluidsynth/types.h
        MidiSFML/libs/fluidsynth/version.h
        MidiSFML/libs/fluidsynth/voice.h

        MidiSFML/libs/midifile/Binasc.cpp
        MidiSFML/libs/midifile/Binasc.h
        MidiSFML/libs/midifile/MidiEvent.cpp
        MidiSFML/libs/midifile/MidiEvent.h
        MidiSFML/libs/midifile/MidiEventList.cpp
        MidiSFML/libs/midifile/MidiEventList.h
        MidiSFML/libs/midifile/MidiFile.cpp
        MidiSFML/libs/midifile/MidiFile.h
        MidiSFML/libs/midifile/MidiMessage.cpp
        MidiSFML/libs/midifile/MidiMessage.h
        MidiSFML/libs/midifile/Options.cpp
        MidiSFML/libs/midifile/Options.h

        MidiSFML/libs/sfMidi/Error.h
        MidiSFML/libs/sfMidi/Loader.cpp
        MidiSFML/libs/sfMidi/Loader.h
        MidiSFML/libs/sfMidi/Midi.cpp
        MidiSFML/libs/sfMidi/Midi.h
        MidiSFML/libs/sfMidi/Music.cpp
        MidiSFML/libs/sfMidi/Music.h

        MidiSFML/libs/fluidsynth.h
        MidiSFML/libs/sfMidi.h

        MidiSFML/MidiPlayer/Chord.cpp
        MidiSFML/MidiPlayer/Chord.hpp
        MidiSFML/MidiPlayer/MidiGenerator.cpp
        MidiSFML/MidiPlayer/MidiGenerator.hpp
        MidiSFML/MidiPlayer/MidiPlayer.cpp
        MidiSFML/MidiPlayer/MidiPlayer.hpp
        MidiSFML/MidiPlayer/Note.cpp
        MidiSFML/MidiPlayer/Note.hpp
        MidiSFML/MidiPlayer/Parser.cpp
        MidiSFML/MidiPlayer/Parser.hpp
        MidiSFML/MidiPlayer/Song.cpp
        MidiSFML/MidiPlayer/Song.hpp
        MidiSFML/MidiPlayer/SongLine.cpp
        MidiSFML/MidiPlayer/SongLine.hpp

        MidiSFML/main.cpp)

# - Find fluidsynth
# Find the native fluidsynth includes and library
#
#  FLUIDSYNTH_INCLUDE_DIR - where to find fluidsynth.h
#  FLUIDSYNTH_LIBRARIES   - List of libraries when using fluidsynth.
#  FLUIDSYNTH_FOUND       - True if fluidsynth found.


IF (FLUIDSYNTH_INCLUDE_DIR AND FLUIDSYNTH_LIBRARIES)
    # Already in cache, be silent
    SET(FluidSynth_FIND_QUIETLY TRUE)
ENDIF (FLUIDSYNTH_INCLUDE_DIR AND FLUIDSYNTH_LIBRARIES)

FIND_PATH(FLUIDSYNTH_INCLUDE_DIR fluidsynth.h)

FIND_LIBRARY(FLUIDSYNTH_LIBRARIES NAMES fluidsynth )
MARK_AS_ADVANCED( FLUIDSYNTH_LIBRARIES FLUIDSYNTH_INCLUDE_DIR )

# handle the QUIETLY and REQUIRED arguments and set FLUIDSYNTH_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(FluidSynth DEFAULT_MSG FLUIDSYNTH_LIBRARIES FLUIDSYNTH_INCLUDE_DIR)
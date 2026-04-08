//
//  ThreeDVisAlgorithm.hpp
//  MidiSFML
//
//  Created by Valery Kovalev on 08.04.2026.
//

#ifndef ThreeDVisAlgorithm_hpp
#define ThreeDVisAlgorithm_hpp

#include <SFML/Graphics.hpp>
#include <vector>
#include "../MidiPlayer/MidiPlayer.hpp"
#include "IVisAlgorithm.hpp"

// 3-D perspective piano-roll visualisation.
//
//  World-space axes
//    X  – MIDI pitch  (0 … 127) × PITCH_SCALE
//    Y  – track index × (−TRACK_STEP)  → higher tracks sit visually above lower ones
//    Z  – time offset from "now"  (future = +Z, past = −Z)  × TIME_SCALE
//
//  Notes are 3-D boxes.  When a note's front face crosses Z = 0 (the playhead)
//  a burst of particles is emitted to visualise the moment of attack.

class ThreeDVisAlgorithm : public IVisAlgorithm {
public:
    ThreeDVisAlgorithm(MidiPlayer& player, sf::RenderWindow& window);
    void draw() override;

private:
    // ---- note geometry -------------------------------------------------------
    struct NoteData {
        float     x, width, y;
        int       timeOn, timeOff;
        sf::Color topColor, frontColor;
    };

    // ---- particle system -----------------------------------------------------
    struct Particle {
        sf::Vector2f pos;       // screen-space position
        sf::Vector2f vel;       // screen-space velocity  (px / s)
        sf::Color    color;
        float        life;      // seconds remaining
        float        maxLife;   // initial lifetime
        float        size;      // half-side of the square sprite
    };

    // ---- helpers -------------------------------------------------------------
    sf::Vector2f project(float wx, float wy, float wz) const;
    void         drawBackground(float nowMs)                  const;
    void         drawPlayhead()                              const;
    void         drawNote(const NoteData& note, float nowMs) const;
    void         emitParticles(const NoteData& note);
    void         updateParticles(float dtSec);
    void         drawParticles()                             const;

    // ---- data ----------------------------------------------------------------
    sf::RenderWindow* _window;
    MidiPlayer*       _player;
    std::vector<NoteData> _notes;      // sorted by timeOn ascending
    int               _trackCount;
    float             _lastNowMs;      // for dt and edge-detection

    std::vector<Particle> _particles;

    // Camera & projection
    float _camX, _camY, _camZ, _focal, _scX, _scY;

    // Background animation
    float _bgTime;       // accumulated seconds
    float _intensity;    // smoothed music intensity 0–1

    // World-space constants
    static constexpr float PITCH_SCALE    = 10.f;
    static constexpr float TRACK_STEP     = 72.f;
    static constexpr float NOTE_HEIGHT    = 22.f;
    static constexpr float TIME_SCALE     = 0.45f;
    static constexpr float CLIP_FAR_DIST  = 3200.f;
    static constexpr float CLIP_NEAR_OFF  = 20.f;

    // Particle constants
    static constexpr int   PARTICLES_PER_NOTE = 28;
    static constexpr float PARTICLE_LIFE_MIN  = 0.35f;
    static constexpr float PARTICLE_LIFE_MAX  = 0.75f;
    static constexpr float PARTICLE_SPEED_MIN = 80.f;
    static constexpr float PARTICLE_SPEED_MAX = 380.f;
};

#endif /* ThreeDVisAlgorithm_hpp */

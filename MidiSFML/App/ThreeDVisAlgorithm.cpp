//
//  ThreeDVisAlgorithm.cpp
//  MidiSFML
//
//  Created by Valery Kovalev on 08.04.2026.
//

#include "ThreeDVisAlgorithm.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>

// ---------------------------------------------------------------------------
// Construction
// ---------------------------------------------------------------------------

ThreeDVisAlgorithm::ThreeDVisAlgorithm(MidiPlayer& player, sf::RenderWindow& window)
    : _player(&player), _window(&window), _lastNowMs(-1.f),
      _bgTime(0.f), _intensity(0.f)
{
    const float W = (float)_window->getSize().x;
    const float H = (float)_window->getSize().y;

    _camX  = 64.f * PITCH_SCALE;
    _camY  = -190.f;
    _camZ  = -460.f;
    _focal = 470.f;
    _scX   = W * 0.5f;
    _scY   = H * 0.63f;

    _trackCount = (int)_player->getSong().getLines().size();
    int insCount = std::max(1, _trackCount);
    int ins = 0;

    for (auto& line : _player->getSong().getLines()) {
        float trackY = -(float)ins * TRACK_STEP;

        sf::Color base(200,
                       (sf::Uint8)(255 / insCount * ins),
                       (sf::Uint8)(255 / insCount * ins));
        if (ins % 2 == 0)
            base = sf::Color((sf::Uint8)(255 / insCount * ins), 200,
                             (sf::Uint8)(255 / insCount * ins));
        if (ins % 3 == 0)
            base = sf::Color((sf::Uint8)(255 / insCount * ins),
                             (sf::Uint8)(255 / insCount * ins), 200);

        sf::Color front((sf::Uint8)(base.r * 6 / 10),
                        (sf::Uint8)(base.g * 6 / 10),
                        (sf::Uint8)(base.b * 6 / 10));

        auto ons  = line.getNotesOn();
        auto offs = line.getNotesOff();
        int  n    = std::min((int)ons.size(), (int)offs.size());

        for (int i = 0; i < n; ++i) {
            NoteData nd;
            nd.x          = ons[i].getMidiValue() * PITCH_SCALE;
            nd.width      = PITCH_SCALE;
            nd.y          = trackY;
            nd.timeOn     = ons[i].getAbsoluteTime();
            nd.timeOff    = offs[i].getAbsoluteTime();
            nd.topColor   = base;
            nd.frontColor = front;
            _notes.push_back(nd);
        }
        ++ins;
    }

    std::sort(_notes.begin(), _notes.end(),
        [](const NoteData& a, const NoteData& b){ return a.timeOn < b.timeOn; });
}

// ---------------------------------------------------------------------------
// Projection
// ---------------------------------------------------------------------------

sf::Vector2f ThreeDVisAlgorithm::project(float wx, float wy, float wz) const {
    float dz = wz - _camZ;
    if (dz < 1.f) dz = 1.f;
    return { _scX + (wx - _camX) * _focal / dz,
             _scY + (wy - _camY) * _focal / dz };
}

// ---------------------------------------------------------------------------
// Quad helper
// ---------------------------------------------------------------------------

static void appendQuad(sf::VertexArray& va,
                       sf::Vector2f p0, sf::Vector2f p1,
                       sf::Vector2f p2, sf::Vector2f p3,
                       sf::Color c)
{
    va.append(sf::Vertex(p0, c)); va.append(sf::Vertex(p1, c)); va.append(sf::Vertex(p2, c));
    va.append(sf::Vertex(p0, c)); va.append(sf::Vertex(p2, c)); va.append(sf::Vertex(p3, c));
}

// ---------------------------------------------------------------------------
// Animated background mesh
// ---------------------------------------------------------------------------

void ThreeDVisAlgorithm::drawBackground(float nowMs) const {
    const int   COLS = 42, ROWS = 24;
    const float W    = (float)_window->getSize().x;
    const float H    = (float)_window->getSize().y;
    const float cw   = W / COLS, ch = H / ROWS;

    // Max channel value driven by smoothed intensity.
    // Even at zero intensity keep a tiny baseline so the mesh isn't pitch-black.
    const float maxBright = 20.f + _intensity * 100.f;

    sf::VertexArray va(sf::Triangles);
    va.resize((std::size_t)(COLS * ROWS * 6));
    std::size_t vi = 0;

    for (int row = 0; row <= ROWS; ++row) {
        for (int col = 0; col <= COLS; ++col) {
            // Normalised grid position.
            float bx = (float)col / COLS;
            float by = (float)row / ROWS;

            // Three overlapping wave layers – different speeds & axes.
            float w1 = std::sin(bx * 7.f  + _bgTime * 0.9f) * 0.5f + 0.5f;
            float w2 = std::cos(by * 5.f  + _bgTime * 1.3f) * 0.5f + 0.5f;
            float w3 = std::sin((bx - by) * 6.f + _bgTime * 0.6f) * 0.5f + 0.5f;
            float wave = (w1 + w2 + w3) / 3.f;           // 0..1

            // Hue shifts slowly over time; wave modulates it locally.
            float hue = _bgTime * 0.18f + wave * 2.2f;
            float r = std::sin(hue)           * 0.5f + 0.5f;
            float g = std::sin(hue + 2.094f)  * 0.5f + 0.5f;
            float b = std::sin(hue + 4.189f)  * 0.5f + 0.5f;

            sf::Color c((sf::Uint8)(r * maxBright),
                        (sf::Uint8)(g * maxBright),
                        (sf::Uint8)(b * maxBright));

            // We only need vertex colours; build quads on the fly.
            if (col < COLS && row < ROWS) {
                float x0 = col * cw,       y0 = row * ch;
                float x1 = (col + 1) * cw, y1 = (row + 1) * ch;

                // Recompute colour for the other three corners too.
                auto vertColor = [&](float bxv, float byv) -> sf::Color {
                    float v1 = std::sin(bxv * 7.f  + _bgTime * 0.9f)           * 0.5f + 0.5f;
                    float v2 = std::cos(byv * 5.f  + _bgTime * 1.3f)           * 0.5f + 0.5f;
                    float v3 = std::sin((bxv - byv) * 6.f + _bgTime * 0.6f)    * 0.5f + 0.5f;
                    float wv = (v1 + v2 + v3) / 3.f;
                    float hv = _bgTime * 0.18f + wv * 2.2f;
                    float rv = std::sin(hv)          * 0.5f + 0.5f;
                    float gv = std::sin(hv + 2.094f) * 0.5f + 0.5f;
                    float bv = std::sin(hv + 4.189f) * 0.5f + 0.5f;
                    return sf::Color((sf::Uint8)(rv * maxBright),
                                     (sf::Uint8)(gv * maxBright),
                                     (sf::Uint8)(bv * maxBright));
                };

                sf::Color c00 = vertColor((float)col / COLS,       (float)row / ROWS);
                sf::Color c10 = vertColor((float)(col+1) / COLS,   (float)row / ROWS);
                sf::Color c11 = vertColor((float)(col+1) / COLS,   (float)(row+1) / ROWS);
                sf::Color c01 = vertColor((float)col / COLS,       (float)(row+1) / ROWS);

                va[vi++] = sf::Vertex({x0, y0}, c00);
                va[vi++] = sf::Vertex({x1, y0}, c10);
                va[vi++] = sf::Vertex({x1, y1}, c11);
                va[vi++] = sf::Vertex({x0, y0}, c00);
                va[vi++] = sf::Vertex({x1, y1}, c11);
                va[vi++] = sf::Vertex({x0, y1}, c01);
            }
        }
    }
    va.resize(vi);
    _window->draw(va);
}

// ---------------------------------------------------------------------------
// Playhead  – a glowing line at Z = 0 per track
// ---------------------------------------------------------------------------

void ThreeDVisAlgorithm::drawPlayhead() const {
    if (_trackCount == 0) return;

    const float xMin = 0.f;
    const float xMax = 127.f * PITCH_SCALE;

    for (int t = 0; t < _trackCount; ++t) {
        float trackY = -(float)t * TRACK_STEP;

        // Project the two ends of the playhead line at this track's Y level.
        sf::Vector2f L = project(xMin, trackY, 0.f);
        sf::Vector2f R = project(xMax, trackY, 0.f);

        // Build a screen-space perpendicular so we can draw a thick glow band.
        sf::Vector2f dir = R - L;
        float        len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len < 1.f) continue;
        sf::Vector2f perp(-dir.y / len, dir.x / len);

        // Narrow glow band.
        for (float hw : {3.f, 1.5f}) {
            sf::Uint8 alpha = (hw > 2.f) ? 18 : 50;
            sf::Color gc(220, 200, 50, alpha);
            sf::VertexArray va(sf::Triangles, 6);
            auto tl = L - perp * hw;  auto tr = R - perp * hw;
            auto bl = L + perp * hw;  auto br = R + perp * hw;
            va[0] = sf::Vertex(tl, gc); va[1] = sf::Vertex(tr, gc); va[2] = sf::Vertex(br, gc);
            va[3] = sf::Vertex(tl, gc); va[4] = sf::Vertex(br, gc); va[5] = sf::Vertex(bl, gc);
            _window->draw(va);
        }

        // Dim core line.
        sf::Color core(200, 185, 80, 160);
        sf::Vertex seg[2] = { sf::Vertex(L, core), sf::Vertex(R, core) };
        _window->draw(seg, 2, sf::Lines);
    }
}

// ---------------------------------------------------------------------------
// Note box
// ---------------------------------------------------------------------------

void ThreeDVisAlgorithm::drawNote(const NoteData& note, float nowMs) const {
    const float nearClip = _camZ + CLIP_NEAR_OFF;
    const float farClip  = _camZ + CLIP_FAR_DIST;

    float zFrontOrig = (note.timeOn  - nowMs) * TIME_SCALE;
    float zBackOrig  = (note.timeOff - nowMs) * TIME_SCALE;

    if (zBackOrig  < nearClip) return;
    if (zFrontOrig > farClip)  return;

    bool  frontClipped = (zFrontOrig < nearClip);
    float zF = std::max(zFrontOrig, nearClip);
    float zB = std::min(zBackOrig,  farClip);
    if (zF >= zB) return;

    float avgZ   = (zF + zB) * 0.5f;
    float tDepth = std::max(0.f, std::min(1.f,
                    (avgZ - nearClip) / (farClip - nearClip)));
    float shade  = 1.0f - 0.52f * tDepth;

    bool isPlaying = (zFrontOrig <= 0.f && zBackOrig > 0.f);
    if (isPlaying) shade = std::min(1.f, shade + 0.28f);

    auto applyShade = [shade](sf::Color c) -> sf::Color {
        return sf::Color((sf::Uint8)(c.r * shade),
                         (sf::Uint8)(c.g * shade),
                         (sf::Uint8)(c.b * shade));
    };

    float xL = note.x,  xR = note.x + note.width;
    float yB = note.y,  yT = note.y - NOTE_HEIGHT;

    auto tBL = project(xL, yT, zB);
    auto tBR = project(xR, yT, zB);
    auto tFL = project(xL, yT, zF);
    auto tFR = project(xR, yT, zF);
    auto bFL = project(xL, yB, zF);
    auto bFR = project(xR, yB, zF);

    sf::VertexArray va(sf::Triangles);
    appendQuad(va, tBL, tBR, tFR, tFL, applyShade(note.topColor));
    if (!frontClipped)
        appendQuad(va, tFL, tFR, bFR, bFL, applyShade(note.frontColor));
    _window->draw(va);
}

// ---------------------------------------------------------------------------
// Particle emission  – called when note.timeOn crosses "now"
// ---------------------------------------------------------------------------

static float randF() { return (float)rand() / (float)RAND_MAX; }

void ThreeDVisAlgorithm::emitParticles(const NoteData& note) {
    // Corners of the front face at Z = 0 (the exact moment of attack).
    sf::Vector2f tL = project(note.x,               note.y - NOTE_HEIGHT, 0.f);
    sf::Vector2f tR = project(note.x + note.width,  note.y - NOTE_HEIGHT, 0.f);
    sf::Vector2f bL = project(note.x,               note.y,               0.f);
    sf::Vector2f bR = project(note.x + note.width,  note.y,               0.f);

    // Brightness-boosted version of the note colour.
    auto brighten = [](sf::Color c, float f) -> sf::Color {
        return sf::Color(
            (sf::Uint8)std::min(255.f, c.r * f),
            (sf::Uint8)std::min(255.f, c.g * f),
            (sf::Uint8)std::min(255.f, c.b * f));
    };

    for (int i = 0; i < PARTICLES_PER_NOTE; ++i) {
        Particle p;

        // Random point on the front face.
        float u = randF(), v = randF();
        p.pos = tL + (tR - tL) * u + (bL - tL) * v;

        // Random outward velocity with a slight upward screen-space bias.
        float angle = randF() * 2.f * 3.14159f;
        float speed = PARTICLE_SPEED_MIN +
                      randF() * (PARTICLE_SPEED_MAX - PARTICLE_SPEED_MIN);
        p.vel = sf::Vector2f(std::cos(angle) * speed,
                             std::sin(angle) * speed - 60.f);

        // Mix between note colour and near-white for variety.
        float whiteMix = randF() * 0.55f;
        sf::Color base = brighten(note.topColor, 1.4f);
        p.color = sf::Color(
            (sf::Uint8)(base.r + (255 - base.r) * whiteMix),
            (sf::Uint8)(base.g + (255 - base.g) * whiteMix),
            (sf::Uint8)(base.b + (255 - base.b) * whiteMix));

        p.maxLife = PARTICLE_LIFE_MIN + randF() * (PARTICLE_LIFE_MAX - PARTICLE_LIFE_MIN);
        p.life    = p.maxLife;
        p.size    = 1.5f + randF() * 3.f;

        _particles.push_back(p);
    }
}

// ---------------------------------------------------------------------------
// Particle update
// ---------------------------------------------------------------------------

void ThreeDVisAlgorithm::updateParticles(float dtSec) {
    constexpr float GRAVITY = 280.f;   // px / s²  downward screen drag

    for (auto& p : _particles) {
        p.vel.y += GRAVITY * dtSec;
        p.pos   += p.vel   * dtSec;
        p.life  -= dtSec;
    }

    _particles.erase(
        std::remove_if(_particles.begin(), _particles.end(),
                       [](const Particle& p){ return p.life <= 0.f; }),
        _particles.end());
}

// ---------------------------------------------------------------------------
// Particle draw  – batched into one VertexArray
// ---------------------------------------------------------------------------

void ThreeDVisAlgorithm::drawParticles() const {
    if (_particles.empty()) return;

    sf::VertexArray va(sf::Triangles);
    va.resize(_particles.size() * 6);
    std::size_t vi = 0;

    for (const auto& p : _particles) {
        float t  = p.life / p.maxLife;          // 1 = just born, 0 = dead
        float s  = p.size * t;                  // shrink over time
        sf::Uint8 a = (sf::Uint8)(255 * t * t); // quadratic fade

        sf::Color c(p.color.r, p.color.g, p.color.b, a);

        sf::Vector2f tl(p.pos.x - s, p.pos.y - s);
        sf::Vector2f tr(p.pos.x + s, p.pos.y - s);
        sf::Vector2f bl(p.pos.x - s, p.pos.y + s);
        sf::Vector2f br(p.pos.x + s, p.pos.y + s);

        va[vi++] = sf::Vertex(tl, c); va[vi++] = sf::Vertex(tr, c); va[vi++] = sf::Vertex(br, c);
        va[vi++] = sf::Vertex(tl, c); va[vi++] = sf::Vertex(br, c); va[vi++] = sf::Vertex(bl, c);
    }

    _window->draw(va);
}

// ---------------------------------------------------------------------------
// Main draw
// ---------------------------------------------------------------------------

void ThreeDVisAlgorithm::draw() {
    if (_trackCount == 0) return;

    float nowMs = (float)_player->getPlayingOffset().asMilliseconds();

    // --- timing ---------------------------------------------------------------
    float dtSec = 0.f;
    if (_lastNowMs >= 0.f && nowMs > _lastNowMs) {
        dtSec = (nowMs - _lastNowMs) / 1000.f;
    } else if (nowMs < _lastNowMs) {
        // Player was rewound – clear leftover particles.
        _particles.clear();
    }

    // Detect notes whose attack crossed Z=0 since the last frame.
    if (dtSec > 0.f) {
        for (auto& note : _notes) {
            if ((float)note.timeOn > _lastNowMs && (float)note.timeOn <= nowMs)
                emitParticles(note);
        }
    }

    _lastNowMs = nowMs;

    // --- music intensity (count active notes, smooth with attack/release) ----
    if (dtSec > 0.f) {
        int active = 0;
        for (auto& note : _notes) {
            float zF = (note.timeOn  - nowMs) * TIME_SCALE;
            float zB = (note.timeOff - nowMs) * TIME_SCALE;
            if (zF <= 0.f && zB > 0.f) ++active;
        }
        float target = std::min(1.f, active / 6.f);
        float rate   = (target > _intensity) ? dtSec * 7.f : dtSec * 1.8f;
        _intensity  += (target - _intensity) * std::min(1.f, rate);
        _bgTime     += dtSec;
    }

    // --- background -----------------------------------------------------------
    drawBackground(nowMs);

    // --- playhead -------------------------------------------------------------
    drawPlayhead();

    // --- notes  (far → near) --------------------------------------------------
    for (int i = (int)_notes.size() - 1; i >= 0; --i)
        drawNote(_notes[i], nowMs);

    // --- particles  (on top of everything) ------------------------------------
    updateParticles(dtSec);
    drawParticles();
}

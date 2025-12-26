#pragma once

#include "Effects/effect.h"

// Matrix effect with falling green particles and fading trails
class MatrixEffect : public Effect {
public:
    MatrixEffect(uint8_t width, uint8_t height);
    void Update(float dt) override;
    void Render(CRGB *leds, const MatrixMap &map) override;
    void Reset() override;

private:
    struct Particle {
        int8_t x;              // X position (column)
        float y;               // Y position (row, float for smooth movement)
        float speed;           // Fall speed
        uint8_t trailLength;   // Length of the trail
        bool active;           // Whether particle is active
    };

    uint8_t m_width;
    uint8_t m_height;
    float m_spawnTimer;        // Timer for spawning new particles
    
    static constexpr uint8_t MAX_PARTICLES = 4;
    Particle m_particles[MAX_PARTICLES];
    
    // Color configuration constants
    static constexpr uint8_t MATRIX_HEAD_HUE = 96;       // Bright green
    static constexpr uint8_t MATRIX_TRAIL_HUE = 96;      // Same green hue
    static constexpr uint8_t MATRIX_HEAD_SAT = 150;      // Desaturated (light green)
    static constexpr uint8_t MATRIX_TRAIL_SAT = 255;     // Fully saturated (dark green)
    static constexpr uint8_t MATRIX_HEAD_BRIGHT = 255;   // Maximum brightness for head
    static constexpr uint8_t MATRIX_TRAIL_BRIGHT = 180;  // Dimmer for trail start
    
    static constexpr float PARTICLE_MIN_SPEED = 3.0f;    // Min fall speed (pixels/sec)
    static constexpr float PARTICLE_MAX_SPEED = 6.0f;    // Max fall speed (pixels/sec)
    static constexpr uint8_t TRAIL_MIN_LENGTH = 3;       // Minimum trail length
    static constexpr uint8_t TRAIL_MAX_LENGTH = 8;       // Maximum trail length
    static constexpr float SPAWN_INTERVAL = 1.5f;        // Seconds between spawn attempts
    
    void spawnParticle();
    void updateParticle(Particle &p, float dt);
};

#include "Effects/matrix_effect.h"

MatrixEffect::MatrixEffect(uint8_t width, uint8_t height)
    : m_width(width), m_height(height), m_spawnTimer(0.0f) {
    // Initialize all particles as inactive
    for (uint8_t i = 0; i < MAX_PARTICLES; ++i) {
        m_particles[i].active = false;
    }
}

void MatrixEffect::Reset() {
    m_spawnTimer = 0.0f;
    // Deactivate all particles
    for (uint8_t i = 0; i < MAX_PARTICLES; ++i) {
        m_particles[i].active = false;
    }
}

void MatrixEffect::spawnParticle() {
    // Find an inactive particle slot
    for (uint8_t i = 0; i < MAX_PARTICLES; ++i) {
        if (!m_particles[i].active) {
            Particle &p = m_particles[i];
            p.active = true;
            p.x = random8(m_width);  // Random column
            p.y = -1.0f;             // Start just above the visible area
            p.speed = PARTICLE_MIN_SPEED + (random8() / 255.0f) * (PARTICLE_MAX_SPEED - PARTICLE_MIN_SPEED);
            p.trailLength = TRAIL_MIN_LENGTH + random8(TRAIL_MAX_LENGTH - TRAIL_MIN_LENGTH + 1);
            break;
        }
    }
}

void MatrixEffect::updateParticle(Particle &p, float dt) {
    if (!p.active) return;
    
    // Move particle down
    p.y += p.speed * dt;
    
    // Deactivate if completely off screen (head + trail)
    if (p.y - p.trailLength > m_height) {
        p.active = false;
    }
}

void MatrixEffect::Update(float dt) {
    // Update all active particles
    for (uint8_t i = 0; i < MAX_PARTICLES; ++i) {
        updateParticle(m_particles[i], dt);
    }
    
    // Spawn new particles periodically
    m_spawnTimer += dt;
    if (m_spawnTimer >= SPAWN_INTERVAL) {
        m_spawnTimer = 0.0f;
        spawnParticle();
    }
}

void MatrixEffect::Render(CRGB *leds, const MatrixMap &map) {
    // Clear all LEDs first
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            leds[map.index(x, y)] = CRGB::Black;
        }
    }
    
    // Render each active particle
    for (uint8_t i = 0; i < MAX_PARTICLES; ++i) {
        const Particle &p = m_particles[i];
        if (!p.active) continue;
        
        int headY = static_cast<int>(p.y);
        
        // Draw the trail first (bottom to top)
        for (uint8_t t = 0; t < p.trailLength; ++t) {
            int trailY = headY - t - 1;
            
            // Skip if off screen
            if (trailY < 0 || trailY >= m_height) continue;
            
            // Calculate fade factor (0.0 = tail end, 1.0 = near head)
            float fadeFactor = 1.0f - (static_cast<float>(t) / p.trailLength);
            fadeFactor = fadeFactor * fadeFactor; // Square for more dramatic fade
            
            // Trail: high saturation, fading brightness
            uint8_t brightness = static_cast<uint8_t>(MATRIX_TRAIL_BRIGHT * fadeFactor);
            
            uint16_t idx = map.index(p.x, trailY);
            leds[idx] = CHSV(MATRIX_TRAIL_HUE, MATRIX_TRAIL_SAT, brightness);
        }
        
        // Draw the head (brightest, less saturated)
        if (headY >= 0 && headY < m_height) {
            uint16_t idx = map.index(p.x, headY);
            leds[idx] = CHSV(MATRIX_HEAD_HUE, MATRIX_HEAD_SAT, MATRIX_HEAD_BRIGHT);
        }
    }
}

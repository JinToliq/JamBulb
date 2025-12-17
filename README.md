# JamBulb — ESP32-C3 Addressable LED Matrix

Quick notes:

- Hardware:
  - LED Data pin: change in `include/pins.h` (default `LED_PIN = 5`).
  - Matrix size is configured via constants (default 16x16).
  - Button pin and encoder pins are in `include/pins.h`.

- Libraries:
  - FastLED is used (added to `platformio.ini` as `lib_deps`).
  - Preferences (NVS) is used for persistence via `Preferences`.

- Persistence & runtime state:
  - Runtime settings (selected effect, brightness, speed) are encapsulated in `RuntimeState` (`include/runtime_state.h`, `src/runtime_state.cpp`).
  - `RuntimeState::load()` and `save()` handle NVS access using keys in `include/Config/runtime_config.h`.

- Coordinate mapping:
  - Logical coordinates use origin at bottom-left (0,0).
  - Mapping is implemented in `include/matrix_map.h` / `src/matrix_map.cpp`.
  - the mapping uses a fixed orientation.
  - Default wiring is a snake (zig-zag) pattern where every other row is reversed (matching your physical wiring).

- Effects:
  - Effects derive from `Effect` (`include/Effects/effect.h`) and implement `Update(float dt)` and `Render(CRGB* leds, const MatrixMap& map)`.
  - Two rainbow-style effects included:
    - `RainbowEffect` (uniform hue across the whole matrix, hue cycles over time)
    - `RainbowRainEffect` (per-row hue with vertical variation + phase animation; this was the original implementation)

- Controls:
  - Button: short press cycles effects (state persisted via `State`).
  - Rotary encoder: turn to adjust brightness; hold + turn to adjust effect speed.

- Build:
  - Board: `esp32-c3-devkitm-1` (see `platformio.ini`)
  - Build locally with: `pio run`

- Git & housekeeping:
  - A `.gitignore` is included to ignore build artifacts and editor state files.

Happy hacking! ✨

---

## License 📜

This project is released under the **MIT License** — see the `LICENSE` file for details.


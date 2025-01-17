# SSD1306 OLED LVGL & LovyanGFX example

Example sketch to use LVGL & LovaynGFX with an SSD1306 OLED display on ESP32 platforms (arduino framework). First flashes the display white/black for 1s, then initializes LVGL and displays a label with "Hello World".

## Usage

Define PINs for `OLED_SCL` and `OLED_SDA` (see `platformio.ini`). Some hardware (like heltec) also requires an `OLED_RST` pin definition. Same for `VEXT` (regulator for display on heltec boards).

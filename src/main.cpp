/*
 * example sketch for LVGL & LovyanGFX on an SSD1306 display
 * 
 * some pointers for LVGL styling on such small monochrome screens:
 *
 *   https://forum.lvgl.io/t/how-do-you-style-lvgl-for-tiny-screens/14941
 *   https://forum.lvgl.io/t/using-lv-use-theme-mono/2873/9
 *
 */

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <lvgl.h>

#include <vanilla_ssd1306.hpp>
static SSD1306 lcd;

 static auto tick_get_cb = []() -> uint32_t {
     return esp_timer_get_time() / 1000ULL;
 };

void my_flush_cb(lv_display_t * display, const lv_area_t * area, uint8_t * px_map)
{
    int w = (area->x2 - area->x1 + 1);
    int h = (area->y2 - area->y1 + 1);

    lcd.startWrite();
    lcd.pushImageDMA(area->x1, area->y1, w, h, (lgfx::grayscale_t*)px_map);
    lcd.endWrite();
    lv_display_flush_ready(display);
}

void setup(void)
{
	Serial.begin(115200);
  Serial.println("Lovyan SSD1306 startup...");

#ifdef VEXT
  // this is being used by Heltec boards to enable the display power supply
  Serial.println("Enable VEXT");
  pinMode(VEXT, OUTPUT);
  digitalWrite(VEXT, LOW);
#endif

#ifdef OLED_RST
  // some boards like heltec require a display reset for the SSD1306
  Serial.println("SSD1306 display reset");
  pinMode(OLED_RST, OUTPUT);  
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  delay(20);
#endif

	lcd.init();
	lcd.setColorDepth(lgfx::palette_1bit);

  // flash screen to confirm that LovyanGFX is working
	lcd.fillScreen(TFT_WHITE);
	delay(1000);
	lcd.fillScreen(TFT_BLACK);
	delay(1000);	
	lv_init();
	lv_tick_set_cb(tick_get_cb);

	static lv_display_t *display = lv_display_create(HOR_RES, VER_RES);
	lv_display_set_color_format(display, LV_COLOR_FORMAT_L8);
	//lv_display_set_color_format(display, LV_COLOR_FORMAT_I1);
	lv_theme_mono_init(display, true, &lv_font_unscii_8);

#define BYTES_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_L8))
// #define BYTES_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_I1))
	static uint8_t buf1[HOR_RES * VER_RES * BYTES_PER_PIXEL + 8]; // 8 byte extra for monochrome palette
	lv_display_set_buffers(display, buf1, NULL, sizeof(buf1), LV_DISPLAY_RENDER_MODE_DIRECT);
	lv_display_set_flush_cb(display, my_flush_cb);

	static lv_style_t style;
	lv_style_init(&style);
	lv_style_set_bg_color(&style, lv_color_hex(0x000000));
	lv_style_set_border_width(&style, 0);
	lv_style_set_border_color(&style, lv_color_hex(0xff0000));
	lv_style_set_pad_all(&style, 0);
	lv_style_set_text_font(&style, &lv_font_unscii_8);
	// lv_obj_remove_style_all(lv_scr_act());
	lv_obj_add_style(lv_scr_act(), &style, LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(lv_scr_act(),LV_OPA_100,0);
	lv_style_set_radius(&style, 0);

	lv_obj_t * label = lv_label_create(lv_scr_act());
	lv_label_set_text(label, "Hello World");
	lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

/*



*/

}

void loop(void)
{
	lv_timer_handler();
	//Serial.println(".");
}

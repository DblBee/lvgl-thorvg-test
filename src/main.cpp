#include <lvgl.h>
#include <TFT_eSPI.h>
#include "dice.h"

#define TFT_HOR_RES 240
#define TFT_VER_RES 240
#define TFT_ROTATION LV_DISPLAY_ROTATION_90

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 5 * (LV_COLOR_DEPTH / 8))
void *draw_buf;
unsigned long last_tick = 0;

void loadLottie()
{
  extern const uint8_t diceAnimation[];
  extern const size_t diceAnimation_size;

  lv_obj_t *lottie = lv_lottie_create(lv_screen_active());
  lv_lottie_set_src_data(lottie, diceAnimation, diceAnimation_size);

#if LV_DRAW_BUF_ALIGN == 4 && LV_DRAW_BUF_STRIDE_ALIGN == 1
  lv_lottie_set_buffer(lottie, 240, 240, &draw_buf);
#else
  LV_DRAW_BUF_DEFINE(draw_buf, 240, 240, LV_COLOR_FORMAT_ARGB8888);
  lv_lottie_set_draw_buf(lottie, &draw_buf);
#endif

  lv_obj_center(lottie);
}

void setup()
{
  lv_init();

  draw_buf = heap_caps_malloc(DRAW_BUF_SIZE, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);

  lv_display_t *disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, DRAW_BUF_SIZE);
  lv_display_set_rotation(disp, TFT_ROTATION);

  loadLottie();
  
  Serial.println("Setup done");
}

void loop()
{
  unsigned long tickPeriod = millis() - last_tick;
  lv_tick_inc(tickPeriod);
  last_tick = millis();
  lv_timer_handler();
}

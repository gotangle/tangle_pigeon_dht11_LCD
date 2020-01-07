#pragma once

#include "common/defs.h"
#include "ST7735.h"

int send_message(esp_sleep_source_t trigger);

void lcd_init();
void lcd_fill_screen(int16_t color);
void lcd_print(int16_t x, int16_t y, int16_t color, const char *fmt, ...);


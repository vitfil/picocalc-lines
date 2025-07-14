#ifndef GFX_H
#define GFX_H

#include "lcdspi.h"

void draw_rect(int x1, int y1, int x2, int y2, unsigned int color);
void draw_rect_fill(int x1, int y1, int x2, int y2, unsigned int color);
void draw_char(int x, int y, char c, unsigned int foreground_color, unsigned int background_color);
void draw_text(int x, int y, const char *s, unsigned int foreground_color, unsigned int background_color);
void draw_circle_fill(int x, int y, int radius, unsigned int color);

#endif
#include "gfx.h"

void draw_bitmap_spi(int x1, int y1, int width, int height, int scale, int fc, int bc, unsigned char *bitmap);
void define_region_spi(int xstart, int ystart, int xend, int yend, int rw);
void draw_rect_spi(int x1, int y1, int x2, int y2, int c);
extern unsigned char font1[];

void draw_rect(int x1, int y1, int x2, int y2, unsigned int color) {
    draw_rect_fill(x1, y1, x2, y2, color);
    draw_rect_fill(x1+1, y1+1, x2-1, y2-1, BLACK);
}

void draw_rect_fill(int x1, int y1, int x2, int y2, unsigned int color) {
    draw_rect_spi(x1, y1, x2, y2, color);
}

void draw_char(int x, int y, char c, unsigned int foreground_color, unsigned int background_color) {
    unsigned char *p, *fp, *np = NULL;
    int height, width;

    fp = (unsigned char *)font1;

    height = fp[1];
    width = fp[0];

    if (c >= fp[2] && c < fp[2] + fp[3]) {
        p = fp + 4 + (int)(((c - fp[2]) * height * width) / 8);
        np = p;

        draw_bitmap_spi(x, y, width, height, 1, foreground_color, background_color, np);
    } else {
        draw_rect_spi(x, y, x + width, y + height, background_color);
    }
}

void draw_text(int x, int y, const char *s, unsigned int foreground_color, unsigned int background_color) {
    while (*s) {
        draw_char(x, y, *s, foreground_color, background_color);
        x += 8;
        s++;
    }
}

void draw_circle_fill(int x, int y, int radius, unsigned int color) {
    int dx, dy;
    int radius_squared = radius * radius;
    
    // Проходим по всем пикселям в квадрате вокруг центра
    for (dx = -radius; dx <= radius; dx++) {
        for (dy = -radius; dy <= radius; dy++) {
            // Проверяем, находится ли пиксель внутри круга
            if (dx * dx + dy * dy <= radius_squared) {
                // Рисуем пиксель (используем draw_rect_spi для рисования одного пикселя)
                draw_rect_spi(x + dx, y + dy, x + dx, y + dy, color);
            }
        }
    }
}
#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

#include "lcdspi/display.h"

#define MAP_ROW_COUNT 8
#define MAP_COL_COUNT 8
#define MAP_CELL_SIZE 40

typedef struct {
    unsigned int cells[MAP_ROW_COUNT][MAP_COL_COUNT];  // Ball color or 0x0000
    int selectedRow;
    int selectedCol;
    int posRow;
    int posCol;
} Map;

enum Animation {
    NONE,
    RAISE,
    FADE
};

void draw_ball(int row, int col, Map *map, enum Animation animation);
void draw_cell(int row, int col, Map *map);
void draw_map(Map *map);

#endif
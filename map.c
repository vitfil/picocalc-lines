#include "map.h"

void draw_cell(int row, int col, Map *map) {
    if (row < 0 || row >= MAP_ROW_COUNT || col < 0 || col >= MAP_COL_COUNT) {
        return;
    }

    int x1 = col * MAP_CELL_SIZE;
    int y1 = row * MAP_CELL_SIZE;
    int x2 = x1 + MAP_CELL_SIZE - 1;
    int y2 = y1 + MAP_CELL_SIZE - 1;

    if (row == map->posRow && col == map->posCol) {
        draw_rect_fill(x1, y1, x2, y2, GREEN);
        draw_rect_fill(x1 + 2, y1 + 2, x2 - 2, y2 - 2, BLACK);
    } else {
        draw_rect(x1, y1, x2, y2, LITEGRAY);
    }

    if (map->cells[row][col] != BLACK) {
        int radius = MAP_CELL_SIZE / 4;
        if (row == map->selectedRow && col == map->selectedCol) {
            radius = MAP_CELL_SIZE / 3;  // Increase radius for selected cell
        }
        draw_circle_fill(x1 + MAP_CELL_SIZE / 2, y1 + MAP_CELL_SIZE / 2, radius, map->cells[row][col]);
    }
}

void draw_map(Map *map) {
    for (int row = 0; row < MAP_ROW_COUNT; row++) {
        for (int col = 0; col < MAP_COL_COUNT; col++) {
            draw_cell(row, col, map);
        }
    }
}
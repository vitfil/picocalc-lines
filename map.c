#include "map.h"

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
} Cell;

Cell to_cell(int row, int col) {
    Cell cell = {
        .x1 = col * MAP_CELL_SIZE,
        .y1 = row * MAP_CELL_SIZE,
    };
    cell.x2 = cell.x1 + MAP_CELL_SIZE - 1;
    cell.y2 = cell.y1 + MAP_CELL_SIZE - 1;
    return cell;
}

void draw_ball(int row, int col, Map *map, enum Animation animation) {
    if (row < 0 || row >= MAP_ROW_COUNT || col < 0 || col >= MAP_COL_COUNT) {
        return;
    }

    Cell cell = to_cell(row, col);

    int radius = MAP_CELL_SIZE / 3;

    draw_circle_fill(cell.x1 + MAP_CELL_SIZE / 2, cell.y1 + MAP_CELL_SIZE / 2, radius, BLACK);

    if (!(map->cells[row][col] == BLACK || row == map->selectedRow && col == map->selectedCol)) {
        radius = MAP_CELL_SIZE / 4;
    }

    switch (animation) {
        case RAISE:
            for (int i = 1; i <= 10; i++) {
                draw_circle_fill(cell.x1 + MAP_CELL_SIZE / 2, cell.y1 + MAP_CELL_SIZE / 2, radius * i / 10, map->cells[row][col]);
                sleep_ms(20);
            }
            break;

        case FADE:
            for (int i = 10; i >= 1; i--) {
                draw_circle_fill(cell.x1 + MAP_CELL_SIZE / 2, cell.y1 + MAP_CELL_SIZE / 2, radius * i / 10, map->cells[row][col]);
                sleep_ms(20);
                draw_circle_fill(cell.x1 + MAP_CELL_SIZE / 2, cell.y1 + MAP_CELL_SIZE / 2, radius * i / 10, BLACK);
            }
            break;

        default:
            draw_circle_fill(cell.x1 + MAP_CELL_SIZE / 2, cell.y1 + MAP_CELL_SIZE / 2, radius, map->cells[row][col]);
            break;
    }
}

void draw_cell(int row, int col, Map *map) {
    if (row < 0 || row >= MAP_ROW_COUNT || col < 0 || col >= MAP_COL_COUNT) {
        return;
    }

    Cell cell = to_cell(row, col);

    if (row == map->posRow && col == map->posCol) {
        draw_rect_fill(cell.x1, cell.y1, cell.x2, cell.y2, GREEN);
        draw_rect_fill(cell.x1 + 2, cell.y1 + 2, cell.x2 - 2, cell.y2 - 2, BLACK);
    } else {
        draw_rect(cell.x1, cell.y1, cell.x2, cell.y2, LITEGRAY);
    }
}

void draw_map(Map *map) {
    for (int row = 0; row < MAP_ROW_COUNT; row++) {
        for (int col = 0; col < MAP_COL_COUNT; col++) {
            draw_cell(row, col, map);
        }
    }
}
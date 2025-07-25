#include <stdio.h>
#include <stdlib.h>

#include "hardware/gpio.h"
#include "i2ckbd/i2ckbd.h"
#include "lcdspi/display.h"
#include "map.h"
#include "pico/rand.h"

bool add_ball();

static Map game_map = {
    .cells = {0},
    .selectedRow = -1,
    .selectedCol = -1,
    .posRow = 0,
    .posCol = 0,
};

static int ballsCount = 0;

#define BALLS_IN_LINE 5
#define BALLS_TYPE_COUNT 3
const unsigned int colors[BALLS_TYPE_COUNT] = {GREEN, RED, BLUE};
// const unsigned int colors[BALLS_TYPE_COUNT] = {GREEN, RED, BLUE, YELLOW, PINK, ORANGE};

void new_game() {
    for (int row = 0; row < MAP_ROW_COUNT; row++) {
        for (int col = 0; col < MAP_COL_COUNT; col++) {
            game_map.cells[row][col] = 0;
        }
    }
    game_map.posRow = 0;
    game_map.posCol = 0;
    game_map.selectedRow = -1;
    game_map.selectedCol = -1;

    ballsCount = 0;
}

bool add_ball() {
    // Check if the game map is full
    bool full = true;
    for (int row = 0; row < MAP_ROW_COUNT; row++) {
        for (int col = 0; col < MAP_COL_COUNT; col++) {
            if (game_map.cells[row][col] == 0) {
                full = false;  // Found an empty cell
                break;
            }
        }
        if (!full) break;
    }
    if (full) {
        // Game map is full, cannot add more balls
        return false;
    }
    int row = get_rand_32() % MAP_ROW_COUNT;
    int col = get_rand_32() % MAP_COL_COUNT;
    if (game_map.cells[row][col] != 0) {
        // Cell is not empty, try again

        return add_ball();
    }

    game_map.cells[row][col] = colors[get_rand_32() % BALLS_TYPE_COUNT];
    draw_ball(row, col, &game_map, RAISE);
    return true;
}

int level() {
    return (ballsCount / 100) + 1;
}

void deselect_ball() {
    if (game_map.selectedRow != -1 && game_map.selectedCol != -1) {
        int posRow = game_map.selectedRow;
        int posCol = game_map.selectedCol;
        game_map.selectedRow = -1;
        game_map.selectedCol = -1;
        draw_ball(posRow, posCol, &game_map, NONE);
    }
}

void remove_lines() {
    unsigned int cells[MAP_ROW_COUNT][MAP_COL_COUNT] = {0};

    // Search >=5 the same color balls in a row
    for (int row = 0; row < MAP_ROW_COUNT; row++) {
        for (int col = 0; col <= MAP_COL_COUNT - BALLS_IN_LINE; col++) {
            if (game_map.cells[row][col] != 0 &&
                game_map.cells[row][col] == game_map.cells[row][col + 1] &&
                game_map.cells[row][col] == game_map.cells[row][col + 2] &&
                game_map.cells[row][col] == game_map.cells[row][col + 3] &&
                game_map.cells[row][col] == game_map.cells[row][col + 4]) {
                // Found 5 or more in a row
                cells[row][col] = 1;
                cells[row][col + 1] = 1;
                cells[row][col + 2] = 1;
                cells[row][col + 3] = 1;
                cells[row][col + 4] = 1;
            }
        }
    }

    // Search >=5 the same color balls in a col
    for (int col = 0; col < MAP_COL_COUNT; col++) {
        for (int row = 0; row <= MAP_ROW_COUNT - BALLS_IN_LINE; row++) {
            if (game_map.cells[row][col] != 0 &&
                game_map.cells[row][col] == game_map.cells[row + 1][col] &&
                game_map.cells[row][col] == game_map.cells[row + 2][col] &&
                game_map.cells[row][col] == game_map.cells[row + 3][col] &&
                game_map.cells[row][col] == game_map.cells[row + 4][col]) {
                // Found 5 or more in a row
                cells[row][col] = 1;
                cells[row + 1][col] = 1;
                cells[row + 2][col] = 1;
                cells[row + 3][col] = 1;
                cells[row + 4][col] = 1;
            }
        }
    }

    // Search >=5 the same color balls in a diagonal (top-left to bottom-right)
    for (int row = 0; row <= MAP_ROW_COUNT - BALLS_IN_LINE; row++) {
        for (int col = 0; col <= MAP_COL_COUNT - BALLS_IN_LINE; col++) {
            if (game_map.cells[row][col] != 0 &&
                game_map.cells[row][col] == game_map.cells[row + 1][col + 1] &&
                game_map.cells[row][col] == game_map.cells[row + 2][col + 2] &&
                game_map.cells[row][col] == game_map.cells[row + 3][col + 3] &&
                game_map.cells[row][col] == game_map.cells[row + 4][col + 4]) {
                // Found 5 or more in a row
                cells[row][col] = 1;
                cells[row + 1][col + 1] = 1;
                cells[row + 2][col + 2] = 1;
                cells[row + 3][col + 3] = 1;
                cells[row + 4][col + 4] = 1;
            }
        }
    }

    // Search >=5 the same color balls in a diagonal (top-right to bottom-left)
    for (int row = 0; row <= MAP_ROW_COUNT - BALLS_IN_LINE; row++) {
        for (int col = BALLS_IN_LINE - 1; col < MAP_COL_COUNT; col++) {
            if (game_map.cells[row][col] != 0 &&
                game_map.cells[row][col] == game_map.cells[row + 1][col - 1] &&
                game_map.cells[row][col] == game_map.cells[row + 2][col - 2] &&
                game_map.cells[row][col] == game_map.cells[row + 3][col - 3] &&
                game_map.cells[row][col] == game_map.cells[row + 4][col - 4]) {
                // Found 5 or more in a row
                cells[row][col] = 1;
                cells[row + 1][col - 1] = 1;
                cells[row + 2][col - 2] = 1;
                cells[row + 3][col - 3] = 1;
                cells[row + 4][col - 4] = 1;
            }
        }
    }

    // Remove the balls
    for (int row = 0; row < MAP_ROW_COUNT; row++) {
        for (int col = 0; col < MAP_COL_COUNT; col++) {
            if (cells[row][col] == 1) {
                draw_ball(row, col, &game_map, FADE);  // Clear the cell on the display
                game_map.cells[row][col] = 0;
                ballsCount++;
            }
        }
    }
}

void add_balls() {
    for (int i = 0; i < 3; i++) {
        if (!add_ball()) {
            // If we cannot add more balls, break the loop
            break;
        }
    }
}

void start_game() {
    new_game();
    draw_map(&game_map);
    draw_rect_fill(MAP_CELL_SIZE, MAP_CELL_SIZE * 3, MAP_CELL_SIZE * 7 - 1, MAP_CELL_SIZE * 5 - 1, BLACK);
    draw_text(64, 154, "Press Space for new game", WHITE, BLACK);
    while (true) {
        if (i2c_kbd_read() == 32) break;
        sleep_ms(20);
    }
    for (int row = 3; row <= 4; row++) {
        for (int col = 1; col <= 6; col++) {
            draw_cell(row, col, &game_map);
        }
    }
    add_balls();
}

int main() {
    stdio_init_all();

    i2c_kbd_init();
    lcd_init();

    lcd_clear();

    start_game();

    while (true) {
        int c = i2c_kbd_read();
        // draw c as rext
        if (c == 32) {  // Space key
            new_game();
            draw_map(&game_map);
            add_balls();
        } else if (c >= 180 && c <= 183) {  // Arrow keys
            int posRow = game_map.posRow;
            int posCol = game_map.posCol;
            switch (c) {
                case 180:  // Left
                    if (game_map.posCol > 0) game_map.posCol--;
                    break;
                case 181:  // Up
                    if (game_map.posRow > 0) game_map.posRow--;
                    break;
                case 183:  // Right
                    if (game_map.posCol < MAP_COL_COUNT - 1) game_map.posCol++;
                    break;
                case 182:  // Down
                    if (game_map.posRow < MAP_ROW_COUNT - 1) game_map.posRow++;
                    break;
            }
            if (posCol != game_map.posCol || posRow != game_map.posRow) {
                draw_cell(posRow, posCol, &game_map);                    // Clear previous position
                draw_ball(posRow, posCol, &game_map, NONE);
                
                draw_cell(game_map.posRow, game_map.posCol, &game_map);  // Draw new position
                draw_ball(game_map.posRow, game_map.posCol, &game_map, NONE);
            }
        } else if (c == 10) {  // Enter key
            if (game_map.cells[game_map.posRow][game_map.posCol] == 0) {
                // If the cell is empty
                if (game_map.selectedRow != -1 && game_map.selectedCol != -1) {
                    game_map.cells[game_map.posRow][game_map.posCol] = game_map.cells[game_map.selectedRow][game_map.selectedCol];
                    game_map.cells[game_map.selectedRow][game_map.selectedCol] = 0;
                    // Deselect the ball
                    deselect_ball();
                    draw_ball(game_map.posRow, game_map.posCol, &game_map, NONE);
                    unsigned int cells[MAP_ROW_COUNT][MAP_COL_COUNT];
                    // Copy the current game map cells to a temporary array
                    for (int row = 0; row < MAP_ROW_COUNT; row++) {
                        for (int col = 0; col < MAP_COL_COUNT; col++) {
                            cells[row][col] = game_map.cells[row][col];
                        }
                    }
                    remove_lines();  // Remove lines after placing a ball
                    add_balls();     // Add new balls after placing one
                    // Draw changed cells
                    // for (int row = 0; row < MAP_ROW_COUNT; row++) {
                    //     for (int col = 0; col < MAP_COL_COUNT; col++) {
                    //         if (cells[row][col] != game_map.cells[row][col]) {
                    //             draw_cell(row, col, &game_map);
                    //         }
                    //     }
                    // }
                    remove_lines();  // Remove lines after adding balls
                }
            } else {
                // If the cell is not empty, select the ball
                if (game_map.selectedRow == game_map.posRow && game_map.selectedCol == game_map.posCol) {
                    // Deselect the ball
                    game_map.selectedRow = -1;
                    game_map.selectedCol = -1;
                    draw_ball(game_map.posRow, game_map.posCol, &game_map, NONE);
                } else {
                    // Deselect the previous ball if any
                    deselect_ball();
                    // Select the new ball
                    game_map.selectedRow = game_map.posRow;
                    game_map.selectedCol = game_map.posCol;
                    draw_ball(game_map.posRow, game_map.posCol, &game_map, NONE);
                }
            }
        }
        // for debugging draw the pressed key
        // if (c != -1) {
        //     // Clear the previous text
        //     draw_text(0, 0, "          ", BLACK, BLACK);
        //     char buf[12];
        //     sprintf(buf, "%d", c);
        //     draw_text(0, 0, buf, WHITE, BLACK);
        // }
        sleep_ms(20);
    }
}

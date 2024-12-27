
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_STEPS 500
#define ROWS 20
#define COLUMNS 40
#define SPAWN_RATE 0.25

int print_Grid(uint8_t **grid) {
  printf(
      "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
  printf(
      "|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|\n");
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      printf(grid[i][j] == 1 ? "0" : "_");
    }
    printf("\n");
  }
  return 0;
}

int count_Neighbors(uint8_t **grid, int x, int y, int offset_index, int count) {
  int offsets[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                       {0, 1},   {1, -1}, {1, 0},  {1, 1}};

  if (offset_index >= 8) {
    return count;
  }

  int neighbor_x = x + offsets[offset_index][0];
  int neighbor_y = y + offsets[offset_index][1];

  if (neighbor_x >= 0 && neighbor_x < ROWS && neighbor_y >= 0 &&
      neighbor_y < COLUMNS) {
    if (grid[neighbor_x][neighbor_y] == 1) {
      count++;
    }
  }

  return count_Neighbors(grid, x, y, offset_index + 1, count);
}

void update_Grid(uint8_t **grid) {

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      int neighbors_Count = count_Neighbors(grid, i, j, 0, 0);
      if (grid[i][j] == 1) {
        if (neighbors_Count < 2) {
          grid[i][j] = 0;
        } else if (neighbors_Count > 3) {
          grid[i][j] = 0;
        }
      } else {
        if (neighbors_Count == 3) {
          grid[i][j] = 1;
        }
      }
    }
  }
}

int generate_Random_Bit(float probability) {
  float random_value = (float)rand() / RAND_MAX;
  return (random_value < probability) ? 1 : 0;
}

void random_Grid(uint8_t **grid) {

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      grid[i][j] = generate_Random_Bit(SPAWN_RATE);
    }
  }
}

uint8_t **malloc_Grid(int rows, int columns) {
  uint8_t **grid = malloc(rows * sizeof(uint8_t *));
  if (!grid) {
    return NULL;
  }

  for (int i = 0; i < rows; i++) {
    grid[i] = malloc(columns * sizeof(uint8_t));
    if (!grid[i]) {
      for (int j = 0; j < i; j++) {
        free(grid[j]);
      }
      free(grid);
      return NULL;
    }
  }
  return grid;
}

int run_GameState(uint8_t **grid, int max_steps) {
  int steps = 0;
  while (steps < max_steps) {
    update_Grid(grid);
    print_Grid(grid);
    usleep(200 * 1000); // (ms * 1000) = microsecs
    steps++;
  }
  return 1;
}

int main() {

  srand(time(0));

  uint8_t **grid = malloc_Grid(ROWS, COLUMNS);

  random_Grid(grid);

  run_GameState(grid, MAX_STEPS);
}

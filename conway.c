
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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

int count_Neighbors(uint8_t **grid, int x, int y) {
  int count = 0;

  int offsets[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                       {0, 1},   {1, -1}, {1, 0},  {1, 1}};

  for (int i = 0; i < 8; i++) {
    int neighbor_x = x + offsets[i][0];
    int neighbor_y = y + offsets[i][1];

    if (neighbor_x < 0 || neighbor_x > ROWS - 1 || neighbor_y < 0 ||
        neighbor_y > COLUMNS - 1) {
      continue;
    }

    if (grid[neighbor_x][neighbor_y] == 1) {
      count++;
    }
  }
  return count;
}

void update_Grid(uint8_t **grid) {

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLUMNS; j++) {
      int neighbors_Count = count_Neighbors(grid, i, j);
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

int run_GameState(uint8_t **grid) {
  while (1) {
    update_Grid(grid);
    print_Grid(grid);
    usleep(200 * 1000); // (ms * 1000) = microsends
  }
  return 1;
}

int main() {

  srand(time(0));

  uint8_t **grid = malloc_Grid(ROWS, COLUMNS);

  random_Grid(grid);

  run_GameState(grid);
}

#include "genCave.h"

void genCave(int rows, int cols, int init_chance, Input *model) {
  model->clear();
  model->row = rows;
  model->col = cols;
  model->tmp = new int *[rows];
  for (int i = 0; i < rows; i++) model->tmp[i] = new int[cols];
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      model->tmp[i][j] = 0;
      int chance = rand() % 10;
      if (chance < init_chance / 10) model->tmp[i][j] = 1;
    }
  }
}

void genNextStep(int birth_limit, int death_limit, Input *model) {
  int next[model->row][model->col];
  for (int i = 0; i < model->row; i++)
    for (int j = 0; j < model->col; j++) next[i][j] = model->tmp[i][j];

  for (int i = 0; i < model->row; i++) {
    for (int j = 0; j < model->col; j++) {
      int neighbours = calcNeighbours(i, j, model);
      if (model->tmp[i][j] && neighbours < death_limit)
        next[i][j] = 0;
      else if (!model->tmp[i][j] && neighbours >= birth_limit)
        next[i][j] = 1;
    }
  }
  for (int i = 0; i < model->row; i++)
    for (int j = 0; j < model->col; j++) model->tmp[i][j] = next[i][j];
}

int calcNeighbours(int r, int c, Input *model) {
  int res = 0;
  if (r == 0 || model->tmp[r - 1][c]) res++;
  if (r == 0 || c == 0 || model->tmp[r - 1][c - 1]) res++;
  if (r == 0 || c == model->col - 1 || model->tmp[r - 1][c + 1]) res++;
  if (c == 0 || model->tmp[r][c - 1]) res++;
  if (c == model->col - 1 || model->tmp[r][c + 1]) res++;
  if (r == model->row - 1 || model->tmp[r + 1][c]) res++;
  if (r == model->row - 1 || c == 0 || model->tmp[r + 1][c - 1]) res++;
  if (r == model->row - 1 || c == model->col - 1 || model->tmp[r + 1][c + 1])
    res++;
  return res;
}
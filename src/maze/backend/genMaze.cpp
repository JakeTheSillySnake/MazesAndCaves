#include "genMaze.h"

void genMaze(int rows, int cols, Input *model) {
  model->clear();
  model->row = rows;
  model->col = cols;
  model->borderX = new int *[rows];
  model->borderY = new int *[rows];
  model->tmp = new int *[rows];
  for (int i = 0; i < rows; i++) {
    model->borderY[i] = new int[cols];
    model->borderX[i] = new int[cols];
    model->tmp[i] = new int[cols];
  }
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      model->borderX[i][j] = 0;
      model->borderY[i][j] = 0;
      model->tmp[i][j] = 0;
    }
  }
  if (rows < 2 || cols < 2) return;
  int count = 1;
  genFirstRow(&count, model);
  for (int i = 0; i < rows; i++) {
    genXBorder(i, model);
    mergeSets(i, model);
    genYBorder(i, model);
    if (i != rows - 1) genNextRow(i, &count, model);
  }
  mergeLastRow(rows - 1, model);
}

void genFirstRow(int *count, Input *model) {
  for (int i = 0; i < model->col; i++) model->tmp[0][i] = (*count)++;
}

void genXBorder(int r, Input *model) {
  for (int i = 0; i < model->col; i++)
    if (rand() % 2 || i == model->col - 1 ||
        (model->tmp[r][i] == model->tmp[r][i + 1] && i != model->col - 1))
      model->borderX[r][i] = 1;
}

void genYBorder(int r, Input *model) {
  for (int i = 0; i < model->col; i++) {
    int free = 0, find = model->tmp[r][i];
    for (int j = 0; j < model->col; j++)
      if (model->tmp[r][j] == find && !model->borderY[r][j]) free++;
    if (free > 1 && rand() % 2) model->borderY[r][i] = 1;
  }
}

void mergeSets(int r, Input *model) {
  for (int i = 0; i < model->col; i++) {
    if (!model->borderX[r][i]) {
      findReplace(r, model->tmp[r][i + 1], model->tmp[r][i], model);
    }
  }
}

void genNextRow(int r, int *count, Input *model) {
  for (int i = 0; i < model->col; i++) {
    if (!model->borderY[r][i])
      model->tmp[r + 1][i] = model->tmp[r][i];
    else
      model->tmp[r + 1][i] = (*count)++;
  }
}

void mergeLastRow(int r, Input *model) {
  for (int i = 0; i < model->col; i++) {
    model->borderY[r][i] = 1;
    if (i == model->col - 1) break;
    if (model->tmp[r][i] != model->tmp[r][i + 1]) {
      model->borderX[r][i] = 0;
      findReplace(r, model->tmp[r][i + 1], model->tmp[r][i], model);
    }
  }
}

void findReplace(int r, int find, int replace, Input *model) {
  for (int j = 0; j < model->col; j++) {
    if (model->tmp[r][j] == find) {
      model->tmp[r][j] = replace;
      if (j != model->col - 1 && model->tmp[r][j] == model->tmp[r][j + 1])
        model->borderX[r][j] = 1;
    }
  }
}
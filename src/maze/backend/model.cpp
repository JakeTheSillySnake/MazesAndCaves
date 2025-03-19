#include "model.h"

using namespace std;

int Input::uploadFile(string file, int mode) {
  clear();
  int error = 0;
  filename = file;
  const char *cfile = file.c_str();

  FILE *fptr = fopen(cfile, "r");
  int res = fscanf(fptr, "%d %d", &row, &col);
  if (res != 2)
    error = EMPTY;
  else if (row < 1 || col < 1 || row > 50 || col > 50)
    error = BAD_FORMAT;
  if (error) {
    fclose(fptr);
    return error;
  }
  if (mode == MAZE_MODE)
    error = uploadMaze(fptr);
  else
    error = uploadCave(fptr);

  if (!error || error == BAD_DATA) fclose(fptr);
  if (error == BAD_DATA) clear();
  return error;
}

int Input::uploadMaze(FILE *fptr) {
  borderX = new int *[row];
  borderY = new int *[row];
  for (int i = 0; i < row; i++) {
    borderY[i] = new int[col];
    borderX[i] = new int[col];
  }
  int n, res, error = 0;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      res = fscanf(fptr, "%d", &n);
      if ((n != 1 && n != 0) || res != 1) {
        error = BAD_DATA;
        break;
      }
      borderX[i][j] = n;
    }
  }
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      res = fscanf(fptr, "%d", &n);
      if ((n != 1 && n != 0) || res != 1) {
        error = BAD_DATA;
        break;
      }
      borderY[i][j] = n;
    }
  }
  return error;
}

int Input::uploadCave(FILE *fptr) {
  int n, res, error = 0;
  tmp = new int *[row];
  for (int i = 0; i < row; i++) tmp[i] = new int[col];
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      tmp[i][j] = 0;
    }
  }
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      res = fscanf(fptr, "%d", &n);
      if ((n != 1 && n != 0) || res != 1) {
        error = BAD_DATA;
        break;
      }
      tmp[i][j] = n;
    }
  }
  return error;
}

void Input::clear() {
  if (borderX) {
    for (int i = 0; i < row; i++) {
      delete[] borderY[i];
      delete[] borderX[i];
    }
    delete[] borderX;
    delete[] borderY;
    borderY = NULL;
    borderX = NULL;
  }
  if (tmp) {
    for (int i = 0; i < row; i++) delete[] tmp[i];
    delete[] tmp;
    tmp = NULL;
  }
  row = 0;
  col = 0;
  filename = "";
}

void Input::saveFile(string file) {
  filename = file;
  const char *cfile = file.c_str();
  FILE *fptr = fopen(cfile, "w");
  fprintf(fptr, "%d %d\n", row, col);
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      fprintf(fptr, "%d ", borderX[i][j]);
    }
    fprintf(fptr, "\n");
  }
  fprintf(fptr, "\n");
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      fprintf(fptr, "%d ", borderY[i][j]);
    }
    fprintf(fptr, "\n");
  }
  fclose(fptr);
}

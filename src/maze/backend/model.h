#ifndef MODEL_H
#define MODEL_H

#include <time.h>

#include <string>

#define MAZE_MODE 1
#define CAVE_MODE 2
#define EMPTY_MODE 3

using namespace std;

enum errors { EMPTY = 1, BAD_DATA, BAD_FORMAT, NO_SOLUTION };

class Input {
 public:
  Input(){};
  ~Input() { clear(); };
  Input(const Input &in) : col(in.col), row(in.row), filename(in.filename) {
    if (in.tmp) {
      tmp = new int *[row];
      for (int i = 0; i < row; i++) tmp[i] = new int[col];
      for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
          tmp[i][j] = in.tmp[i][j];
        }
      }
    }
    if (in.borderX) {
      borderX = new int *[row];
      borderY = new int *[row];
      for (int i = 0; i < row; i++) {
        borderY[i] = new int[col];
        borderX[i] = new int[col];
      }
      for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
          borderX[i][j] = in.borderX[i][j];
          borderY[i][j] = in.borderY[i][j];
        }
      }
    }
  }
  Input &operator=(const Input &other) {
    Input tmp(other);
    swap(*this, tmp);
    return *this;
  }
  void swap(Input &first, Input &second) {
    std::swap(first.col, second.col);
    std::swap(first.row, second.row);
    std::swap(first.filename, second.filename);
    std::swap(first.tmp, second.tmp);
    std::swap(first.borderX, second.borderX);
    std::swap(first.borderY, second.borderY);
  }

  int uploadFile(string file, int mode);
  int uploadMaze(FILE *fptr);
  int uploadCave(FILE *fptr);
  void saveFile(string file);
  void clear();

  int col = 0, row = 0;
  int **borderX = nullptr, **borderY = nullptr;
  int **tmp = nullptr;
  string filename = "";
};

#endif

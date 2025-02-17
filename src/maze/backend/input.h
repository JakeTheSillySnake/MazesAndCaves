#ifndef INPUT_H
#define INPUT_H

#include <iostream>

enum errors {
  EMPTY = 1,
  NOT_FOUND,
  BAD_DATA,
  BAD_FORMAT,
};

class Input {
 public:
  Input();
  ~Input();

  int uploadFile(char file[]);
  void printData();

  int col = 0, row = 0;
  int **borderX, **borderY;
};

#endif

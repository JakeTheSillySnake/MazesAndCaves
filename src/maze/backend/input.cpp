#include "input.h"

using namespace std;

Input::Input() {}

Input::~Input() {}

int Input::uploadFile(char file[]) {
  int error = 0;
  FILE *fptr = fopen(file, "r");
  if (fptr == NULL)
    error = NOT_FOUND;
  else {
    int res = fscanf(fptr, "%d %d", &row, &col);
    if (res != 2)
      error = EMPTY;
    else if (row <= 0 || col <= 0 || row > 50 || col > 50)
      error = BAD_FORMAT;
    if (error) {
      fclose(fptr);
      return error;
    }

    borderX = new int *[row];
    borderY = new int *[row];
    for (int i = 0; i < row; i++) {
      borderY[i] = new int[col];
      borderX[i] = new int[col];
    }
    int n;
    for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++) {
        fscanf(fptr, "%d", &n);
        if (n != 1 && n != 0) {
          error = BAD_DATA;
          break;
        }
        borderX[i][j] = n;
      }
    }
    for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++) {
        fscanf(fptr, "%d", &n);
        if (n != 1 && n != 0) {
          error = BAD_DATA;
          break;
        }
        borderY[i][j] = n;
      }
    }
  }
  if (!error || error == BAD_DATA) fclose(fptr);
  if (error == BAD_DATA) {
    for (int i = 0; i < row; i++) {
      delete[] borderX[i];
      delete[] borderY[i];
    }
    delete[] borderX;
    delete[] borderY;
  }
  return error;
}

void Input::printData() {
  cout << "\nSize: " << row << " * " << col << endl << " ";
  for (int i = 0; i < col; i++) cout << "_ ";
  cout << endl;
  for (int i = 0; i < row; i++) {
    cout << "|";
    for (int j = 0; j < col; j++) {
      if (borderY[i][j])
        cout << "_";
      else
        cout << " ";
      if (borderX[i][j])
        cout << "|";
      else
        cout << " ";
    }
    cout << "\n";
  }
  cout << endl;
}

int main() {
  cout << "Input file: ";
  char file[40];
  scanf("%s", file);
  Input in;
  int error = in.uploadFile(file);
  if (!error)
    in.printData();
  else
    cout << "Error code: " << error << endl;
  return 0;
}

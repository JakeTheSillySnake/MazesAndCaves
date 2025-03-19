#include <gtest/gtest.h>

#include "../backend/genMaze.h"

int check_no_access(Input *in) {
  int no_access = 0;
  for (int i = 0; i < in->row; i++) {
    for (int j = 0; j < in->col; j++) {
      int walls = 0;
      if (in->borderX[i][j]) walls++;
      if (in->borderY[i][j]) walls++;
      if (j == 0)
        walls++;
      else if (in->borderX[i][j - 1])
        walls++;
      if (i == 0)
        walls++;
      else if (in->borderY[i - 1][j])
        walls++;

      if (walls > 3) no_access++;
    }
  }
  return no_access;
}

int check_loops(Input *in) {
  int loops = 0;
  for (int i = 0; i < in->row; i++) {
    for (int j = 0; j < in->col; j++) {
      if (i && i != in->row - 1 && j && j != in->col - 1 && in->borderX[i][j] &&
          !in->borderX[i - 1][j] && !in->borderX[i + 1][j] &&
          !in->borderY[i][j] && !in->borderY[i - 1][j] &&
          !in->borderY[i][j + 1] && !in->borderY[i - 1][j + 1])
        loops++;

      if (i && i != in->row - 1 && j && j != in->col - 1 && in->borderY[i][j] &&
          !in->borderX[i][j] && !in->borderX[i][j - 1] &&
          !in->borderX[i + 1][j] && !in->borderX[i + 1][j - 1] &&
          !in->borderY[i][j + 1] && !in->borderY[i][j - 1])
        loops++;
    }
  }
  return loops;
}

TEST(test_genMaze, maze_1x1) {
  Input in;
  genMaze(1, 1, &in);
  ASSERT_EQ(in.row, 1);
  ASSERT_EQ(in.col, 1);
  ASSERT_EQ(in.borderX[0][0], 0);
  ASSERT_EQ(in.borderY[0][0], 0);
}

TEST(test_genMaze, maze_1x2) {
  Input in;
  genMaze(1, 2, &in);
  ASSERT_EQ(in.row, 1);
  ASSERT_EQ(in.col, 2);
  ASSERT_EQ(check_no_access(&in), 0);
  ASSERT_EQ(check_loops(&in), 0);
}

TEST(test_genMaze, maze_2x1) {
  Input in;
  genMaze(2, 1, &in);
  ASSERT_EQ(in.row, 2);
  ASSERT_EQ(in.col, 1);
  ASSERT_EQ(check_no_access(&in), 0);
  ASSERT_EQ(check_loops(&in), 0);
}

TEST(test_genMaze, maze_15x15) {
  Input in;
  genMaze(15, 15, &in);
  ASSERT_EQ(in.row, 15);
  ASSERT_EQ(in.col, 15);
  ASSERT_EQ(check_no_access(&in), 0);
  ASSERT_EQ(check_loops(&in), 0);
}

TEST(test_genMaze, maze_50x50) {
  Input in;
  genMaze(50, 50, &in);
  ASSERT_EQ(in.row, 50);
  ASSERT_EQ(in.col, 50);
  ASSERT_EQ(check_no_access(&in), 0);
  ASSERT_EQ(check_loops(&in), 0);
}
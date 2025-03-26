#include <gtest/gtest.h>

#include "../backend/genMaze.h"
#include "../backend/solveMaze.h"

int check_no_access(Input *in) {
  for (int i = 0; i < in->row; i++) {
    std::pair<int, int> start(i, 0),
    end(i, in->col - 1);
    std::vector<pair<int, int>> path = SolveMaze(start, end, in);
    if (!path.size()) return 1;
  }
  for (int i = 0; i < in->col; i++) {
    std::pair<int, int> start(0, i),
    end(i, in->row - 1);
    std::vector<pair<int, int>> path = SolveMaze(start, end, in);
    if (!path.size()) return 1;
  }
  return 0;
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
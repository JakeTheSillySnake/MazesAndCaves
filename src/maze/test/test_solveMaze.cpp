#include <gtest/gtest.h>

#include "../backend/genMaze.h"
#include "../backend/solveMaze.h"

TEST(test_solveMaze, unsolvable) {
  Input in;
  genMaze(4, 4, &in);
  in.borderX[0][0] = 1;
  in.borderY[0][0] = 1;
  std::vector<std::pair<int, int>> test =
      SolveMaze(std::pair<int, int>{3, 3}, std::pair<int, int>{0, 0}, &in);
  ASSERT_EQ(test.size(), 0);
}

TEST(test_solveMaze, solve_1) {
  Input in;
  genMaze(4, 4, &in);
  std::vector<std::pair<int, int>> test =
      SolveMaze(std::pair<int, int>{3, 3}, std::pair<int, int>{0, 0}, &in);
  ASSERT_GT(test.size(), 0);
}

TEST(test_solveMaze, solve_2) {
  Input in;
  genMaze(50, 50, &in);
  std::vector<std::pair<int, int>> test =
      SolveMaze(std::pair<int, int>{48, 48}, std::pair<int, int>{0, 0}, &in);
  ASSERT_GT(test.size(), 0);
}

TEST(test_solveMaze, solve_3) {
  Input in;
  genMaze(25, 25, &in);
  std::vector<std::pair<int, int>> test =
      SolveMaze(std::pair<int, int>{0, 0}, std::pair<int, int>{24, 24}, &in);
  ASSERT_GT(test.size(), 0);
}

TEST(test_solveMaze, solve_4) {
  Input in;
  genMaze(1, 1, &in);
  std::vector<std::pair<int, int>> test =
      SolveMaze(std::pair<int, int>{0, 0}, std::pair<int, int>{0, 0}, &in);
  ASSERT_EQ(test.size(), 1);
}
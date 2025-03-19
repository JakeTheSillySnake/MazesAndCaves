#include <gtest/gtest.h>

#include "../backend/solveMaze.h"

TEST(test_solveMaze, unsolvable) {
  Input in;
  in.uploadFile("../assets/example_files/lab7_solve.txt", MAZE_MODE);
  std::vector<std::pair<int, int>> test =
      SolveMaze(std::pair<int, int>{3, 3}, std::pair<int, int>{0, 0}, &in);
  ASSERT_EQ(test.size(), 0);
}

TEST(test_solveMaze, solve_1) {
  Input in;
  in.uploadFile("../assets/example_files/lab7_solve.txt", MAZE_MODE);
  std::vector<std::pair<int, int>> test =
      SolveMaze(std::pair<int, int>{0, 3}, std::pair<int, int>{0, 0}, &in);
  ASSERT_EQ(test.size(), 4);
}

TEST(test_solveMaze, solve_2) {
  Input in;
  in.uploadFile("../assets/example_files/lab7_solve.txt", MAZE_MODE);
  std::vector<std::pair<int, int>> test =
      SolveMaze(std::pair<int, int>{0, 3}, std::pair<int, int>{0, 3}, &in);
  ASSERT_EQ(test.size(), 1);
}

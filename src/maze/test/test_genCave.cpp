#include <gtest/gtest.h>

#include "../backend/genCave.h"

TEST(test_genCave, cave_1x1) {
  Input in;
  genCave(1, 1, 50, &in);
  ASSERT_EQ(in.row, 1);
  ASSERT_EQ(in.col, 1);
  ASSERT_TRUE(in.borderX == nullptr);
  ASSERT_EQ(calcNeighbours(0, 0, &in), 8);
}

TEST(test_genCave, cave_15x15) {
  Input in;
  genCave(15, 15, 100, &in);
  ASSERT_EQ(in.row, 15);
  ASSERT_EQ(in.col, 15);
  ASSERT_TRUE(in.borderY == nullptr);
  ASSERT_EQ(calcNeighbours(0, 0, &in), 8);
}

TEST(test_genCave, cave_50x50) {
  Input in;
  genCave(50, 50, 0, &in);
  ASSERT_EQ(in.row, 50);
  ASSERT_EQ(in.col, 50);
  ASSERT_TRUE(in.tmp != nullptr);
  ASSERT_EQ(calcNeighbours(0, 0, &in), 5);
}

TEST(test_genCave, caveNext_1x1) {
  Input in;
  genCave(1, 1, 0, &in);
  genNextStep(1, 1, &in);
  ASSERT_EQ(in.tmp[0][0], 1);
  genNextStep(9, 9, &in);
  ASSERT_EQ(in.tmp[0][0], 0);
}

TEST(test_genCave, caveNext_50x50) {
  Input in;
  genCave(50, 50, 100, &in);
  genNextStep(1, 1, &in);
  int res = 0;
  for (int i = 0; i < in.row; i++) {
    for (int j = 0; j < in.col; j++) {
      if (!in.tmp[i][j]) res++;
    }
  }
  genNextStep(9, 9, &in);
  for (int i = 0; i < in.row; i++) {
    for (int j = 0; j < in.col; j++) {
      if (in.tmp[i][j]) res++;
    }
  }
  ASSERT_EQ(res, 0);
}

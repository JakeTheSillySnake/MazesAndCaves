#include <gtest/gtest.h>

#include "../backend/agent.h"
#include "../backend/genMaze.h"
#include "../backend/solveMaze.h"

TEST(test_agent, size4x4) {
  Input in;
  genMaze(4, 4, &in);
  Agent agent{&in};
  agent.LearnAgent({3, 3});
  std::vector<std::pair<int, int>> test = agent.GetPathFromPosition({0, 0});
  ASSERT_NE(test.size(), 0);
}

TEST(test_agent, size10x10) {
  Input in;
  genMaze(10, 10, &in);
  Agent agent;
  agent.LoadMaze(&in);
  agent.LearnAgent({9, 9});
  std::vector<std::pair<int, int>> test = agent.GetPathFromPosition({0, 0});
  ASSERT_NE(test.size(), 0);
}

TEST(test_solveMaze, size15x15) {
  Input in;
  genMaze(15, 15, &in);
  Parameters params;
  Penalties pens;
  Agent agent{&in};
  agent.SetupParams(params);
  agent.SetupPenalty(pens);
  agent.LearnAgent({7, 7});

  std::vector<std::pair<int, int>> test = agent.GetPathFromPosition({0, 0});
  auto Qtable = agent.GetQTable();
  auto cellVals = agent.GetQCell(7, 7);

  ASSERT_NE(test.size(), 0);
  ASSERT_NE(Qtable.size(), 0);
  ASSERT_NE(cellVals.size(), 0);
}
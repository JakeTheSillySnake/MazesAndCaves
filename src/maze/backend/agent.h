#ifndef __AGENT_H__
#define __AGENT_H__

#include <atomic>
#include <thread>
#include <vector>

#include "model.h"
#include "solveMaze.h"

enum class Actions { Left, Up, Right, Down };

struct Penalties {
  double wall_penalty{-1.0};
  double visited_penalty{-0.5};
  double move_penalty{-0.01};
  double target_reward{100.0};
};

struct Parameters {
  double epsilon{0.65};
  double decay_rate{0.001};
  double learning_rate{0.25};
  double discount{0.95};
  int max_moves{10000};
  int epochs{1000000};
};

class Agent {
 private:
  Input* maze_;

  std::vector<std::vector<double>> Q_table_{};
  std::vector<std::vector<int>> visited_matrix_{};
  std::pair<int, int> end_position_{};
  Parameters params_{};
  Penalties penalty_{};

 public:
  std::atomic_int progress{0};
  std::atomic_bool terminate{false};

  Agent(unsigned int seed = time({}));
  Agent(Input* maze, unsigned int seed = time({}));
  ~Agent() = default;

  void LoadMaze(Input* maze);
  void SetupParams(Parameters params);
  void SetupPenalty(Penalties penalty);
  void LearnAgent(std::pair<int, int> end_position);

  std::vector<pair<int, int>> GetPathFromPosition(std::pair<int, int> position);

  int GetQTableIndex(std::pair<int, int> position);
  void ResetQTable();
  std::pair<pair<int, int>, double> PerformAction(
      pair<int, int> current_position, Actions action,
      pair<int, int> end_position);

  std::vector<std::vector<double>> GetQTable();
  std::vector<double> GetQCell(int i, int j);

 private:
  Actions ChooseAction(std::pair<int, int> position, double epsilon);

  double UpdateEpsilon(int current_epoch);
};

#endif

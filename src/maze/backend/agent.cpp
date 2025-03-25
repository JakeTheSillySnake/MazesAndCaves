#include "agent.h"

Agent::Agent(unsigned int seed) { srand(seed); }

Agent::Agent(Input* maze, unsigned int seed) : Agent(seed) { maze_ = maze; }

void Agent::LoadMaze(Input* maze) {
  maze_ = maze;
  ResetQTable();
}

void Agent::SetupParams(Parameters params) { params_ = params; }

void Agent::SetupPenalty(Penalties penalty) { penalty_ = penalty; }

std::vector<pair<int, int>> Agent::GetPathFromPosition(
    std::pair<int, int> position) {
  std::vector<pair<int, int>> result{};
  result.push_back(position);
  int move = 0;
  while (position != end_position_ && move != maze_->col * maze_->row) {
    move++;
    int action = 0;
    double max = Q_table_[GetQTableIndex(position)][0];
    for (int i = 1; i < 4; i++) {
      if (Q_table_[GetQTableIndex(position)][i] > max) {
        max = Q_table_[GetQTableIndex(position)][i];
        action = i;
      }
    }
    if (action == 0) {
      if (position.second - 1 >= 0 &&
          maze_->borderX[position.first][position.second - 1] != 1) {
        position.second--;
      }
    }
    if (action == 1) {
      if (position.first - 1 >= 0 &&
          maze_->borderY[position.first - 1][position.second] != 1) {
        position.first--;
      }
    }
    if (action == 2) {
      if (position.second + 1 < maze_->col &&
          maze_->borderX[position.first][position.second] != 1) {
        position.second++;
      }
    }
    if (action == 3) {
      if (position.first + 1 < maze_->col &&
          maze_->borderY[position.first][position.second] != 1) {
        position.first++;
      }
    }
    result.push_back(position);
  }
  return result;
}

std::vector<std::vector<double>> Agent::GetQTable() { return Q_table_; }

std::vector<double> Agent::GetQCell(int i, int j) {
  try {
    return Q_table_.at(i * maze_->col + j);
  } catch (...) {
    return std::vector<double>(0);
  }
}

void Agent::LearnAgent(std::pair<int, int> end_position,
                       std::atomic_int* progress) {
  end_position_ = end_position;
  ResetQTable();
  double epsilon = params_.epsilon;

  for (int i = 0; i < params_.epochs; i++) {
    int move{0};
    bool done{false};
    std::pair<int, int> position{std::rand() % maze_->row,
                                 std::rand() % maze_->col};
    visited_matrix_ = std::vector<std::vector<int>>(
        maze_->row, std::vector<int>(maze_->col, 0));
    visited_matrix_[position.first][position.second] = 1;
    while (!done && move != params_.max_moves) {
      Actions action = ChooseAction(position, epsilon);
      auto result = PerformAction(position, action, end_position);
      std::pair<int, int> next_position = result.first;
      double reward = result.second;

      if (visited_matrix_[next_position.first][next_position.second] &&
          reward == penalty_.move_penalty) {
        reward = penalty_.visited_penalty * 0.25 *
                 visited_matrix_[next_position.first][next_position.second];
      } else {
        visited_matrix_[next_position.first][next_position.second]++;
      }
      double max_q = Q_table_[GetQTableIndex(next_position)][0];
      for (int i = 1; i < 4; i++) {
        if (Q_table_[GetQTableIndex(next_position)][i] > max_q) {
          max_q = Q_table_[GetQTableIndex(next_position)][i];
        }
      }
      Q_table_[GetQTableIndex(position)][static_cast<int>(action)] +=
          params_.learning_rate *
          (reward + (params_.discount * max_q) -
           Q_table_[GetQTableIndex(position)][static_cast<int>(action)]);
      move++;
      position = next_position;
      if (position == end_position) {
        done = true;
      }
    }
    epsilon = UpdateEpsilon(i);
    progress->fetch_add(1, std::memory_order_relaxed);
  }
}

void Agent::ResetQTable() {
  Q_table_ = std::vector<std::vector<double>>(maze_->col * maze_->row,
                                              std::vector<double>(4, 0));
}
int Agent::GetQTableIndex(std::pair<int, int> position) {
  return position.first * maze_->col + position.second;
}

Actions Agent::ChooseAction(std::pair<int, int> position, double epsilon) {
  if (static_cast<double>(std::rand()) / RAND_MAX < epsilon) {
    return static_cast<Actions>(std::rand() % 4);
  }

  int action = 0;
  double max = Q_table_[GetQTableIndex(position)][0];
  for (int i = 1; i < 4; i++) {
    if (Q_table_[GetQTableIndex(position)][i] > max) {
      max = Q_table_[GetQTableIndex(position)][i];
      action = i;
    }
  }

  return static_cast<Actions>(action);
}

std::pair<pair<int, int>, double> Agent::PerformAction(
    pair<int, int> position, Actions action, pair<int, int> end_position) {
  double reward = penalty_.move_penalty;
  if (action == Actions::Left) {
    if (position.second - 1 >= 0 &&
        maze_->borderX[position.first][position.second - 1] != 1) {
      position.second--;
    } else {
      reward = penalty_.wall_penalty;
    }
  }
  if (action == Actions::Up) {
    if (position.first - 1 >= 0 &&
        maze_->borderY[position.first - 1][position.second] != 1) {
      position.first--;
    } else {
      reward = penalty_.wall_penalty;
    }
  }
  if (action == Actions::Right) {
    if (position.second + 1 < maze_->col &&
        maze_->borderX[position.first][position.second] != 1) {
      position.second++;
    } else {
      reward = penalty_.wall_penalty;
    }
  }
  if (action == Actions::Down) {
    if (position.first + 1 < maze_->col &&
        maze_->borderY[position.first][position.second] != 1) {
      position.first++;
    } else {
      reward = penalty_.wall_penalty;
    }
  }

  if (position == end_position) {
    reward = penalty_.target_reward;
  }

  return {position, reward};
}

double Agent::UpdateEpsilon(int current_epoch) {
  double epsilon = params_.epsilon - params_.decay_rate * current_epoch;
  if (epsilon < 0.01) {
    epsilon = 0.01;
  }
  return epsilon;
}

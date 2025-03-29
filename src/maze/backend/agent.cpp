#include "agent.h"

/**
 * @brief Agent basic constructor
 *
 * Construct agent with empty maze
 *
 * @param seed random seed
 *
 */

Agent::Agent(unsigned int seed) { srand(seed); }

/**
 * @brief Agent additional constructor
 *
 * Construct agent with given maze and see
 * @param maze  Given maze
 * @param seed random seed
 *
 */

Agent::Agent(Input* maze, unsigned int seed) : Agent(seed) { maze_ = maze; }

/**
 * @brief Load maze
 *
 * Load maze in agent
 * @param maze  Given maze
 *
 */

void Agent::LoadMaze(Input* maze) {
  maze_ = maze;
  ResetQTable();
}

/**
 * @brief Setup paramaterers
 *
 * Setup given parameters for agent
 * @param params Given params
 *
 */

void Agent::SetupParams(Parameters params) { params_ = params; }

/**
 * @brief Setup paramaterers
 *
 * Сonfigures the penalty values for the agent, such as move penalty, wall
 * penalty, visited penalty, and target reward.
 * @param penalty given penalty
 *
 */

void Agent::SetupPenalty(Penalties penalty) { penalty_ = penalty; }

/**
 * @brief Computes the optimal path from a given starting position to the end
 * position using the learned Q-Table.
 *
 *
 * @param position start position
 *
 * @return A vector of positions representing the path. If no valid path is
 * found, the vector is cleared.
 *
 */

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
  if (move == maze_->col * maze_->row) {
    result.clear();
  }
  return result;
}
/**
 * @brief Retrieves the current Q-Table, which contains the learned Q-values for
 * each state-action pair.
 *
 *
 * @return A 2D vector representing the Q-Table.
 *
 */

std::vector<std::vector<double>> Agent::GetQTable() { return Q_table_; }

/**
 * @brief Retrieves the Q-values for a specific cell in the maze.
 *
 * @param i Row index
 * @param j Col index
 *
 * @return A vector of Q-values for the specified cell. If the cell is invalid,
 * an empty vector is returned.
 *
 */

std::vector<double> Agent::GetQCell(int i, int j) {
  try {
    return Q_table_.at(i * maze_->col + j);
  } catch (...) {
    return std::vector<double>(0);
  }
}

/**
 * @brief Trains the agent using Q-Learning over multiple epochs. The agent
 * learns to navigate from random starting positions to the specified end
 * position.
 *
 * @param end_position end position
 *
 */

void Agent::LearnAgent(std::pair<int, int> end_position) {
  end_position_ = end_position;
  ResetQTable();
  double epsilon = params_.epsilon;

  for (int i = 0; i < params_.epochs && !terminate.load(); i++) {
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
    progress.fetch_add(1, std::memory_order_relaxed);
  }
}

/**
 * @brief Resets the Q-Table to zero for all state-action pairs. This is
 * typically done when loading a new maze.
 *
 */

void Agent::ResetQTable() {
  Q_table_ = std::vector<std::vector<double>>(maze_->col * maze_->row,
                                              std::vector<double>(4, 0));
}

/**
 * @brief Converts a maze position (row, column) into a single index for
 accessing the Q-Table.
 * @param position The position in the maze.
 *
 * @return An integer index corresponding to the position in the Q-Table.
 */
int Agent::GetQTableIndex(std::pair<int, int> position) {
  return position.first * maze_->col + position.second;
}

/**
 * @brief Selects an action for the agent based on the epsilon-greedy policy.
 * With probability epsilon, a random action is chosen; otherwise, the action
 * with the highest Q-value is selected.
 *
 * @param position The current position in the maze.
 * @param epsilon The exploration rate.
 * @return The selected action
 */

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

/**
 * @brief Executes the given action in the maze and calculates the resulting
 * position and reward.
 *
 * @param position The current position in the maze.
 * @param action The action to perform
 * @param end_position The target position in the maze.
 *
 * @return A pair containing the new position and the reward received.
 */

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

/**
 * @brief Updates the exploration rate (epsilon) based on the current epoch.
 * Epsilon decreases over time to reduce exploration and increase exploitation.
 *
 * @param position  The current training epoch.
 *
 * @return The updated value of epsilon
 */

double Agent::UpdateEpsilon(int current_epoch) {
  double epsilon = params_.epsilon - params_.decay_rate * current_epoch;
  if (epsilon < 0.01) {
    epsilon = 0.01;
  }
  return epsilon;
}

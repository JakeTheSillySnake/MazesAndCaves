#include "solveMaze.h"

/**
 * @bried Solve given maze
 *
 * Solve maze given by Input from start_position to end_position
 * return zero size vector if path can't be found
 *
 * @param start_position start position
 * @param end_position end position
 * @param model Maze
 *
 * @return std::vector<pair<int, int>> -> path from end to start
 */

std::vector<pair<int, int>> SolveMaze(std::pair<int, int> start_position,
                                      std::pair<int, int> end_position,
                                      Input* model) {
  std::vector<std::vector<int>> visited =
      ConstructVisited(start_position, model);
  std::vector<std::pair<int, int>> result{};
  if (visited[end_position.first][end_position.second] != 0) {
    while (visited[end_position.first][end_position.second] != 1) {
      result.push_back(end_position);
      if (end_position.first + 1 < model->row &&
          visited[end_position.first + 1][end_position.second] ==
              visited[end_position.first][end_position.second] - 1 &&
          model->borderY[end_position.first][end_position.second] != 1) {
        end_position.first = end_position.first + 1;
      } else if (end_position.first - 1 >= 0 &&
                 visited[end_position.first - 1][end_position.second] ==
                     visited[end_position.first][end_position.second] - 1 &&
                 model->borderY[end_position.first - 1][end_position.second] !=
                     1) {
        end_position.first = end_position.first - 1;
      } else if (end_position.second + 1 < model->col &&
                 visited[end_position.first][end_position.second + 1] ==
                     visited[end_position.first][end_position.second] - 1 &&
                 model->borderX[end_position.first][end_position.second] != 1) {
        end_position.second = end_position.second + 1;
      } else if (end_position.second - 1 >= 0 &&
                 visited[end_position.first][end_position.second - 1] ==
                     visited[end_position.first][end_position.second] - 1 &&
                 model->borderX[end_position.first][end_position.second - 1] !=
                     1) {
        end_position.second = end_position.second - 1;
      }
    }
    result.push_back(end_position);
  }
  return result;
}

/**
 * @bried Construct visited table
 *
 * Construct visited table from start position
 *
 * @param start_position start position
 * @param model Maze
 *
 * @return std::vector<std::vector<int>>> -> Visited table
 */

std::vector<std::vector<int>> ConstructVisited(
    std::pair<int, int> start_position, Input* model) {
  std::queue<std::pair<int, int>> cells;
  cells.push(start_position);
  std::vector<vector<int>> visited(model->row, std::vector<int>(model->col, 0));
  visited[start_position.first][start_position.second] = 1;

  while (!cells.empty()) {
    std::pair<int, int> current_position = cells.front();
    cells.pop();

    // right cell
    if (current_position.second + 1 < model->col &&
        model->borderX[current_position.first][current_position.second] != 1) {
      if (visited[current_position.first][current_position.second + 1] == 0) {
        visited[current_position.first][current_position.second + 1] =
            visited[current_position.first][current_position.second] + 1;
        cells.push(std::pair<int, int>{current_position.first,
                                       current_position.second + 1});
      } else {
        visited[current_position.first][current_position.second] = std::min(
            visited[current_position.first][current_position.second],
            visited[current_position.first][current_position.second + 1] + 1);
      }
    }

    // left cell
    if (current_position.second - 1 >= 0 &&
        model->borderX[current_position.first][current_position.second - 1] !=
            1) {
      if (visited[current_position.first][current_position.second - 1] == 0) {
        visited[current_position.first][current_position.second - 1] =
            visited[current_position.first][current_position.second] + 1;
        cells.push(std::pair<int, int>{current_position.first,
                                       current_position.second - 1});
      } else {
        visited[current_position.first][current_position.second] = std::min(
            visited[current_position.first][current_position.second],
            visited[current_position.first][current_position.second - 1] + 1);
      }
    }

    // down cell
    if (current_position.first + 1 < model->row &&
        model->borderY[current_position.first][current_position.second] != 1) {
      if (visited[current_position.first + 1][current_position.second] == 0) {
        visited[current_position.first + 1][current_position.second] =
            visited[current_position.first][current_position.second] + 1;
        cells.push(std::pair<int, int>{current_position.first + 1,
                                       current_position.second});
      }

      else {
        visited[current_position.first][current_position.second] = std::min(
            visited[current_position.first][current_position.second],
            visited[current_position.first + 1][current_position.second] + 1);
      }
    }

    // upper cell
    if (current_position.first - 1 >= 0 &&
        model->borderY[current_position.first - 1][current_position.second] !=
            1) {
      if (visited[current_position.first - 1][current_position.second] == 0) {
        visited[current_position.first - 1][current_position.second] =
            visited[current_position.first][current_position.second] + 1;
        cells.push(std::pair<int, int>{current_position.first - 1,
                                       current_position.second});
      } else {
        visited[current_position.first][current_position.second] = std::min(
            visited[current_position.first][current_position.second],
            visited[current_position.first - 1][current_position.second] + 1);
      }
    }
  }
  return visited;
}
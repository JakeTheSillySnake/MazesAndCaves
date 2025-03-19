#ifndef __SOLVE_MAZE_H__
#define __SOLVE_MAZE_H__
#include <iostream>
#include <queue>
#include <vector>

#include "model.h"
std::vector<pair<int, int>> SolveMaze(std::pair<int, int> start_position,
                                      std::pair<int, int> end_position,
                                      Input* model);

std::vector<std::vector<int>> ConstructVisited(
    std::pair<int, int> start_position, Input* model);

#endif

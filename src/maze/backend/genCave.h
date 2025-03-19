#ifndef GENCAVE_H
#define GENCAVE_H

#include "model.h"

void genCave(int rows, int cols, int init_chance, Input *model);
void genNextStep(int birth_limit, int death_limit, Input *model);
int calcNeighbours(int r, int c, Input *model);

#endif
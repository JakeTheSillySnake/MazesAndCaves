#ifndef GENMAZE_H
#define GENMAZE_H

#include "model.h"

void genMaze(int rows, int cols, Input *model);
void genFirstRow(int *count, Input *model);
void genXBorder(int r, Input *model);
void genYBorder(int r, Input *model);
void mergeSets(int r, Input *model);
void findReplace(int r, int find, int replace, Input *model);
void genNextRow(int r, int *count, Input *model);
void mergeLastRow(int r, Input *model);

#endif
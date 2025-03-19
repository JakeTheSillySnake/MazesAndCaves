#include "maze.h"
#include "ui_maze.h"

void Maze::drawField(int mode) {
  scene->clear();
  int rows, cols;
  if (mode == MAZE_MODE) {
    rows = maze.row, cols = maze.col;
    ui->SolveStartRow->setMaximum(maze.row - 1);
    ui->SolveStartCol->setMaximum(maze.col - 1);
    ui->SolveEndRow->setMaximum(maze.row - 1);
    ui->SolveEndCol->setMaximum(maze.col - 1);
  } else
    rows = cave.row, cols = cave.col;
  float squareSizeY = sizeMax / float(rows / mazeMax),
        squareSizeX = sizeMax / float(cols / mazeMax);
  float rectW = rows * squareSizeY, rectH = cols * squareSizeX;
  float startX = (sceneSize - rectW) / 2.0 - 1,
        startY = (sceneSize - rectH) / 2.0 - 1;

  QPainterPath path;
  path.addRoundedRect(startX, startY, rectW, rectH, 5, 5);
  auto rect = scene->addPath(path);
  if (mode == EMPTY_MODE) {
    rect->setBrush(Qt::black);
    return;
  }
  rect->setBrush(Qt::white);
  QPen pen(Qt::darkGray);
  pen.setWidth(7);
  rect->setPen(pen);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      QPainterPath p;
      if (mode == MAZE_MODE) {
        if (maze.borderY[i][j]) {
          p.addRect(startX + j * (squareSizeX),
                    startY + (i + 1) * (squareSizeY), squareSizeX + 1, lineW);
        }
        if (maze.borderX[i][j]) {
          p.addRect(startX + (j + 1) * (squareSizeX),
                    startY + i * (squareSizeY), lineW, squareSizeY + 1);
        }
      } else if (mode == CAVE_MODE && cave.tmp[i][j]) {
        p.addRect(startX + j * (squareSizeX), startY + i * (squareSizeY),
                  squareSizeX, squareSizeY);
      }
      auto newP = scene->addPath(p);
      pen.setWidth(1);
      newP->setBrush(Qt::darkGray);
      newP->setPen(pen);
    }
  }
  ui->graphicsView->update();
}

void Maze::drawSolution(int start_r, int start_c,
                        std::vector<pair<int, int>> path) {
  int rows = maze.row, cols = maze.col;
  float squareSizeY = sizeMax / float(rows / mazeMax),
        squareSizeX = sizeMax / float(cols / mazeMax);
  float rectW = rows * squareSizeY, rectH = cols * squareSizeX;
  float startX = (sceneSize - rectW) / 2.0 - 1,
        startY = (sceneSize - rectH) / 2.0 - 1;
  float beginPathY = startY + start_r * (squareSizeY) + squareSizeY / 2;
  float beginPathX = startX + start_c * (squareSizeX) + squareSizeX / 2;

  QPainterPath p;
  QPen pen(Qt::green);
  pen.setWidth(2);
  pen.setCapStyle(Qt::RoundCap);
  p.moveTo(beginPathX + 1, beginPathY + 1);
  p.addEllipse(QPointF(beginPathX + 1, beginPathY + 1), 1, 1);
  auto newP = scene->addPath(p);
  newP->setBrush(Qt::green);
  newP->setPen(pen);

  int size = path.size();
  for (int i = 0; i < size; i++) {
    float pointY = startY + path[i].first * (squareSizeY) + squareSizeY / 2;
    float pointX = startX + path[i].second * (squareSizeX) + squareSizeX / 2;
    p.lineTo(pointX + 1, pointY + 1);
    p.moveTo(pointX + 1, pointY + 1);
    if (i == size - 1) p.addEllipse(QPointF(pointX + 1, pointY + 1), 1, 1);
    newP = scene->addPath(p);
    newP->setBrush(Qt::green);
    newP->setPen(pen);
  }
  ui->graphicsView->update();
}

#ifndef MAZE_H
#define MAZE_H

#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QErrorMessage>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QFormLayout>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QLineEdit>
#include <QMainWindow>
#include <QPainterPath>
#include <QTimer>

#include "../backend/genCave.h"
#include "../backend/genMaze.h"
#include "../backend/solveMaze.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Maze;
}
QT_END_NAMESPACE

class Maze : public QMainWindow {
  Q_OBJECT

 public:
  const float sceneSize = 500;
  const float lineW = 2;
  const float sizeMax = 9.9;
  const float mazeMax = 50;

  Maze(QWidget *parent = nullptr);
  ~Maze();

  // basic logic
  void loadFont();
  void connectSlots();
  void setScene();
  void errorMessage(int code);

  // drawing scene
  void drawField(int mode);
  void drawSolution(int start_r, int start_c, std::vector<pair<int, int>> path);

  // input processing
  int uploadFile(QByteArray fileContent, int mode, Input *model);
  int uploadMaze(QTextStream &data, Input *model);
  int uploadCave(QTextStream &data, Input *model);
  void setTimer();

 private slots:
  void loadMaze();
  void loadCave();
  void solveMazeIn();
  void genMazeIn();
  void genCaveIn();
  void genNext();
  void saveMaze();
  void updateBirthVal();
  void updateDeathVal();
  void toggleAutomode();
  void switchTab();
  void timerAlarm();

 private:
  Ui::Maze *ui;
  QFont *font;
  Input maze, cave;
  QErrorMessage *msg;
  QGraphicsScene *scene;
  QTimer *timer;
  int automode = 0, solved = 0;
  std::vector<std::pair<int, int>> solution;
  std::pair<int, int> endPoint;
};
#endif  // MAZE_H

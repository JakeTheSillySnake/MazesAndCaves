#include <QApplication>

#include "maze.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  Maze w;
  w.setWindowIcon(QIcon(QFileInfo("../assets/maze.png").absoluteFilePath()));
  w.show();
  return a.exec();
}

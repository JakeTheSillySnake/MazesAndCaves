#include <QApplication>

#include "maze.h"

int main(int argc, char *argv[]) {
  srand(time(NULL));
  QApplication a(argc, argv);
  Maze w;
  w.setWindowIcon(QIcon(QFileInfo("../assets/maze.png").absoluteFilePath()));
  w.setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint |
                   Qt::WindowCloseButtonHint | Qt::CustomizeWindowHint);
  w.show();
  return a.exec();
}

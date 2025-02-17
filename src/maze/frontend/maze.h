#ifndef MAZE_H
#define MAZE_H

#include <QMainWindow>
#include <QFontDatabase>
#include <QFileInfo>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class Maze; }
QT_END_NAMESPACE

class Maze : public QMainWindow
{
  Q_OBJECT

public:
  Maze(QWidget *parent = nullptr);
  ~Maze();

  void loadFont();
  void connectSlots();

private:
  Ui::Maze *ui;
  QFont *font;
};
#endif // MAZE_H

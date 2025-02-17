#include "maze.h"

#include "ui_maze.h"

Maze::Maze(QWidget *parent) : QMainWindow(parent), ui(new Ui::Maze) {
  ui->setupUi(this);
  loadFont();
  connectSlots();
}

void Maze::loadFont() {
  int id = QFontDatabase::addApplicationFont(
      QFileInfo("../assets/Raleway-Medium.ttf").absoluteFilePath());
  font = new QFont(QFontDatabase::applicationFontFamilies(id).at(0));
  font->setPixelSize(17);
  setFont(*font);
  std::vector<QMenu *> items = {ui->menuAlgorithm, ui->menuTools, ui->menuFile,
                                ui->menuUpload};
  for (int i = 0; i < (int)items.size(); i++) items[i]->setFont(*font);
  ui->menubar->setFont(*font);
}

void Maze::connectSlots() {
  connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
}

Maze::~Maze() { delete ui; }

#include "maze.h"
#include "ui_maze.h"

Maze::Maze(QWidget *parent) : QMainWindow(parent), ui(new Ui::Maze) {
  ui->setupUi(this);
  msg = new QErrorMessage(0);
  msg->setWindowTitle("Error");
  timer = new QTimer();
  loadFont();
  connectSlots();
  setScene();
}

void Maze::loadFont() {
  QFontDatabase::addApplicationFont(
      QFileInfo("../assets/purisa_font.ttf").absoluteFilePath());
  font = new QFont("Purisa");
  font->setBold(true);
  font->setPixelSize(15);
  ui->menuFile->setFont(*font);
  ui->menubar->setFont(*font);
  ui->tabWidget->setFont(*font);
}

void Maze::connectSlots() {
  connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
  connect(ui->actionMaze, SIGNAL(triggered()), this, SLOT(loadMaze()));
  connect(ui->actionCave, SIGNAL(triggered()), this, SLOT(loadCave()));
  connect(ui->SolveButton, SIGNAL(clicked()), this, SLOT(solveMazeIn()));
  connect(ui->actionSaveMaze, SIGNAL(triggered()), this, SLOT(saveMaze()));
  connect(ui->SaveMazeButton, SIGNAL(clicked()), this, SLOT(saveMaze()));
  connect(ui->GenMazeButton, SIGNAL(clicked()), this, SLOT(genMazeIn()));
  connect(ui->SliderBirth, SIGNAL(valueChanged(int)), this,
          SLOT(updateBirthVal()));
  connect(ui->SliderDeath, SIGNAL(valueChanged(int)), this,
          SLOT(updateDeathVal()));
  connect(ui->AutomodeButton, SIGNAL(clicked()), this, SLOT(toggleAutomode()));
  connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(switchTab()));
  connect(ui->GenCaveButton, SIGNAL(clicked()), this, SLOT(genCaveIn()));
  connect(ui->NextStepButton, SIGNAL(clicked()), this, SLOT(genNext()));
}

void Maze::setScene() {
  scene = new QGraphicsScene(this);
  ui->graphicsView->setScene(scene);
  ui->graphicsView->setRenderHint(QPainter::Antialiasing, true);
  QRect content = ui->graphicsView->contentsRect();
  ui->graphicsView->setSceneRect(0, 0, content.width(), content.height());
  toggleAutomode();
  ui->NextStepButton->setDisabled(true);
  ui->SolveButton->setDisabled(true);
}

void Maze::errorMessage(int code) {
  if (code == EMPTY)
    msg->showMessage("Oops! File is empty.");
  else if (code == NO_SOLUTION)
    msg->showMessage("No solution is possble.");
  else if (code == BAD_FORMAT)
    msg->showMessage("Mazes and caves should have 1 to 50 rows and columns.");
  else
    msg->showMessage("Oops! Couldn't process this file.");
}

Maze::~Maze() { delete ui; }

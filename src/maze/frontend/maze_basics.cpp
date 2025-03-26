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
  connect(ui->actionMaze, SIGNAL(triggered()), this, SLOT(getMazeInfo()));
  connect(ui->actionCave, SIGNAL(triggered()), this, SLOT(getCaveInfo()));
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
  connect(ui->TrainAgentButton, SIGNAL(clicked()), this, SLOT(trainAgent()));
  connect(ui->SolveAgentButton, SIGNAL(clicked()), this, SLOT(applyAgent()));
  connect(ui->CancelAgentButton, SIGNAL(clicked()), this, SLOT(cancelAgent()));
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
  ui->TrainAgentButton->setDisabled(true);
  ui->SolveAgentButton->setDisabled(true);
  ui->ProgessWindow->hide();
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

void Maze::updateBirthVal() {
  ui->BirthLabel->setNum(ui->SliderBirth->value());
  ui->BirthLabel->setAlignment(Qt::AlignCenter);
}

void Maze::updateDeathVal() {
  ui->DeathLabel->setNum(ui->SliderDeath->value());
  ui->DeathLabel->setAlignment(Qt::AlignCenter);
}

void Maze::toggleAutomode() {
  if (ui->AutomodeButton->isChecked()) {
    ui->FrameBox->setDisabled(false);
    ui->FrameSpeedLabel->setText(
        tr("<p style='color:white;font-size:10pt;'>Hold Frame For:</p>"));
    ui->NextStepButton->setDisabled(true);
    automode = 1;
    setTimer();
  } else {
    ui->FrameBox->setDisabled(true);
    ui->FrameSpeedLabel->setText(
        tr("<p style='color:gray;font-size:10pt;'>Hold Frame For:</p>"));
    if (cave.tmp && cave.filename == "") ui->NextStepButton->setDisabled(false);
    automode = 0;
  }
  ui->FrameSpeedLabel->setAlignment(Qt::AlignCenter);
}

void Maze::switchTab() {
  if (ui->tabWidget->currentIndex() == 0 && maze.borderX) {
    drawField(MAZE_MODE);
    if (solved) drawSolution(endPoint.first, endPoint.second, solution);
  } else if (ui->tabWidget->currentIndex() == 1 && cave.tmp)
    drawField(CAVE_MODE);
  else
    drawField(EMPTY_MODE);
}

Maze::~Maze() { delete ui; }

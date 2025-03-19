#include "maze.h"
#include "ui_maze.h"

void Maze::loadMaze() { loadFile(MAZE_MODE); }

void Maze::loadCave() {
  loadFile(CAVE_MODE);
  ui->NextStepButton->setDisabled(true);
}

void Maze::loadFile(int mode) {
  QString filename = QFileDialog::getOpenFileName(
      0, QObject::tr("Open File"), QDir::currentPath(), QObject::tr("*txt"));
  QString fullName = QFileInfo(filename).absoluteFilePath();
  auto path = fullName.toLocal8Bit().data();
  int error = 0;
  Input tmp;
  if (!filename.isEmpty()) {
    if (mode == MAZE_MODE) {
      tmp = maze;
      error = maze.uploadFile(path, MAZE_MODE);
      ui->tabWidget->setCurrentIndex(0);
    } else {
      tmp = cave;
      error = cave.uploadFile(path, CAVE_MODE);
      ui->tabWidget->setCurrentIndex(1);
    }
    if (error) {
      errorMessage(error);
      if (mode == MAZE_MODE)
        maze = tmp;
      else
        cave = tmp;
    } else {
      drawField(mode);
      ui->SolveButton->setDisabled(false);
      if (mode == MAZE_MODE) solved = 0;
    }
  }
}

void Maze::genMazeIn() {
  genMaze(ui->MazeRowsBox->value(), ui->MazeColsBox->value(), &maze);
  drawField(MAZE_MODE);
  ui->SolveButton->setDisabled(false);
  solved = 0;
}

void Maze::genCaveIn() {
  genCave(ui->CaveRowsBox->value(), ui->CaveColsBox->value(),
          ui->ChanceBox->value(), &cave);
  drawField(CAVE_MODE);
  if (!ui->AutomodeButton->isChecked()) ui->NextStepButton->setDisabled(false);
}

void Maze::genNext() {
  genNextStep(ui->SliderBirth->value(), ui->SliderDeath->value(), &cave);
  drawField(CAVE_MODE);
}

void Maze::saveMaze() {
  QString filename = QFileDialog::getSaveFileName(
      0, "Save file", QString("%1/maze.txt").arg(QDir::currentPath()), ".txt");
  if (filename.isEmpty()) return;
  auto path = filename.toLocal8Bit().data();
  maze.saveFile(path);
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
  qDebug() << maze.row << maze.col;
  if (ui->tabWidget->currentIndex() == 0 && maze.borderX) {
    drawField(MAZE_MODE);
    if (solved) drawSolution(endPoint.first, endPoint.second, solution);
  } else if (ui->tabWidget->currentIndex() == 1 && cave.tmp)
    drawField(CAVE_MODE);
  else
    drawField(EMPTY_MODE);
}

void Maze::setTimer() {
  timer->disconnect();
  connect(timer, SIGNAL(timeout()), this, SLOT(timerAlarm()));
  timer->start(ui->FrameBox->value());
}

void Maze::timerAlarm() {
  if (automode && ui->tabWidget->currentIndex() == 1 && cave.tmp) {
    drawField(CAVE_MODE);
    setTimer();
    genNext();
  }
}

void Maze::solveMazeIn() {
  std::pair<int, int> start(ui->SolveStartRow->value(),
                            ui->SolveStartCol->value()),
      end(ui->SolveEndRow->value(), ui->SolveEndCol->value());
  endPoint = end;
  solution = SolveMaze(start, end, &maze);
  if (!solution.size())
    errorMessage(NO_SOLUTION);
  else {
    drawField(MAZE_MODE);
    drawSolution(end.first, end.second, solution);
    solved = 1;
  }
}

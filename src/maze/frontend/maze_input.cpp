#include "maze.h"
#include "ui_maze.h"

void Maze::getMazeInfo() {
  auto fileContentReady = [this](const QString &filePath,
                                 const QByteArray &fileContent) {
    if (filePath.isEmpty()) return;
    Input tmp = maze;
    QString fullName = QFileInfo(filePath).absoluteFilePath();
    maze.filename = fullName.toLocal8Bit().data();
    int error = uploadFile(fileContent, MAZE_MODE, &maze);
    if (error) {
      errorMessage(error);
      maze = tmp;
    } else {
      drawField(MAZE_MODE);
      ui->SolveButton->setDisabled(false);
      ui->SolveAgentButton->setDisabled(true);
      ui->TrainAgentButton->setDisabled(false);
      ui->TrainAgentButton->setDisabled(false);
      solved = 0;
    }
    ui->tabWidget->setCurrentIndex(0);
  };
  QFileDialog::getOpenFileContent("*.txt", fileContentReady);
}

void Maze::getCaveInfo() {
  auto fileContentReady = [this](const QString &filePath,
                                 const QByteArray &fileContent) {
    if (filePath.isEmpty()) return;
    Input tmp = cave;
    QString fullName = QFileInfo(filePath).absoluteFilePath();
    cave.filename = fullName.toLocal8Bit().data();
    int error = uploadFile(fileContent, CAVE_MODE, &cave);
    if (error) {
      errorMessage(error);
      cave = tmp;
    } else {
      drawField(CAVE_MODE);
      ui->NextStepButton->setDisabled(true);
    }
    ui->tabWidget->setCurrentIndex(1);
  };
  QFileDialog::getOpenFileContent("*.txt", fileContentReady);
}

int Maze::uploadFile(QByteArray fileContent, int mode, Input *model) {
  model->clear();
  if (fileContent.isEmpty()) return EMPTY;
  QTextStream data(fileContent);
  data >> model->row >> model->col;
  if (model->row < 1 || model->col < 1 || model->row > 50 || model->col > 50)
    return BAD_FORMAT;
  if (mode == MAZE_MODE)
    return uploadMazeFile(data, model);
  else
    return uploadCaveFile(data, model);
}

int Maze::uploadMazeFile(QTextStream &data, Input *model) {
  model->borderX = new int *[model->row];
  model->borderY = new int *[model->row];
  for (int i = 0; i < model->row; i++) {
    model->borderY[i] = new int[model->col];
    model->borderX[i] = new int[model->col];
  }
  for (int i = 0; i < model->row; i++) {
    for (int j = 0; j < model->col; j++) {
      data >> model->borderX[i][j];
      if ((model->borderX[i][j] != 1 && model->borderX[i][j] != 0) ||
          data.status() != QTextStream::Ok) {
        return BAD_DATA;
      }
    }
  }
  for (int i = 0; i < model->row; i++) {
    for (int j = 0; j < model->col; j++) {
      data >> model->borderY[i][j];
      if ((model->borderY[i][j] != 1 && model->borderY[i][j] != 0) ||
          data.status() != QTextStream::Ok) {
        return BAD_DATA;
      }
    }
  }
  return 0;
}

int Maze::uploadCaveFile(QTextStream &data, Input *model) {
  model->tmp = new int *[model->row];
  for (int i = 0; i < model->row; i++) model->tmp[i] = new int[model->col];
  for (int i = 0; i < model->row; i++) {
    for (int j = 0; j < model->col; j++) {
      data >> model->tmp[i][j];
      if ((model->tmp[i][j] != 1 && model->tmp[i][j] != 0) ||
          data.status() != QTextStream::Ok) {
        return BAD_DATA;
      }
    }
  }
  return 0;
}

void Maze::genMazeIn() {
  genMaze(ui->MazeRowsBox->value(), ui->MazeColsBox->value(), &maze);
  drawField(MAZE_MODE);
  ui->SolveButton->setDisabled(false);
  ui->TrainAgentButton->setDisabled(false);
  ui->SolveAgentButton->setDisabled(true);
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
  QByteArray fileContent;
  fileContent.append(QByteArray::number(maze.row));
  fileContent.append(" ");
  fileContent.append(QByteArray::number(maze.col));
  fileContent.append("\n");
  for (int i = 0; i < maze.row; i++) {
    for (int j = 0; j < maze.col; j++) {
      fileContent.append(QByteArray::number(maze.borderX[i][j]));
      fileContent.append(" ");
    }
    fileContent.append("\n");
  }
  fileContent.append("\n");
  for (int i = 0; i < maze.row; i++) {
    for (int j = 0; j < maze.col; j++) {
      fileContent.append(QByteArray::number(maze.borderY[i][j]));
      fileContent.append(" ");
    }
    fileContent.append("\n");
  }
  QFileDialog::saveFileContent(fileContent, "maze.txt");
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

void Maze::trainAgent() {
  ui->SolveAgentButton->setDisabled(true);
  agent.LoadMaze(&maze);
  std::pair<int, int> end(ui->SolveEndRow->value(), ui->SolveEndCol->value());
  Parameters params;
  agent.progress.store(0);
  agent.terminate.store(false);
  std::thread workThread(&Agent::LearnAgent, &agent, end);

  ui->ProgessWindow->show();
  ui->progressBar->setValue(0);
  ui->progressBar->update();

  while (agent.progress.load(std::memory_order_relaxed) < params.epochs && !agent.terminate.load()) {
    std::this_thread::sleep_for(50ms);
    ui->progressBar->setValue(agent.progress.load(std::memory_order_relaxed));
    ui->progressBar->update();
    QCoreApplication::processEvents();
  }
  workThread.join();
  ui->ProgessWindow->hide();
  if (!agent.terminate.load())
    ui->SolveAgentButton->setDisabled(false);
}

void Maze::applyAgent() {
  std::pair<int, int> start(ui->SolveStartRow->value(),
                            ui->SolveStartCol->value()),
      end = endPoint;
  solution = agent.GetPathFromPosition(start);
  endPoint = start;
  if (solution[0] == solution[solution.size() - 1] && end != start)
    errorMessage(NO_SOLUTION);
  else {
    drawField(MAZE_MODE);
    drawSolution(endPoint.first, endPoint.second, solution);
    solved = 1;
  }
}

void Maze::cancelAgent() {
  agent.terminate.store(true);
}

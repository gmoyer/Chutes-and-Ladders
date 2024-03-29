#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    newHomeWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newGameWindow() {
    gameWindow = new GameWindow();
    setCentralWidget(gameWindow);
    connect(gameWindow, &GameWindow::gameOver, this, &MainWindow::gameOver);
}
void MainWindow::newHomeWindow() {
    homeWindow = new HomeWindow();
    setCentralWidget(homeWindow);
    connect(homeWindow, &HomeWindow::startGame, this, &MainWindow::startGame);
}

void MainWindow::startGame(QVector<Player *> players) {
    newGameWindow();
    gameWindow->setPlayers(players);
    gameWindow->startTurn();
}

void MainWindow::gameOver(QVector<Player*> players, bool gameWon) {
    newHomeWindow();
    if (gameWon)
        for (Player* player : players)
            player->addGame();
    homeWindow->updatePlayers(players);
    homeWindow->updateCSV();
}

#include <QTimer>

#include "gamewindow.h"
#include "ui_gamewindow.h"

#include "board.h"
#include "player.h"
#include "event.h"
#include "windialog.h"

QColor GameWindow::playerColors[GameWindow::maxPlayerCount] = {
    QColor(255, 0, 0),
    QColor(0, 255, 0),
    QColor(0, 0, 255),
    QColor(255, 255, 0)
};

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    //Create board view
    QGraphicsView *boardView = ui->boardGraphics;
    QRect boardRect = boardView->contentsRect();
    Board::width = boardRect.width();
    Board::height = boardRect.height();
    Board::rowHeight = Board::height / Board::rows;
    Board::colWidth = Board::width / Board::cols;

    //initial game variables
    activePlayerNumber = 0;

    //Create graphics scenes
    boardScene = new QGraphicsScene;
    boardView->setScene(boardScene);
    boardView->setSceneRect(0,0,Board::width,Board::height);

    spawnScene = new QGraphicsScene;
    ui->spawnGraphics->setScene(spawnScene);
    ui->spawnGraphics->setSceneRect(0, 0, Board::colWidth, Board::rowHeight);

    //seed random numbers
    srand(static_cast<unsigned>(QTime::currentTime().msec()));

    //Create and add the board to the scene
    board = new Board(boardScene);

    turnTimer = new QTimer(this);
    connect(turnTimer, &QTimer::timeout, this, &GameWindow::tickTurn);

    advanceTimer = new QTimer(this);
    connect(advanceTimer, &QTimer::timeout, this, &GameWindow::advanceOnBoard);

    buttonsDisabled = false;
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::setPlayers(QVector<Player*> p) {
    if (p.size() > maxPlayerCount) {
        qDebug() << "Too many players!";
        return;
    }
    players = p;
    int i = 0;
    for (Player* player : players) {
        spawnScene->addItem(player);
        player->setPlayerNum(i);
        player->setColor(playerColors[i]);
        i++;
    }
    boardScene->update();
}

void GameWindow::startTurn() {
    buttonsDisabled = false;
    playerMoved = false;
    activePlayer = players[activePlayerNumber];
    secondsLeft = 10;
    diceRoll = 0;
    updateLabels();

    turnTimer->start(1000);
}

void GameWindow::tickTurn() {
    updateLabels();

    if (secondsLeft <= 0) {
        advanceTurn();
    } else {
        secondsLeft--;
    }
}

void GameWindow::rollDice() {
    diceRoll = (rand() % 6) + (rand() % 6) + 2;

    updateLabels();
}

void GameWindow::advanceTurn() {
    ui->rollButton->setText("Roll");
    turnTimer->stop();
    buttonsDisabled = true;
    if (activePlayer->getOnBoard()) {
        advanceTimer->start(250);
    } else {
        if (diceRoll == 6) {
            activePlayer->setOnBoard(true);
            spawnScene->removeItem(activePlayer);
            boardScene->addItem(activePlayer);
        }
        endTurn();
    }
}

void GameWindow::advanceOnBoard() {
    if (diceRoll <= 0) {
        int playerSquare = activePlayer->getSquare();
        Event* event = board->getEvent(playerSquare);

        if (playerMoved && event != nullptr && event->startsOn(playerSquare)) {
            activePlayer->setSquare(event->getEnd());
            boardScene->update();
        }

        endTurn();
    } else {
        diceRoll--;
        activePlayer->advanceSquare();
        boardScene->update();
        playerMoved = true;
    }
}

void GameWindow::endTurn() {
    advanceTimer->stop();
    ++activePlayerNumber %= players.size();
    if (activePlayer->getOnBoard())
        activePlayer->addPreviousMove(activePlayer->getSquare());
    if (activePlayer->getSquare() >= Board::squares - 1) {
        activePlayer->addWin();
        WinDialog dialog(this);
        dialog.setWinner(activePlayer->getName());
        dialog.exec();
        emit gameOver(players, true);
    }
    startTurn();
}

void GameWindow::updateLabels() {
    ui->timerLabel->setText(QString("Time left: %1").arg(QString::number(secondsLeft)));
    ui->diceLabel->setText(QString("Dice: %1").arg(diceRoll == 0 ? "" : QString::number(diceRoll)));
    ui->playerLabel->setText(QString("%1's turn").arg(activePlayer->getName()));
    ui->rerollButton->setText(QString("Reroll (%1 left)").arg(activePlayer->getRerollCount()));
    ui->undoButton->setText(QString("Undo (%1 left)").arg(activePlayer->getUndoCount()));

    QString playerColor = activePlayer->getColor().name();
    ui->playerLabel->setStyleSheet(QString("background-color: %1").arg(playerColor));
}

void GameWindow::on_rollButton_clicked()
{
    if (buttonsDisabled)
        return;
    if (diceRoll == 0) { //first roll
        ui->rollButton->setText("Next Turn");
        rollDice();
    } else { //next turn
        advanceTurn();
    }
}


void GameWindow::on_rerollButton_clicked()
{
    if (buttonsDisabled)
        return;
    if (diceRoll != 0 && activePlayer->getRerollCount() > 0) {
        activePlayer->useReroll();
        rollDice();
    }
}


void GameWindow::on_undoButton_clicked()
{
    if (buttonsDisabled)
        return;
    activePlayer->undoPreviousMove();
    boardScene->update();

}

void GameWindow::on_quitButton_clicked()
{
    emit gameOver(players, false);
}


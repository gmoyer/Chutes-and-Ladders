#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QTime>
#include <QValidator>
#include <QRegularExpression>
#include <QWidget>
#include "board.h"
#include "player.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GameWindow;
}
QT_END_NAMESPACE

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    void setPlayers(QVector<Player*> players);
    void startTurn();
    void advanceTurn();
    void endTurn();

    void tickTurn();
    void rollDice();
    void advanceOnBoard();

    void updateLabels();

private slots:
    void on_rollButton_clicked();

    void on_rerollButton_clicked();

    void on_undoButton_clicked();
    void on_quitButton_clicked();

signals:
    void gameOver(QVector<Player*> p, bool gameWon);

private:
    Ui::GameWindow *ui;
    Board* board;
    QGraphicsScene *boardScene;
    QGraphicsScene *spawnScene;
    QTimer* turnTimer;
    QTimer* advanceTimer;
    bool buttonsDisabled;

    static const int maxPlayerCount{4};
    static QColor playerColors[maxPlayerCount];

    QVector<Player*> players;
    Player* activePlayer;
    int activePlayerNumber;
    int secondsLeft;
    int diceRoll;
    bool playerMoved;
};

#endif // GAMEWINDOW_H

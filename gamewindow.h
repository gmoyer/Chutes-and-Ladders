#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

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

    void rollDice();
    void advanceOnBoard();

    void updateLabels();

signals:
    void gameOver(QVector<Player*> p); //sends updated players vector where player who won gets win added

private slots:
    void on_rollButton_clicked();
    void on_rerollButton_clicked();
    void on_undoButton_clicked();
    void on_quitButton_clicked();

private:
    Ui::GameWindow *ui;
    Board* board;
    QTimer* timer;
    bool buttonsDisabled;

    static const int maxPlayerCount{4};
    static QColor playerColors[maxPlayerCount];

    QVector<Player*> players;
    Player* activePlayer;
    int secondsLeft;
    int diceRoll;
};

#endif // GAMEWINDOW_H

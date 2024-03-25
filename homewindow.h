#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "player.h"
#include "leaderboard.h"

namespace Ui {
class HomeWindow;
}

class HomeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();
    void updatePlayers(QVector<Player*> p);
    void loadCSV();
    void updateCSV();

    void updateTable();

signals:
    void startGame(QVector<Player*> p);

private slots:
    void on_startButton_clicked();
    void on_prevButton_clicked(); //for leaderboard
    void on_nextButton_clicked(); //for leaderboard

private:
    Ui::HomeWindow *ui;
    static const QString filename;
    QVector<Player*> players;
};

#endif // HOMEWINDOW_H

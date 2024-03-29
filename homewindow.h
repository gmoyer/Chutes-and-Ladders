#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QWidget>
#include <fstream>
#include <sstream>
#include <QAbstractTableModel>
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

private slots:
    void on_startButton_clicked();
    void on_prevButton_clicked();

    void on_nextButton_clicked();

signals:
    void startGame(QVector<Player*> p);

private:
    Ui::HomeWindow *ui;
    static const QString filename;
    QMap<QString, Player*> playerMap;
    LeaderBoard leaderBoard;
};

#endif // HOMEWINDOW_H

#include "homewindow.h"
#include "ui_homewindow.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>

const QString HomeWindow::filename = "players.csv";

HomeWindow::HomeWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomeWindow)
{
    ui->setupUi(this);

    ui->leaderboard->setModel(&leaderBoard);
    ui->leaderboard->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    loadCSV();
    updateTable();
}

HomeWindow::~HomeWindow()
{
    delete ui;
}

void HomeWindow::loadCSV() {
    QFile file(filename);
    bool status = file.open(QIODevice::ReadOnly);
    if (!status) {
        qDebug() << "Load CSV:" << file.errorString();
        return;
    }
    while (!file.atEnd()) {
        QList<QByteArray> line = file.readLine().split(',');
        if (line.size() != 3) {
            qDebug() << "csv has row that does not have required 3 columns";
            return;
        }
        QString name = line[0];
        int wins = line[1].toInt();
        int games = line[2].toInt();
        Player* player = new Player(name);
        player->setWinCount(wins);
        player->setGameCount(games);
        playerMap[name] = player;
    }
}

void HomeWindow::updateCSV() {
    QFile file(filename);
    bool status = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!status) {
        qDebug() << "Update CSV:" << file.errorString();
        return;
    }

    QTextStream out(&file);

    for (Player* player : playerMap.values()) {
        out << player->getName() << ',';
        out << player->getWinCount() << ',';
        out << player->getGameCount() << '\n';
    }

    file.close();
}

void HomeWindow::updatePlayers(QVector<Player *> p) {
    for (Player* player : p) {
        playerMap[player->getName()] = player;
    }
    updateTable();
}

void HomeWindow::updateTable() {
    leaderBoard.setPlayerData(playerMap.values());
    ui->leaderboardStatus->setText(leaderBoard.status());
    ui->nextButton->setVisible(leaderBoard.nextButton());
    ui->prevButton->setVisible(leaderBoard.prevButton());
}

void HomeWindow::on_startButton_clicked()
{
    QVector<Player*> players;
    QString playerNames[] = {
        ui->player1Name->text(),
        ui->player2Name->text(),
        ui->player3Name->text(),
        ui->player4Name->text()
    };

    for (QString name : playerNames) {
        if (name != "") {
            if (playerMap.contains(name)) {
                playerMap[name]->reset();
                players.push_back(playerMap[name]);
            }
            else
                players.push_back(new Player(name));
        }
    }

    if (players.size() < 2) {
        return;
    } else {
        emit startGame(players);
    }

}

void HomeWindow::on_prevButton_clicked()
{
    leaderBoard.prevSet();
    updateTable();
}


void HomeWindow::on_nextButton_clicked()
{
    leaderBoard.nextSet();
    updateTable();
}


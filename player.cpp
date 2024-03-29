#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>

#include "player.h"
#include "board.h"

Player::Player(QString n) {
    name = n;
    winCount = 0;
    gameCount = 0;

    color = QColor(0, 0, 0);
    size = QSize(20, 20);

    reset(); //set initial game state
}

void Player::reset() {
    square = 0;
    rerollCount = 5;
    undoCount = 5;

    onBoard = false;
}

QRectF Player::boundingRect() const {
    return QRectF(0,0, Board::width, Board::height);
}

QPainterPath Player::shape() const
{
    QPainterPath path;
    path.addRect(0,0, Board::width, Board::height);
    return path;
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    int width = size.width();
    int height = size.height();
    int heightMargin = (Board::rowHeight - (height * 2)) / 3; //formula for max 4 players
    int widthMargin = (Board::colWidth - (width * 2)) / 3; //formula for max 4 players

    QBrush b = painter->brush();
    QPen p = painter->pen();
    painter->setBrush(QBrush(color));
    //painter->setPen(QPen(color, 5));

    QPoint pos = onBoard ? Board::getSquarePoint(square) : QPoint(0, 0);
    QPoint offset(widthMargin, heightMargin);

    //adjust based off of which player we are
    if (playerNum / 2 == 1) {
        offset += QPoint(widthMargin + width, 0);
    }
    if (playerNum % 2 == 1) {
        offset += QPoint(0, heightMargin + height);
    }

    painter->drawEllipse(QRect(pos + offset, size));

    painter->setBrush(b);
    painter->setPen(p);
}


void Player::undoPreviousMove() {
    if (previousMoves.size() < 2 || undoCount <= 0)
        return;

    previousMoves.pop_back();

    int n = previousMoves.size() - 1;
    square = previousMoves[n];

    undoCount--;
}

double Player::getWinPercentage() const {
    if (gameCount == 0)
        return 0;
    else
        return (winCount / (gameCount+0.0)) * 100;
}

QString Player::getWinPercentageString() const {
    if (gameCount <= 0)
        return "0%";
    QString p = QString::number(getWinPercentage(), 'f', 1);
    return QString("%1\%").arg(p);
}

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>

#include "board.h"
#include "event.h"
#include "player.h"

int Board::width = 0;
int Board::height = 0;
int Board::rowHeight = 0;
int Board::colWidth = 0;

Board::Board(QGraphicsScene* scene) : scene(scene) {
    color = QColor(0, 0, 0);

    for (int i = 0; i < squares; i++)
        events[i] = nullptr;

    scene->addItem(this);

    //creating initial chutes and ladders
    Event* event;
    int start = 0;
    int end = 0;
    int tolerance;
    for (int i = 0; i < chuteCount; i++) {
        tolerance = 10000;
        do {
            start = (rand() % (Board::squares - Board::cols - 1)) + Board::cols;
        } while (events[start] != nullptr && tolerance-- > 0);
        if (tolerance <= 0)
            qDebug("Could not find valid position for chute");
        tolerance = 10000;
        do {
            end = rand() % start;
        } while ((start / 10 == end / 10 || events[end] != nullptr) && tolerance-- > 0);
        if (tolerance <= 0)
            qDebug("Could not find valid position for chute");
        event = new Chute(start, end);
        events[start] = event;
        events[end] = event;
        scene->addItem(event);
    }

    for (int i = 0; i < ladderCount; i++) {
        tolerance = 10000;
        do {
            start = rand() % (Board::squares - Board::cols - 1) + 1;
        } while (events[start] != nullptr && tolerance-- > 0);
        if (tolerance <= 0)
            qDebug("Could not find valid position for ladder");
        tolerance = 10000;
        do {
            end = start + 1 + rand() % (Board::squares - start - 1);
        } while ((start / 10 == end / 10 || events[end] != nullptr) && tolerance-- > 0);
        if (tolerance <= 0)
            qDebug("Could not find valid position for ladder");
        event = new Ladder(start, end);
        events[start] = event;
        events[end] = event;
        scene->addItem(event);
    }

}

QRectF Board::boundingRect() const {
    return QRectF(0,0, Board::width, Board::height);
}

QPainterPath Board::shape() const
{
    QPainterPath path;
    path.addRect(0,0, Board::width, Board::height);
    return path;
}


void Board::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    QBrush b = painter->brush();
    painter->setBrush(QBrush(color));

    for (int i = 1; i < rows; i++) {
        painter->drawLine(0, i * rowHeight, Board::width, i * rowHeight);
    }
    for (int i = 1; i < cols; i++) {
        painter->drawLine(i * colWidth, 0, i * colWidth, Board::height);
    }

    //draw square numbers
    for (int i = 0; i < squares; i++) {
        painter->drawText(getSquarePoint(i) + QPoint(0, rowHeight), QString::number(i+1));
    }

    //painter->drawRect(QRect(0, 0, 10, 10));

    //reset brush back to what it was
    painter->setBrush(b);
}

Event* Board::getEvent(int square) {
    if (square < 0 || square >= squares)
        return nullptr;
    return events[square];
}

QPoint Board::getSquarePoint(int square) {
    int squareRow = square / rows;
    int squareCol = square % rows;

    if (squareRow % 2 == 1) //back and forth board shape
        squareCol = cols - squareCol - 1;

    squareRow = rows - squareRow - 1; //board starts at bottom

    return QPoint(squareCol * colWidth, squareRow * rowHeight);
}

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>

#include "event.h"
#include "board.h"


Event::Event(int s, int e) {
    color = QColor(0, 0, 0);
    start = s;
    end = e;
    QPoint offset(Board::colWidth / 2, Board::rowHeight / 2);
    startPoint = Board::getSquarePoint(start) + offset;
    endPoint = Board::getSquarePoint(end) + offset;
}



QRectF Event::boundingRect() const {
    return QRectF(0,0, Board::width, Board::height);
}

QPainterPath Event::shape() const
{
    QPainterPath path;
    path.addRect(0,0, Board::width, Board::height);
    return path;
}


void Event::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    QPen p = painter->pen();
    painter->setPen(QPen(color, 5));

    painter->drawLine(startPoint, endPoint);

    //reset brush back to what it was
    painter->setPen(p);
}


Chute::Chute(int s, int e) : Event(s, e) {
    color = QColor(0, 0, 255);
}

Ladder::Ladder(int s, int e) : Event(s, e) {
    color = QColor(161, 102, 47);
}

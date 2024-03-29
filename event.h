#ifndef EVENT_H
#define EVENT_H

#include <QColor>
#include <QGraphicsItem>

class Board;

class Event : public QGraphicsItem
{
protected:
    QColor color;

    int start;
    int end;
    QPoint startPoint;
    QPoint endPoint;
public:
    Event(int s, int e);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    operator QString() const {
        return QString("Start: %1, End: %2").arg(QString::number(start), QString::number(end));
    }

    bool startsOn(int square) { return square == start; }
    int getEnd() { return end; }
};

class Chute : public Event {
public:
    Chute(int s, int e);
};

class Ladder : public Event {
public:
    Ladder(int s, int e);
};

#endif // EVENT_H

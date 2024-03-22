#ifndef EVENT_H
#define EVENT_H

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

#ifndef BOARD_H
#define BOARD_H

class Board : public QGraphicsItem
{
public:
    static int width;
    static int height;
    static int rowHeight;
    static int colWidth;

    static const int rows{10};
    static const int cols{10};
    static const int squares{rows*cols};
    static const int chuteCount{10};
    static const int ladderCount{10};

    Board(QGraphicsScene* scene);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    static QPoint getSquarePoint(int square);
    Event* getEvent(int square);
private:
    Event* events[squares];
};

#endif // BOARD_H

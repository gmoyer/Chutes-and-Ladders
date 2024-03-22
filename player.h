#ifndef PLAYER_H
#define PLAYER_H

class Player : public QGraphicsItem
{
private:
    QString name;
    int winCount;
    int gameCount;

    int square;
    int playerNum;
    bool onBoard;

    int rerollCount;
    int undoCount;
    QVector<int> previousMoves;

    QColor color;
    QSize size;
public:
    Player(QString name);

    QString getName() { return name; }
    int getSquare() { return square; }
    void setSquare(int newSquare) { square = newSquare; }
    void advanceSquare() { square ++; }
    void addPreviousMove(int move) { previousMoves.push_back(move); }
    int getRerollCount() { return rerollCount; }
    void useReroll() { rerollCount--; }
    int getUndoCount() { return undoCount; }
    void setPlayerNum(int num) { playerNum = num; }
    void setColor(QColor pColor) { color = pColor; }
    QColor getColor() { return color; }
    bool getOnBoard() { return onBoard; }
    void setOnBoard(bool ob) { onBoard = ob; }
    void setWinCount(int w) { winCount = w; }
    void addWin() { winCount++; }
    int getWinCount() { return winCount; }
    void setGameCount(int g) { gameCount = g; }
    void addGame() { gameCount++; }
    int getGameCount() { return gameCount; }
    bool operator<(const Player &other) const {
        return getWinPercentage() < other.getWinPercentage();
    }
    bool operator>(const Player &other) const {
        return getWinPercentage() > other.getWinPercentage();
    }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void undoPreviousMove();
    double getWinPercentage() const;
};

#endif // PLAYER_H

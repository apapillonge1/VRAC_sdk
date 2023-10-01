#ifndef ROBOT_H
#define ROBOT_H

#include <QGraphicsPixmapItem>

class Robot : public QGraphicsPixmapItem
{
public:
    Robot();

    void setPos(QPointF pos);
    QPointF pos();

    int theta() const;
    void setTheta(int newTheta);

    void addItemHandler(QString name, QPointF pos, int radius);
    QGraphicsItem* getItemHandler(QString name) {return mItemHandlers[name].get();}

private:
    int mTheta;
    QMap<QString, std::shared_ptr<QGraphicsItem>> mItemHandlers;
};

#endif // ROBOT_H

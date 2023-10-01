#include "robot.h"

Robot::Robot()
{
    setFlags(QGraphicsItem::ItemIsMovable);
}

void Robot::setPos(QPointF pos)
{
    QGraphicsPixmapItem::setPos(pos - boundingRect().center());
}

QPointF Robot::pos()
{
    return sceneBoundingRect().center();
}

int Robot::theta() const
{
    return mTheta;
}

void Robot::setTheta(int newTheta)
{
    mTheta = newTheta;
}

void Robot::addItemHandler(QString name, QPointF pos, int radius)
{
    mItemHandlers[name] = std::shared_ptr<QGraphicsItem>(new QGraphicsEllipseItem(QRectF(-radius, -radius, radius * 2, radius * 2), this));
    mItemHandlers[name]->setPos(pos);
}

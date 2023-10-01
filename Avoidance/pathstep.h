#ifndef PATHSTEP_H
#define PATHSTEP_H

#include <QPointF>
#include <QPen>
#include <QGraphicsPathItem>

typedef struct PathStep
{
    PathStep(const QPointF s, const QPointF c1, const QPointF c2, const QPointF g);

    PathStep(QPointF s, QPointF g);

    void setStart(const QPointF s);

    QPointF start;
    QPointF checkPoint1;
    QPointF checkPoint2;
    QPointF goal;

    std::shared_ptr<QGraphicsPathItem> uiItem;
} PathStep;

#endif // PATHSTEP_H

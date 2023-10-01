#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QPolygonF>
#include <QGraphicsPolygonItem>

typedef struct Obstacle
{
    Obstacle(QPointF centerPos, int radius, const QVector<QPointF>& outer= QVector<QPointF>());
    Obstacle(QPolygonF poly, QPolygonF polyAvoidance = QPolygonF());

    static QPolygonF generatePolygon(QPointF centerPos, int radius);

    QPolygonF poly;
    QPolygonF polyAvoidance;
    std::shared_ptr<QGraphicsPolygonItem> uiItem;
    std::shared_ptr<QGraphicsPolygonItem> uiItemAvoidance;
} Obstacle;

#endif // OBSTACLE_H

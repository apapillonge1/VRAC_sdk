#include "obstacle.h"

#include <QPen>

#include "gamestate.h"

Obstacle::Obstacle(QPointF centerPos, int radius, const QVector<QPointF> &outer)
{
    poly = outer;
    // can be reduced to improve calculation time but will reduce precision of the avoidance
    poly.append(generatePolygon(centerPos, radius));

    uiItem = std::make_shared<QGraphicsPolygonItem>(poly);
    uiItem->setFlags(QGraphicsItem::ItemIsMovable);
    uiItemAvoidance = std:: make_shared<QGraphicsPolygonItem>();
    uiItemAvoidance->setParentItem(uiItem.get());

    polyAvoidance = generatePolygon(centerPos, radius + GameState::get()->robot().boundingRect().width() * 0.8);
    uiItemAvoidance->setPolygon(polyAvoidance);

    uiItem->setBrush(QBrush(Qt::black));
    uiItem->setOpacity(0.5);
    uiItemAvoidance->setPen(QPen(Qt::green, 20));
}

Obstacle::Obstacle(QPolygonF poly, QPolygonF polyAvoidance)
{
    uiItem = std::make_shared<QGraphicsPolygonItem>(poly);

    if (polyAvoidance.isEmpty()) return;
    uiItemAvoidance = std::make_shared<QGraphicsPolygonItem>(polyAvoidance);
    uiItemAvoidance->setParentItem(uiItem.get());
}

QPolygonF Obstacle::generatePolygon(QPointF centerPos, int radius)
{
    QPolygonF octogon;
    const int vertices = 8;
    for(int i=0; i< vertices; i++)
    {
        QPointF octoPoint;
        octoPoint.setX(centerPos.x() + radius * cos((i + 0.5) * 2 * 3.1415 / vertices));
        octoPoint.setY(centerPos.y() + radius * sin( (i + 0.5) * 2 * 3.1415 / vertices));
        octogon.append(octoPoint);
    }

    return octogon;
}

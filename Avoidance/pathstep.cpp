#include "pathstep.h"

#include "gamestate.h"

PathStep::PathStep(const QPointF s, const QPointF c1, const QPointF c2, const QPointF g) : start(s), checkPoint1(c1), checkPoint2(c2), goal(g) {
    QPainterPath bezier;
    bezier.moveTo(s);
    bezier.cubicTo(checkPoint1, checkPoint2, g);

    uiItem = std::make_shared<QGraphicsPathItem>(bezier);
    QPen pen(Qt::blue, GameState::get()->robot().boundingRect().width());
    uiItem->setPen(pen);
    uiItem->setOpacity(0.3);
}

PathStep::PathStep(QPointF s, QPointF g) : start(s), checkPoint1(s), checkPoint2(g), goal(g) {
    QPainterPath bezier;
    bezier.moveTo(s);
    bezier.cubicTo(checkPoint1, checkPoint2, g);

    uiItem = std::make_shared<QGraphicsPathItem>(bezier);
    QPen pen(Qt::blue, GameState::get()->robot().boundingRect().width());
    uiItem->setPen(pen);
    uiItem->setOpacity(0.3);
}

void PathStep::setStart(const QPointF s)
{
    if (start == checkPoint1) {
        checkPoint1 = s;
        start = s;
    }

    QPainterPath bezier;
    bezier.moveTo(start);
    bezier.cubicTo(checkPoint1, checkPoint2, goal);
    uiItem->setPath(bezier);
}

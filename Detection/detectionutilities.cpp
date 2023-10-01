#include "detectionutilities.h"

#include <qmath.h>


QPointF DetectionUtilities::calculatePosition(const QPointF &pos, int currentAngle, int dist, int angle)
{
    QPointF calculatedPos = {};
    double rad = (angle + currentAngle) * M_PI / 180.0;
    calculatedPos.setX(pos.x() + dist * qSin(rad));
    calculatedPos.setY(pos.y() + dist * qCos(rad));
    return calculatedPos;
}

QPointF DetectionUtilities::calculatePosition(const QPointF &pos, double currentRad, int dist, double rad)
{
    QPointF calculatedPos = {};
    rad += currentRad;
    calculatedPos.setX(pos.x() + dist * qSin(rad));
    calculatedPos.setY(pos.y() + dist * qCos(rad));
    return calculatedPos;
}

#ifndef DETECTIONUTILITIES_H
#define DETECTIONUTILITIES_H

#include <QPointF>
#include <QVector>

namespace DetectionUtilities
{
    QPointF calculatePosition(const QPointF& pos, int currentAngle, int dist, int angle);
    QPointF calculatePosition(const QPointF& pos, double currentRad, int dist, double rad);
};

#endif // DETECTIONUTILITIES_H

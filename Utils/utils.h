#ifndef UTILS_H
#define UTILS_H

#include <QPointF>

QPointF toScaled(const QPointF& realPoint);
QPointF toReal(const QPointF& scaledPoint);
bool isInArenaScaled(const QPointF& scaledPoint);
bool isInArenad(const QPointF& realPoint);

#endif // UTILS_H

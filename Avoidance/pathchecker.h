#ifndef PATHCHECKER_H
#define PATHCHECKER_H

#include <QPolygonF>
#include <QGraphicsPolygonItem>
#include <QGraphicsPathItem>

#include "pathstep.h"
#include "Scenes/obstacle.h"

enum PathStatus {
    VALID,
    DANGEROUS,
    INVALID
};

class PathChecker
{
    public :
        PathChecker();

        static bool isAreaFree(QPolygonF &area, const QVector<Obstacle>& obstacles);
        static PathStatus checkPath(const QVector<PathStep>& path, bool ignoreStaticObstacles);
        static PathStatus checkPath(const PathStep &path, bool ignoreStaticObstacles);
        static PathStatus checkPath(const QPointF& start, const QPointF& goal, bool ignoreStaticObstacles);
};

#endif // PATHCHECKER_H

#ifndef PATHPLANNER_H
#define PATHPLANNER_H

#include <QPointF>
#include <QVector>

#include "pathstep.h"
#include "Scenes/obstacle.h"

typedef struct vertex
{
    vertex(QPointF p, qreal d = 0xFFFF){ pos = p, distance = d, visited = false; previous = nullptr;}
    QPointF pos;
    qreal distance;
    bool visited;

    QVector<std::shared_ptr<vertex>>neighbors;
    std::shared_ptr<vertex> previous;
} vertex;

class PathPlanner
{

public :
    PathPlanner();

    static QVector<PathStep> findPath(const QVector<PathStep> &);
    static QVector<PathStep> findPath(const PathStep&);
    static QVector<PathStep> findPath(const QPointF& start, const QPointF& goal);

private:
    static QVector<std::shared_ptr<vertex>> gatherVertices(const QPointF& start, const QPointF& goal);
    static void setupNeighbors(QVector<std::shared_ptr<vertex>>& vertices);
    static std::shared_ptr<vertex> getShortestPathRewinder(QVector<std::shared_ptr<vertex>>& vertices, const QPointF& goal);
    static std::shared_ptr<vertex> minDist(QVector<std::shared_ptr<vertex>> &vector);
};

#endif // PATHPLANNER_H

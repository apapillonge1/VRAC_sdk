#include "pathplanner.h"

#include "pathchecker.h"
#include "gamestate.h"

PathPlanner::PathPlanner()
{

}

QVector<PathStep> PathPlanner::findPath(const QVector<PathStep> &path)
{
    if (path.isEmpty()) return QVector<PathStep>();

    return findPath(path.front().start, path.back().goal);
}

QVector<PathStep> PathPlanner::findPath(const PathStep &path)
{
    return findPath(path.start, path.goal);
}

QVector<PathStep> PathPlanner::findPath(const QPointF &start, const QPointF& goal)
{
    QVector<std::shared_ptr<vertex>> vertices = gatherVertices(start, goal);
    setupNeighbors(vertices);
    std::shared_ptr<vertex> rewinder = getShortestPathRewinder(vertices, goal);

    // No path available, resetting to straight line to goal, for the moment
    if ( rewinder == nullptr)
    {
        return QVector<PathStep>();
    }

    QVector<PathStep> newPath;

    //BackTracking the path from the rewinder
    while (rewinder->previous != nullptr)
    {
        newPath.emplaceFront(rewinder->previous->pos, rewinder->pos); // straight line
        rewinder = rewinder->previous;
    }

    return newPath;
}

QVector<std::shared_ptr<vertex>> PathPlanner::gatherVertices(const QPointF& start, const QPointF& goal)
{
    QVector<std::shared_ptr<vertex>> vertices;
    vertices <<  std::make_shared<vertex>(vertex(start, 0));

    for (auto &ennemy : GameState::get()->playground().obstacles())
    {
        QPolygonF avoid = ennemy.uiItemAvoidance->mapToScene(ennemy.polyAvoidance);
        for (auto vertice : avoid)
        {
            vertices << std::make_shared<vertex>(vertex(vertice, 0xFFFF)); //0xFFFF for max dist
        }
    }

    for (auto &obstacle : GameState::get()->playground().staticObstacles())
    {
        for (auto vertice : obstacle.polyAvoidance)
        {
            vertices << std::make_shared<vertex>(vertex(vertice, 0xFFFF)); //0xFFFF for max dist
        }
    }

    vertices << std::make_shared<vertex>(vertex(goal, 0xFFFF)); //0xFFFF for max dist
    return vertices;
}

void PathPlanner::setupNeighbors(QVector<std::shared_ptr<vertex> > &vertices)
{
    for (const auto &currentVertex : vertices)
    {
        if (currentVertex == nullptr) continue;

        std::copy_if(vertices.begin(), vertices.end(), std::back_inserter(currentVertex->neighbors),
            [&](std::shared_ptr<vertex> v)
            {
                if (v == nullptr) return false;
                if (v->pos == currentVertex->pos) return false;
                return PathChecker::checkPath(currentVertex->pos, v->pos, false) == PathStatus::VALID;
            });
    }
}

std::shared_ptr<vertex> PathPlanner::getShortestPathRewinder(QVector<std::shared_ptr<vertex> > &vertices, const QPointF& goal)
{
    std::shared_ptr<vertex> rewinder = nullptr;

    for (int ind = 0; ind < vertices.size(); ind++)
    {
        //take the non visited vertice with the minimum dist in the graph
        std::shared_ptr<vertex> currentVertex = minDist(vertices);

        if (currentVertex == nullptr) continue;

        currentVertex->visited = true;

        auto goalExist = [=](std::shared_ptr<vertex> neighbor)
        {
            if (neighbor == nullptr) return false;
            qreal totalDist = currentVertex->distance + QLineF(currentVertex->pos, neighbor->pos).length();
            // If it's shorter !
            if (totalDist < neighbor->distance && neighbor->visited == false)
            {
                neighbor->distance = totalDist;
                neighbor->previous = currentVertex;
            }
            return neighbor->pos == goal;
        };

        auto ret = std::find_if(currentVertex->neighbors.begin(), currentVertex->neighbors.end(), goalExist);

        if (ret != currentVertex->neighbors.end())
        {
            if (rewinder == nullptr) rewinder = *ret;
            else if ((*ret).get()->distance < rewinder.get()->distance)
            {
                rewinder = *ret;
            }
        }
    }

    return rewinder;
}

std::shared_ptr<vertex> PathPlanner::minDist(QVector<std::shared_ptr<vertex> > &vertices)
{
    int minimum = 0xFFFF;

    std::shared_ptr<vertex> minDistVertex = nullptr;

    for (const auto &vertex : vertices)
    {
        if (vertex == nullptr) continue;

        if (vertex->distance <= minimum && vertex->visited == false)
        {
            minimum = vertex->distance;
            minDistVertex = vertex;
        }
    }
    return minDistVertex;
}


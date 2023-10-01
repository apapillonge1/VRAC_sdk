#include "pathchecker.h"

#include <QPen>

#include "gamestate.h"

PathChecker::PathChecker()
{

}

bool PathChecker::isAreaFree(QPolygonF &area, const QVector<Obstacle>& obstacles)
{
    QGraphicsPolygonItem areaItem(area);
    for (auto& obstacle : obstacles) {
        obstacle.uiItem->collidesWithItem(&areaItem);
        return false;
    }
    return true;
}

PathStatus PathChecker::checkPath(const QVector<PathStep> &path, bool ignoreStaticObstacles)
{
    for (auto& step : path) {
        for (auto& obstacle : GameState::get()->playground().obstacles()) {
            if (obstacle.uiItem->collidesWithItem(step.uiItem.get())) {
                step.uiItem->setPen(QPen(Qt::red, GameState::get()->robot().boundingRect().width()));
                return PathStatus::INVALID;
            }
            else {
                step.uiItem->setPen(QPen(Qt::blue, GameState::get()->robot().boundingRect().width()));
            }
        }

        if (ignoreStaticObstacles) continue;

        for (auto& obstacle : GameState::get()->playground().staticObstacles()) {
            if (obstacle.uiItem->collidesWithItem(step.uiItem.get())) return PathStatus::INVALID;
        }
    }
    return PathStatus::VALID;
}

PathStatus PathChecker::checkPath(const PathStep &path, bool ignoreStaticObstacles)
{
    for (auto& obstacle : GameState::get()->playground().obstacles()) {
        if (obstacle.uiItem->collidesWithItem(path.uiItem.get())) {
            path.uiItem->pen().setColor(Qt::red);
            return PathStatus::INVALID;
        }
        else {
            path.uiItem->setPen(QPen(Qt::blue, GameState::get()->robot().boundingRect().width()));
        }
    }

    if (ignoreStaticObstacles) return PathStatus::VALID;

    for (auto& obstacle : GameState::get()->playground().staticObstacles()) {
        if (obstacle.uiItem->collidesWithItem(path.uiItem.get())) return PathStatus::INVALID;
    }

    return PathStatus::VALID;
}

PathStatus PathChecker::checkPath(const QPointF& start, const QPointF &goal, bool ignoreStaticObstacles)
{
    PathStep path(start, goal);
    path.uiItem->setPen(QPen(Qt::blue, GameState::get()->robot().boundingRect().width()));
    return PathChecker::checkPath(path, ignoreStaticObstacles);
}

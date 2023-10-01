#ifndef PLANNER_H
#define PLANNER_H

#include <QObject>
#include <set>

#include "../../gamestate.h"
#include "task.h"
#include "goal.h"

using Plan = QVector<ITask*>;

class Planner
{
public:
    Planner();

    Plan getPlan(Goal *goal, const QVariantMap& blackBoard = {});

    void setTaskPool(const QVector<ITask *> &newTaskPool);

private:
    struct Node {
        ITask *task;
        QVariantMap state;
        QVector<Node> childrens;
    };

    bool buildPlans(Node &step, const QVariantMap& blackBoard);

    Plan findBestPlan(QVariantMap& desiredState, const QVariantMap& blackBoard);
    Plan findCheapestPlan(QVector<Plan>& plans, const QVariantMap& blackBoard);

    QVector<Plan> gatherAllPlans(const Node& rootNode, const QVariantMap& blackBoard) const;

private:
    QVector<ITask*> mTaskPool;
};

#endif // PLANNER_H

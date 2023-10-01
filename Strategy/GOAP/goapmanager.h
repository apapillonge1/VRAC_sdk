#ifndef GOAPMANAGER_H
#define GOAPMANAGER_H

#include "../strategymanager.h"
#include "planner.h"
#include "goal.h"
#include "task.h"

class GoapManager : public StrategyManager
{
public:
    GoapManager(QObject *parent = nullptr);
    ~GoapManager();

    void addTask(ITask* task);
    void removeTask(QString taskName);
    void addGoal(Goal* goal);
    void removeGoal(QString goalName);
    void followPlan();
    virtual void run() override;
    ITask& currentTask() const;

public slots:
    void update();
    void onTaskFinished();

private:
    void setupNewGoal(Goal *newGoal);
    Goal* selectGoal(Goal * = nullptr);

    QTimer *mCadencer;
    Planner mPlanner;
    Goal* mCurrentGoal;
    ITask* mCurrentTask;
    QVector<ITask*> mPlan;
    QVector<ITask*> mTaskPool;
    QVector<Goal*> mGoalPool;
};

#endif // GOAPMANAGER_H

#include "goapmanager.h"

GoapManager::GoapManager(QObject *parent) : StrategyManager(parent)
{
}

GoapManager::~GoapManager()
{
    for (auto task : mTaskPool) delete task;
    for (auto goal : mGoalPool) delete goal;

    mTaskPool.clear();
    mGoalPool.clear();
}

void GoapManager::addTask(ITask* task)
{
    mTaskPool.push_back(task);
}

void GoapManager::removeTask(QString taskName)
{
    mTaskPool.removeIf([&](const auto& task)
    {
        return (task->name() == taskName);
    });
}

void GoapManager::addGoal(Goal* goal)
{
    mGoalPool.push_back(goal);
}

void GoapManager::removeGoal(QString goalName)
{
    mGoalPool.removeIf([&](const auto& goal)
    {
        return (goal->name() == goalName);
    });
}

void GoapManager::run() {
    mCadencer = new QTimer();
    connect(mCadencer, &QTimer::timeout, this, &GoapManager::update);
    mCadencer->start(100);
    mPlanner.setTaskPool(mTaskPool);
    exec();
}

void GoapManager::setupNewGoal(Goal *newGoal)
{
    if (newGoal == nullptr)
    {
        return;
    }

    // GOAL CHANGED ! Recalc plan
    if (newGoal != mCurrentGoal) mCurrentGoal = newGoal;


    qDebug() << "Computing plan for goal : " << mCurrentGoal->name();
    mPlan = mPlanner.getPlan(mCurrentGoal, GameState::get()->states());

    if (mPlan.isEmpty() || mPlan.front() == nullptr)
    {
        qDebug() << mCurrentGoal->name() << "is valid but no plan was found...";
        setupNewGoal(selectGoal(mCurrentGoal));
    }
}

void GoapManager::update()
{
    setupNewGoal(selectGoal());
    followPlan();
}

void GoapManager::followPlan()
{
    if (mPlan.isEmpty()) {
        mCurrentGoal = nullptr;
        return;
    }

    if (mCurrentTask != mPlan.front())
    {
        mCurrentTask = mPlan.front();
        connect(mCurrentTask, &ITask::finished, this, &GoapManager::onTaskFinished, Qt::QueuedConnection);
        mCurrentTask->run();
    }
}

void GoapManager::onTaskFinished()
{
    mPlan.pop_front();
    disconnect(static_cast<ITask*>(sender()), &ITask::finished, this, &GoapManager::onTaskFinished);
    mCurrentTask = nullptr;
}


ITask& GoapManager::currentTask() const
{
    return *mCurrentTask;
}

Goal *GoapManager::selectGoal(Goal * invalidGoal)
{
    Goal *highestPrio = nullptr;
    for (const auto &goal : mGoalPool)
    {
        if (goal->isValid() && goal != invalidGoal && (highestPrio == nullptr || highestPrio->priority() < goal->priority()))
        {
            highestPrio = goal;
        }
    }
    return highestPrio;
}

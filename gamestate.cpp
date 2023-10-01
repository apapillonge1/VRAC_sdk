#include "gamestate.h"

#include <QFile>
#include <QMutexLocker>

GameState::GameState(QObject *parent) : QObject(parent)
{
    playground().addItem(&mRobot);
}

void GameState::clearState()
{
    QMutexLocker locker(&mStatesLock);
    mStates.clear();
}

void GameState::setState(QString name, QVariant value)
{
    QMutexLocker locker(&mStatesLock);
    mStates[name] = value;
}

QVariant GameState::getState(QString name)
{
    QMutexLocker locker(&mStatesLock);
    return mStates[name];
}

Playground &GameState::playground()
{
    return mPlayground;
}

Robot &GameState::robot()
{
    return mRobot;
}

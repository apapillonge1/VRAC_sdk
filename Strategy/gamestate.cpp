#include "gamestate.h"

#include <QFile>
#include <QMutexLocker>

GameState::GameState(QObject *parent) : QObject(parent)
{

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

void GameState::clearElements()
{
    QMutexLocker locker(&mStatesLock);
    mElements.clear();
}

void GameState::addElement(QString groupName, GameElement* element)
{
    QMutexLocker locker(&mStatesLock);
    mElements[groupName].push_back(std::move(element));
}

void GameState::removeElement(QString groupName, QString name)
{
    QMutexLocker locker(&mStatesLock);
    qDebug() << "Ennemies Total : " << mElements[groupName].size();
    mElements[groupName].removeIf([&](const auto& element){
        qDebug() << element->name() << "&" << name;
        return element->name() == name;
    });
    qDebug() << "Ennemies Total after : " << mElements[groupName].size();
}

const GameElement &GameState::getClosestElement(QString groupName, QPointF currentPos) const
{
    int closestDist = 0xFFFFFFFF;

    GameElement *closest;

    for (auto& element : mElements[groupName])
    {
        int dist = QLineF(currentPos, element->entryPoint()).length();

        if (dist < closestDist)
        {
            closest = element;
            closestDist = dist;
        }
    }
    return *closest;
}

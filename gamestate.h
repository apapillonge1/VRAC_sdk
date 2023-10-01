#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QMap>
#include <QString>
#include <QPointF>
#include <QMutex>
#include <QObject>
#include <QGraphicsPolygonItem>
#include <Utils/Singleton.h>

#include "Scenes/playground.h"
#include "Scenes/robot.h"

class GameState : public QObject, public Singleton<GameState>
{
    Q_OBJECT
    friend class Singleton<GameState>;
public:
    ~GameState() {}

    void clearState();
    void setState(QString name, QVariant value);
    QVariant getState(QString name);
    const QVariantMap &states() { return mStates; }

    Playground &playground();
    Robot &robot();

private:
    explicit GameState(QObject *parent = nullptr);

    QMutex mStatesLock;
    QVariantMap mStates;

    Playground mPlayground;
    Robot mRobot;
};

#endif // GAMESTATE_H

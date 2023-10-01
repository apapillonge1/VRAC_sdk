#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <QMap>
#include <QString>
#include <QPointF>
#include <QMutex>
#include <QObject>
#include <QGraphicsPolygonItem>
#include <Utils/Singleton.h>

#include "Scenes/gameelement.h"

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


    void clearElements();
    void addElement(QString groupName, GameElement* element);
    void removeElement(QString groupName, QString name);

    QVector<GameElement*>& getElements(QString groupName) { return mElements[groupName]; }
    const GameElement& getClosestElement(QString groupName, QPointF currentPos) const;

    const QVector<QGraphicsPolygonItem*> &obstacles() const { return mObstacles; }
    void resetObstacles();

private:
    explicit GameState(QObject *parent = nullptr);
    QMutex mStatesLock;
    QVariantMap mStates;
    QMutex mElementsLock;
    QMap<QString, QVector<GameElement*>> mElements;
    QMutex mObstaclesLock;
    QVector<QGraphicsPolygonItem*> mObstacles;
};

#endif // GAMESTATE_H

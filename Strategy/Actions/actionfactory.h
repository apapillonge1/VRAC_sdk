#ifndef ACTIONFACTORY_H
#define ACTIONFACTORY_H

#include <QObject>
#include <QMap>

#include "Utils/Singleton.h"

class State;

class ActionFactory : public Singleton<ActionFactory>
{
    friend class Singleton<ActionFactory>;

public:
    State* getAction(QString tag, QVariantMap params);

    void addAction(QString tag, std::function<State *(QString, QVariantMap)>);

private:
    ActionFactory();

    QMap<QString, std::function<State *(QString, QVariantMap)>> mMap;
};

#endif // ACTIONFACTORY_H

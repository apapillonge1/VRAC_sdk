#ifndef __STATE_H
#define __STATE_H

#include <QObject>
#include <QSet>
#include <QVariant>

#include "event.h"

class Transition;

class State : public QObject
{
    Q_OBJECT
public:
    State(QString name);
    State(QString name, QVariantMap params);

    void addTransition(Transition *t) {mTransitions.insert(t);}
    const QSet<Transition *> &transitions() const;
    void setTransitions(const QSet<Transition *> &newTransitions);

    void addParam(QString key, QVariant value) { mParams[key] = value;}
    void setParams(const QVariantMap& newParams) { mParams = newParams; }
    const QVariantMap& effects() const {return mParams;};

    bool testCheckSum(uint16_t checkSum);
    uint16_t checkSum();

signals :
    void sendEvent(Event e);

public slots:
    virtual bool onEvent(Event);
    virtual void onEntry(Event&) = 0;
    virtual void onExit(Event&) = 0;

protected:
    QSet<Transition*> mTransitions;
    QVariantMap mParams;

    uint16_t mCheckSum = 0xFFFF;
    uint16_t mCheckCounter = 0xFFFF;
};

#endif

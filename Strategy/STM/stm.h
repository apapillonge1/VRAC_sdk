#ifndef STM_H
#define STM_H

#include "state.h"
#include <QObject>

class Stm : public State
{
    Q_OBJECT

public:
    Stm(QString name);
    ~Stm();

    virtual void onEntry(Event&) override;
    virtual void onExit(Event&) override;

    void addState(State *s);
    const QSet<State *> &states() const;
    void setStates(const QSet<State *> &newStates);

    QSet<State *> &currentStates();
    void setCurrentStates(const QSet<State *> &newCurrentStates);
    void setInitialState(State *s);

signals:
    void finished();

public slots:
    virtual bool onEvent(Event e) override;

private:
    QSet<State*> mStates;
    QSet<State*> mCurrentStates;
    State* mInitialState;
};

#endif // STM_H

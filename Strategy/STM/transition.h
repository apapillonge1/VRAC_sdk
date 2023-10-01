#ifndef TRANSITION_H
#define TRANSITION_H

#include <QString>
#include <QObject>
#include <QSet>

class State;
struct Event;

class Transition : public QObject
{
    Q_OBJECT
public:
    Transition(const QString &value);

    State *sourceState() const;
    void setSourceState(State *newSourceState);

    void addTargetState (State *newTargetState);
    const QSet<State *> &targetStates() const;
    void setTargetStates(const QSet<State *> &newTargetStates);

    bool eventTest(Event &e);

private:
    QString mValue;
    State *mSourceState;
    QSet <State*> mTargetStates;
};
#endif // TRANSITION_H

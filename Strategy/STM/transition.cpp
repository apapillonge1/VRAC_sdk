#include "transition.h"
#include "state.h"
#include "event.h"

Event::Event(const QString &value, uint16_t checkSum) :
    m_value(value),
    mCheckSum(checkSum)
{
}


Transition::Transition(const QString &value)
    : mValue(value)
{
    setObjectName(value);
}

bool Transition::eventTest(Event& e)
{
    State *source = sourceState();

    if (!source) return false;

    if ((mValue == e.m_value) && (source->testCheckSum(e.mCheckSum)))
    {
        QString allStates = sourceState()->objectName() + " -> ";

        for (const auto &state : targetStates())
        {
            allStates += state->objectName();
        }

        return true;
    }

    return false;
}

void Transition::addTargetState(State *newTargetState)
{
    mTargetStates.insert(newTargetState);
}

const QSet<State *> &Transition::targetStates() const
{
    return mTargetStates;
}

void Transition::setTargetStates(const QSet<State *> &newTargetStates)
{
    mTargetStates = newTargetStates;
}

State *Transition::sourceState() const
{
    return mSourceState;
}

void Transition::setSourceState(State *newSourceState)
{
    mSourceState = newSourceState;
}

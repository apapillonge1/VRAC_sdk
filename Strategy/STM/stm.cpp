#include "stm.h"

#include <QtConcurrent>

#include "transition.h"


bool Stm::onEvent(Event e)
{
    for (auto it = mCurrentStates.begin(); it != mCurrentStates.end(); it++) {
        // if a transition happend in the current state sub states, stopping here.
        if ((*it)->onEvent(e))
        {
            return true;
        }
        // if no transition possible, exiting
        if ((*it)->transitions().isEmpty())
        {
            qDebug() << (*it)->objectName() << "onExit";
            (*it)->onExit(e);
            it = mCurrentStates.erase(it);

            if (mCurrentStates.isEmpty()) finished();
            return false;
        }

        // now checking the transitions of the current States
        for (Transition *t : (*it)->transitions())
        {
            if (t->eventTest(e))
            {
                qDebug() << (*it)->objectName() << "onExit";
                (*it)->onExit(e);
                // The transition is valid !
                it = mCurrentStates.erase(it);

                for (auto ts : t->targetStates())
                {
                    it = mCurrentStates.insert(ts);
                    qDebug() << (*it)->objectName() << "onEntry";
                    (*it)->onEntry(e);
                    sendEvent(Event("NoEvent"));
                }

                return true;
            }
        }
    }

    return false;
}

Stm::Stm(QString name) : State(name)
{
    connect(this, &State::sendEvent, this, &State::onEvent, Qt::QueuedConnection);
}

Stm::~Stm()
{

}

void Stm::onEntry(Event& e)
{
    if (mInitialState == nullptr) {
        finished();
        return;
    }

    mCurrentStates.clear();
    mCurrentStates.insert(mInitialState);
    mInitialState->onEntry(e);
}

void Stm::onExit(Event&)
{
}

void Stm::addState(State *s)
{
    connect(s, &State::sendEvent, this, &State::onEvent, Qt::QueuedConnection);
    mStates.insert(s);
}

void Stm::setInitialState(State *s)
{
    mInitialState = s;
}

QSet<State *> &Stm::currentStates()
{
    return mCurrentStates;
}

void Stm::setCurrentStates(const QSet<State *> &newCurrentStates)
{
    mCurrentStates = newCurrentStates;
}

const QSet<State *> &Stm::states() const
{
    return mStates;
}

void Stm::setStates(const QSet<State *> &newStates)
{
    mStates = newStates;
}

#include "state.h"

#include <QByteArrayView>
#include <stdlib.h>

#include "event.h"
#include "transition.h"

State::State(QString name) : mCheckSum(QByteArrayView(name.toLocal8Bit().data(), name.length()).toUInt())
{
    setObjectName("[" + name + "]");
    mCheckSum += (rand() %0xFFFF);
}

State::State(QString name, QVariantMap params) : mParams(params), mCheckSum(QByteArrayView(name.toLocal8Bit().data(), name.length()).toUInt())
{
    setObjectName("[" + name + "]");
    mCheckSum += (rand() %0xFFFF);
}

bool State::onEvent(Event)
{
    // States doesn't handle events, the Stms do
    return false;
}

const QSet<Transition *> &State::transitions() const
{
    return mTransitions;
}

void State::setTransitions(const QSet<Transition *> &newTransitions)
{
    mTransitions = newTransitions;
}

bool State::testCheckSum(uint16_t checkSum)
{
    if (checkSum == 0xFFFF)
    {
        return true;
    }

    if (checkSum != mCheckSum )
    {
        return false;
    }

    if (mCheckCounter > 0) mCheckCounter--;

    return (mCheckCounter == 0);
}

uint16_t State::checkSum()
{
    if (mCheckCounter == 0xFF) mCheckCounter = 0;

    mCheckCounter++;
    return mCheckSum;
}

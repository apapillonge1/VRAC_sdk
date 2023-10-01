#include "strategymanager.h"
#include "gamestate.h"

StrategyManager::StrategyManager(QObject *parent) : QThread(parent)
{
    mGameTimer.setInterval(std::chrono::seconds(100));
    mGoBackHomeTimer.setInterval(std::chrono::seconds(90));
    mFunnyActionTimer.setInterval(std::chrono::seconds(98));

    mGameTimer.setSingleShot(true);
    mGoBackHomeTimer.setSingleShot(true);
    mFunnyActionTimer.setSingleShot(true);

    connect(&mGameTimer, &QTimer::timeout, this, &StrategyManager::end);
    connect(&mGoBackHomeTimer, &QTimer::timeout, this, &StrategyManager::goBackHome);
    connect(&mFunnyActionTimer, &QTimer::timeout, this, &StrategyManager::doFunnyAction);
}

void StrategyManager::setFunnyActionTimer(int seconds)
{
    mFunnyActionTimer.setInterval(std::chrono::seconds(seconds));
}

void StrategyManager::setGoBackHomeTimer(int seconds)
{
    mGoBackHomeTimer.setInterval(std::chrono::seconds(seconds));
}

void StrategyManager::reset()
{
    mGameTimer.stop();
    mGoBackHomeTimer.stop();
    mFunnyActionTimer.stop();
}

void StrategyManager::jackPulled()
{
    mGameTimer.start();
    mGoBackHomeTimer.start();
    mFunnyActionTimer.start();
    GameState::get()->setState("hasJack", false);
}

bool StrategyManager::gameEnded() const
{
    return mGameEnded;
}

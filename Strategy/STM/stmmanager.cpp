#include "stmmanager.h"

StmManager::StmManager(QObject *parent) : StrategyManager(parent)
{

}

void StmManager::run()
{
    if (mStm == nullptr) return;

    Event e("NoEvent");
    mStm->onEntry(e);
    mStm->onEvent(e);

    exec();
}

Stm *StmManager::stm() const
{
    return mStm;
}

void StmManager::setStm(Stm *newStm)
{
    mStm = newStm;
}

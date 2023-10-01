#ifndef STMMANAGER_H
#define STMMANAGER_H

#include "../strategymanager.h"
#include "stm.h"

class StmManager : public StrategyManager
{
public:
    explicit StmManager(QObject *parent = nullptr);

    virtual void run() override;

    Stm *stm() const;
    void setStm(Stm *newStm);

private:
    Stm* mStm = nullptr;
};

#endif // STMMANAGER_H

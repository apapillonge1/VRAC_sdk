#ifndef STRATEGYMANAGER_H
#define STRATEGYMANAGER_H

#include <QObject>
#include <QTimer>
#include <QThread>

class StrategyManager : public QThread
{
    Q_OBJECT
public:
    StrategyManager(QObject *parent = nullptr);

    void setFunnyActionTimer(int seconds);
    void setGoBackHomeTimer(int seconds);
    void reset();

    virtual void run() override = 0;
    virtual void jackPulled();

    bool gameEnded() const;

signals:
    void doFunnyAction();
    void goBackHome();
    void end();

private:
    QTimer mGameTimer;
    QTimer mGoBackHomeTimer;
    QTimer mFunnyActionTimer;

    bool mGameEnded;
};

#endif // STRATEGYMANAGER_H

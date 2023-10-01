#ifndef AVOIDANCEMANAGER_H
#define AVOIDANCEMANAGER_H

#include <QObject>
#include <QThread>
#include <QTimer>

#include "Utils/Singleton.h"
#include "pathplanner.h"
#include "pathstep.h"

class IMotionController;

class AvoidanceManager : public QObject, public Singleton<AvoidanceManager>
{
    Q_OBJECT
    friend class Singleton<AvoidanceManager>;
    public :

    void setDelay(int ms); // Delay before calculating a newPath
    void setBlockedDelay(int ms); // Delay before sending the blocked signal when no path is found
    void setHoloMode(bool mode);

    void setIgnoreStaticObstacle(bool newIgnoreStaticObstacle);
    void setMotionController(IMotionController *newMotionController);
    void setCollisionHysteresis(int newCollisionHysteresis);

signals :
    void blocked(); // Emitted when no path is found and blocked delay expired

public slots:
    bool moveTo(const PathStep&, double thetaEnd); // Invoke this method to move the robot using a cubic bezier path
    bool moveTo(QPointF goal, double thetaEnd); // Invoke this method to move the robot to a specific pos
    bool doLine(int dist);
    bool rotate(int thetaEnd);

private slots:
    void update();
    void onMoveFinished();
    void findNewPath();

private :
    AvoidanceManager();

    QTimer mDelayTimer;
    QTimer mBlockedTimer;
    QTimer mLoopTimer;

    bool mIgnoreStaticObstacle = false;
    int mCollisionHysteresis = 2;
    int mCollisionRadius = 1000;
    int mCollisionCount = 0;
    int mHoloMode = false;

    IMotionController * mMotionController = nullptr; //object used to send commands to the motor board
    double mThetaEnd = 0;
};

#endif // AVOIDANCEMANAGER_H

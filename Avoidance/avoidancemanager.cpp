#include "avoidancemanager.h"

#include "pathchecker.h"
#include "gamestate.h"
#include "motioncontroller.h"

AvoidanceManager::AvoidanceManager()
{
    connect(&mDelayTimer, &QTimer::timeout, this, &AvoidanceManager::findNewPath);
    connect(&mBlockedTimer, &QTimer::timeout, this, &AvoidanceManager::blocked);
    connect(&mLoopTimer, &QTimer::timeout, this, &AvoidanceManager::update);

    mDelayTimer.setSingleShot(true);
    mBlockedTimer.setSingleShot(true);

    mLoopTimer.start(100);
}

void AvoidanceManager::update()
{
    GameState::get()->playground().updatePath();

    if (mHoloMode) {
        findNewPath();
        return;
    }

    PathStatus sts = PathChecker::checkPath(GameState::get()->playground().path(), mIgnoreStaticObstacle);

    if (sts == PathStatus::INVALID) {
        mCollisionCount++;

        if (mCollisionCount > mCollisionHysteresis) {
            if (mMotionController != nullptr) mMotionController->emergencyStop();

            if (!mDelayTimer.isActive()) mDelayTimer.start(); // Calc new Path at timeout
        }
        return;
    }

    mCollisionCount = 0;
    if (mDelayTimer.isActive()) mDelayTimer.stop();

    if (sts == PathStatus::DANGEROUS && mMotionController != nullptr) {
        mMotionController->setSpeed(60);
    }
    else if (mMotionController != nullptr){
        mMotionController->setSpeed(100);
    }
}

void AvoidanceManager::findNewPath()
{
    QVector<PathStep> newPath = PathPlanner::findPath(GameState::get()->playground().path());

    if (newPath.isEmpty()) {
        if (!mBlockedTimer.isActive()) mBlockedTimer.start();
        return;
    }

    if (mBlockedTimer.isActive()) mBlockedTimer.stop();

    GameState::get()->playground().setPath(newPath);
    if (mMotionController != nullptr) mMotionController->moveTo(newPath.front());
}

void AvoidanceManager::onMoveFinished()
{
    GameState::get()->playground().popPathStep();
    if (mMotionController != nullptr) mMotionController->moveTo(GameState::get()->playground().path().front());
}

bool AvoidanceManager::moveTo(QPointF goal, double thetaEnd)
{
    return moveTo(PathStep(GameState::get()->robot().pos(), goal), thetaEnd);
}

bool AvoidanceManager::doLine(int )
{
    return true;
}

bool AvoidanceManager::rotate(int theta)
{
    mThetaEnd = theta;
    if (mMotionController != nullptr) mMotionController->rotate(mThetaEnd);

    return true;
}

bool AvoidanceManager::moveTo(const PathStep& path, double thetaEnd)
{
    QVector<PathStep> newPath {path};

    if (PathChecker::checkPath(path, mIgnoreStaticObstacle) == PathStatus::INVALID) {
        newPath = PathPlanner::findPath(path);
    }

    if (newPath.isEmpty()) {
        qDebug() << "No path found for this move";
        return false;
    }
    GameState::get()->playground().setPath(newPath);

    mThetaEnd = thetaEnd;
    if (mMotionController != nullptr) mMotionController->moveTo(newPath.front());

    return true;
}

void AvoidanceManager::setIgnoreStaticObstacle(bool newIgnoreStaticObstacle)
{
    mIgnoreStaticObstacle = newIgnoreStaticObstacle;
}

void AvoidanceManager::setDelay(int ms)
{
    mDelayTimer.setInterval(ms);
}

void AvoidanceManager::setBlockedDelay(int ms)
{
    mBlockedTimer.setInterval(ms);
}

void AvoidanceManager::setHoloMode(bool mode)
{
    mHoloMode = mode;
}

void AvoidanceManager::setCollisionHysteresis(int newCollisionHysteresis)
{
    mCollisionHysteresis = newCollisionHysteresis;
}

void AvoidanceManager::setMotionController(IMotionController *newMotionController)
{
    mMotionController = newMotionController;
    connect(mMotionController, &IMotionController::moveFinished, this, &AvoidanceManager::onMoveFinished);
}

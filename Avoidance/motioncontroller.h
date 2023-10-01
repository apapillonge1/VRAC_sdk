#ifndef MOTIONCONTROLLER_H
#define MOTIONCONTROLLER_H

#include <QObject>
#include <QPointF>

#include "pathstep.h"

class IMotionController : public QObject
{
    Q_OBJECT

    public :
        IMotionController();

        virtual void moveTo(QPointF pos, double theta) = 0;
        virtual void moveTo(const PathStep& p) = 0;
        virtual void line(int dist) = 0;
        virtual void rotate(double theta) = 0;

        virtual void emergencyStop() = 0;

        virtual void setSpeed(int speed) = 0;
        virtual void setAccel(int accel) = 0;
        virtual void setDeccel(int deccel) = 0;

        virtual void setRotateSpeed(int speed) = 0;
        virtual void setRotateAccel(int accel) = 0;
        virtual void setRotateDeccel(int deccel) = 0;

    signals :
        void moveFinished();

    public slots:

        void onMoveAckReceived(QString ack, int checksum = 0xFFFF)
        {
            if (mACKWanted == ack && (mChecksum == checksum || checksum == 0xFFFF)) {
                moveFinished();
            }
        }

    private:
        QString mACKWanted;
        int mChecksum;
};
#endif // MOTIONCONTROLLER_H

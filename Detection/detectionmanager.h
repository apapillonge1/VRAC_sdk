#ifndef DETECTIONMANAGER_H
#define DETECTIONMANAGER_H

#include <QThread>
#include <QObject>
#include <QTimer>

#include "Utils/Singleton.h"
#include "sensor.h"

class DetectionManager : public QThread, public Singleton<DetectionManager>
{
    Q_OBJECT
    friend class Singleton<DetectionManager>;
public :
    void addBodySensor(ISensor *sensor) { mBodySensors.append(sensor); }
    void addBeaconSensor(ISensor *sensor) { mBeaconSensors.append(sensor); }

    virtual void run() override;

    static void addToGroup(QPointF point, QVector<QPolygonF>& groups);

private slots:
    void update();

private:
    explicit DetectionManager(QObject *parent = nullptr);
    ~DetectionManager();
    QTimer mLoopTicker;
    QVector<ISensor*> mBeaconSensors;
    QVector<ISensor*> mBodySensors;
};

#endif // DETECTIONMANAGER_H

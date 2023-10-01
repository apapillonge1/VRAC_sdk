#include "detectionmanager.h"

#include <QVariant>
#include <QPointF>
#include <QPolygonF>
#include <QLineF>

#include "Scenes/obstacle.h"
#include "gamestate.h"

DetectionManager::DetectionManager(QObject *parent) : QThread(parent)
{
    connect(&mLoopTicker, &QTimer::timeout, this, &DetectionManager::update);
}

DetectionManager::~DetectionManager()
{
    for (auto sensor : mBodySensors) {
        delete sensor;
    }
    mBodySensors.clear();

    for (auto sensor : mBeaconSensors) {
        delete sensor;
    }
    mBeaconSensors.clear();
}

void DetectionManager::run()
{
    mLoopTicker.start(100);
    exec();
}

void DetectionManager::addToGroup(QPointF point, QVector<QPolygonF> &groups)
{
    bool groupDetected = false;
    if(!groups.isEmpty())
    {
        for(auto& group : groups)
        {
            QLineF dist = QLineF(group.boundingRect().center(), point);
            if(dist.length() < 50)
            {
                group.append(point);
                groupDetected = true;
            }
        }
    }

    if(!groupDetected)
    {
        QPolygonF poly;
        poly.append(point);
        groups.append(std::move(poly));
    }
}

void DetectionManager::update()
{
    QVector<QPolygonF>groupedBeaconPoints;
    QVector<QPolygonF>groupedBodyPoints;

    for (auto &sensor : mBeaconSensors) {
        for (const auto & var : sensor->read()) {
            QPointF point = var.toPointF();
            addToGroup(point, groupedBeaconPoints);
        }
    }

    for (auto &sensor : mBodySensors) {
        for (const auto & var : sensor->read()) {
            QPointF point = var.toPointF();
            addToGroup(point, groupedBodyPoints);
        }
    }

    // now we have to pair bodypoints and beacon points
    QVector<Obstacle> ennemies;

    for (auto & beaconGroup : groupedBeaconPoints) {
        qreal minDist = 100; // to be determined
        QPolygonF ennemyBody;
        for (auto & bodyGroup : groupedBodyPoints) {
            qreal dist = QLineF(beaconGroup.boundingRect().center(), bodyGroup.boundingRect().center()).length();

            if (dist < minDist) {
                ennemyBody.append(bodyGroup);
            }
        }

        ennemies.emplaceBack(beaconGroup.boundingRect().center(), GameState::get()->robot().boundingRect().width(), ennemyBody);
    }

    GameState::get()->playground().setObstacles(ennemies);
}

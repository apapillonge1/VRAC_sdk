#ifndef SIMULATIONSOCKET_H
#define SIMULATIONSOCKET_H

#include "subscriber.h"
#include "QTcpSocket"

class SimulationSocket : public Subscriber, public QTcpSocket
{
public:
    SimulationSocket(QVariantMap simulationCharacteristics);

    void onMessage(int id, const QByteArray data) override;

signals:

public slots:
    void readyRead();

private:

};

#endif // SIMULATIONSOCKET_H

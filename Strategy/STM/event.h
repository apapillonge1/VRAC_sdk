#ifndef EVENT_H
#define EVENT_H

#include <QString>
#include <QObject>

struct Event
{
    Event(const QString &val, uint16_t checkSum = 0xFFFF);

    QString m_value;
    uint16_t mCheckSum;
};

#endif // EVENT_H

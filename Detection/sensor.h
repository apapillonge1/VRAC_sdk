#ifndef ISENSOR_H
#define ISENSOR_H

#include <QObject>

class ISensor : public QObject
{
    Q_OBJECT
public:
    explicit ISensor(QObject *parent = nullptr);

    QVariantList read();

signals:

};

#endif // ISENSOR_H

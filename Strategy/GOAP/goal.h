#ifndef GOAL_H
#define GOAL_H

#include <QString>
#include <QMap>
#include <QVariant>

class Goal{

public :
    Goal() {}
    Goal(QString name) {mName = name;}
    virtual ~Goal() {}

    const QString &name() const { return mName; }

    virtual bool isValid() const { return true; }
    virtual int priority() const { return 1; }

    void setDesiredState(const QVariantMap &newDesiredState) { mDesiredState = newDesiredState; }
    void setDesiredState(QString key, QVariant value) { mDesiredState[key] = value; }
    QVariantMap desiredState() { return mDesiredState; }

private:
    QVariantMap mDesiredState;
    QString mName;
};

#endif

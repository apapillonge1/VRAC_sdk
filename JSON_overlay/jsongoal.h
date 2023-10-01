#ifndef JSONGOAL_H
#define JSONGOAL_H

#include "../Strategy/GOAP/goal.h"

#include <QJsonObject>
#include <QJsonArray>

class JsonGoal : public Goal
{
public:
    JsonGoal(QString filename, QString dir);

    virtual bool isValid() const override;
    virtual int priority() const override;

private:
    QJsonObject mValid;
    QJsonArray mPrio;
};

#endif // JSONGOAL_H

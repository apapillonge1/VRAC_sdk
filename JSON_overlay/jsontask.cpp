#include "jsontask.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

#include "jsonstatemachine.h"
#include "../Strategy/STM/event.h"
#include "../Strategy/STM/stm.h"
#include "../Strategy/GOAP/task.h"
#include "gamestate.h"



JsonTask::JsonTask(QString fileName, QString dir) : ITask(fileName)
{
    QString path = dir + fileName + ".json";
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    file.close();

    QJsonObject json = QJsonDocument::fromJson(val.toUtf8()).object();

    QJsonObject preconditions = json["preconditions"].toObject();
    QJsonObject effects = json["effects"].toObject();
    mCost = json["cost"].toObject();
    mValid = json["validity"].toObject();

    setPreconditions(preconditions.toVariantMap());
    setEffects(effects.toVariantMap());
    mStm = new JsonStateMachine(json["stmFile"].toString(), "/Users/juliensolbach/Documents/Workspace/VRAC/TestSDK/STMs/");

    connect(mStm, &Stm::finished, this, &ITask::finished);
}

JsonTask::~JsonTask()
{
    delete mStm;
}

void JsonTask::run()
{
    qDebug() << "Starting TASK :" << objectName();
    if (mStm == nullptr) finished();

    Event e("NoEvent");
    mStm->onEntry(e);
    mStm->onEvent(e);
}

int JsonTask::cost(const QVariantMap &) const
{
    if (mCost["distToElem"].isString()) {
        //calculate the path to object entry point and its cost
    }

    return 10;
}

bool JsonTask::isValid() const
{
    if (mValid["gameElement"].isObject()) {
        QJsonObject gameElements = mValid["gameElement"].toObject();
        for (auto &key : gameElements.keys())
        {
            if((GameState::get()->playground().getElementGroup(key).isEmpty() ^ gameElements[key].toBool()) == false)
                return false;
        }
    }

    if (mValid["gameState"].isObject())
    {
        QJsonObject state = mValid["gameState"].toObject();
        for (auto &key : state.keys())
        {
            if (!GameState::get()->states().contains(key) || GameState::get()->states()[key] != state[key].toVariant())
            {
                return false;
            }
        }
    }
    return true;
}

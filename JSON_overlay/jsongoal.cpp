#include "jsongoal.h"

#include <QFile>
#include <QJsonDocument>

#include "gamestate.h"

JsonGoal::JsonGoal(QString filename, QString dir) : Goal(filename)
{
    QString path = dir + filename + ".json";
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    file.close();

    QJsonObject json = QJsonDocument::fromJson(val.toUtf8()).object();

    QJsonObject desiredState = json["desiredState"].toObject();
    mValid = json["validity"].toObject();
    mPrio = json["priority"].toArray();

    setDesiredState(desiredState.toVariantMap());
}

bool JsonGoal::isValid() const
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

int JsonGoal::priority() const
{
    for (auto & ref : mPrio) {

        bool prioValid = true;
        QJsonObject prio = ref.toObject();

        if (prio["gameState"].isObject()) {
            QJsonObject state = prio["gameState"].toObject();
            for (auto &key : state.keys())
            {
                if (!GameState::get()->states().contains(key) || GameState::get()->states()[key] != state[key].toVariant())
                {
                    prioValid = false;
                    break;
                }
            }
        }

        if (prioValid)
            return prio["prio"].toInt();
    }

    return 0;
}

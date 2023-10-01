#include "jsonstatemachine.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "../Strategy/STM/transition.h"
#include "../Strategy/Actions/actionfactory.h"

JsonStateMachine::JsonStateMachine(QString filename, QString dir) : Stm("[" + filename + "]")
{
    QString path = dir + filename + ".json";
    QFile file(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    file.close();

    QJsonObject json = QJsonDocument::fromJson(val.toUtf8()).object();

    QMap<QString, State*> stateMap;

    bool first = true;

    /// SETTING UP STATES
    auto const actions = json["actions"].toArray();
    for(auto itAction : actions)
    {
        QJsonObject action = itAction.toObject();
        State * state;

        if (action["file"].isString())
        {
            state = new JsonStateMachine(action["file"].toString(), dir);
        }
        else
        {
            QString actionTag = action["tag"].toString();
            state = ActionFactory::get()->getAction(actionTag, action.toVariantMap());
        }

        if (first)
        {
            setInitialState(state);
            first = false;
        }
        addState(state);
        stateMap[action["tag"].toString()] = state;
    }

    for(auto itAction : actions)
    {
        QJsonObject action = itAction.toObject();

        State *currentState = stateMap[action["tag"].toString()];
        QJsonArray transitions = action["transitions"].toArray();

        QMap<QString, Transition*> transitionMap;
        for(auto transition : transitions)
        {
            State *nextState = stateMap[transition.toObject()["destination"].toString()];
            QString event = transition.toObject()["type"].toString();

            Transition *transi;
            if ((transi = transitionMap[event]) == nullptr)
            {
                transi = new Transition(event);
                transitionMap[event] = transi;
                transi->setSourceState(currentState);
            }

            transi->addTargetState(nextState);
            currentState->addTransition(transi);
        }
    }
}

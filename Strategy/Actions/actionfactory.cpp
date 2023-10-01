#include "actionfactory.h"

#include "basicactions.h"

ActionFactory::ActionFactory()
{
    mMap.insert("Line",     [](QString Tag, QVariantMap params){return new Line(Tag, params);});
    mMap.insert("Rotate",   [](QString Tag, QVariantMap params){return new Rotate(Tag, params);});

    mMap.insert("TakeGameElement",   [](QString Tag, QVariantMap params){return new TakeGameElement(Tag, params);});
    mMap.insert("SetGameState",   [](QString Tag, QVariantMap params){return new SetGameState(Tag, params);});
}

State *ActionFactory::getAction(QString tag, QVariantMap action)
{
    auto actionIt = mMap.find(action["action"].toString());

    if (actionIt == mMap.end())
    {
        //DEBUG() << "This action doesn't exist !!!" << action["action"].toString();
        return nullptr;
    }

    QVariantMap params  = action["parameters"].toMap();

    return actionIt.value()(tag, params);
}

void ActionFactory::addAction(QString tag, std::function<State *(QString, QVariantMap)>func)
{
    mMap.insert(tag,func);
}

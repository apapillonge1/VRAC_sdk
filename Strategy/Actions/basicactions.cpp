#include "basicactions.h"

#include "gamestate.h"
#include "Avoidance/avoidancemanager.h"
#include "Detection/detectionutilities.h"

void Line::onEntry(Event&)
{
    int dist = mParams["dist"].toInt();
    AvoidanceManager::get()->doLine(dist);
}

void Line::onExit(Event&)
{
}

void Rotate::onEntry(Event&)
{
    int theta = mParams["theta"].toInt();
    AvoidanceManager::get()->rotate(theta);
}

void Rotate::onExit(Event&)
{
}

void SetGameState::onEntry(Event &)
{
    QString state = mParams["state"].toMap()["selected"].toString();
    QVariant value = mParams["value"];
    GameState::get()->setState(state, value);
}

void SetGameState::onExit(Event &)
{

}

void TakeGameElement::onEntry(Event &)
{
    QString elementGroup = mParams["group"].toMap()["selected"].toString();
    QString tag = mParams["tag"].toMap()["selected"].toString();

    GameElement *element = GameState::get()->playground().popElement(elementGroup, tag);

    if (element != nullptr) {
     // add to inventory ?
        delete element;
    }
}

void TakeGameElement::onExit(Event &)
{

}

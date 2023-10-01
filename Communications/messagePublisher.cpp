#include "messagePublisher.h"

#include <QDebug>

#include "subscriber.h"

MessagePublisher::MessagePublisher()
{

}

MessagePublisher::~MessagePublisher()
{
    for (auto dump : mDumpSubscribers) {
        if (dump != nullptr) delete dump;
    }

    for (auto sub : mSubscribers) {
        if (sub != nullptr) delete sub;
    }
}

void MessagePublisher::subscribe(Subscriber *newSub)
{
    mSubscribers.append(newSub);
}

void MessagePublisher::sendMessage(const QString targetNode, int id, const QByteArray data)
{
    qDebug() << "Test message";
    for (auto dump : mDumpSubscribers) {
        dump->onMessage(id, data);
    }

    for (auto sub : mSubscribers) {
        if (!sub->nodes().contains(targetNode)) continue;
        sub->onMessage(id, data);
    }
}

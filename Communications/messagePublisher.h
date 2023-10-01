#ifndef MESSAGEPUBLISHER_H
#define MESSAGEPUBLISHER_H

#include <QObject>
#include <QMap>

#include "Utils/Singleton.h"

class Subscriber;

class MessagePublisher: public QObject, public Singleton<MessagePublisher>
{
    Q_OBJECT
    friend class Singleton<MessagePublisher>;
    public :
    MessagePublisher();
    ~MessagePublisher();

    void subscribe(Subscriber *newSub);
    void subscribeAsDump(Subscriber *dump) {mDumpSubscribers.append(dump);}
    void sendMessage(const QString targetNode, int id, const QByteArray data = QByteArray());

private:
    QVector<Subscriber*> mSubscribers;
    QVector<Subscriber*> mDumpSubscribers;
};

#define sendMessageTo MessagePublisher::get()->sendMessage

#endif // MESSAGEPUBLISHER_H

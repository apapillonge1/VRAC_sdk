#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <QObject>

class Subscriber : public QObject {
    Q_OBJECT
public:
    Subscriber(QStringList nodes = QStringList()) : mNodes(nodes) {}

    const QStringList &nodes() const {return mNodes;}
    void addNode(QString node) {mNodes.append(node);}
    void removeNode(QString node) { mNodes.removeOne(node);}

public slots:
    virtual void onMessage(int id, const QByteArray data) = 0;

private:
    QStringList mNodes;
};



#endif // SUBSCRIBER_H

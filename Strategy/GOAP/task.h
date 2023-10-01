#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QJsonObject>

class ITask : public QObject
{
    Q_OBJECT
public:
    ITask(QString name, QObject* parent = nullptr) : QObject(parent) {setObjectName(name);}
    virtual ~ITask() {}

    const QString &name() const { return mName; }

    void addPrecondition(QString key, QVariant value) { mPreconditions[key] = value;}
    void setPreconditions(const QVariantMap& newPreconditions) { mPreconditions = newPreconditions; }
    const QVariantMap& preconditions() const { return mPreconditions;};

    void addEffect(QString key, QVariant value) { mEffects[key] = value;}
    void setEffects(const QVariantMap& newEffects) { mEffects = newEffects; }
    const QVariantMap& effects() const {return mEffects;};

    virtual void run() = 0;
    virtual int cost(const QVariantMap&) const { return 1000;}
    virtual bool isValid() const { return true; }

signals:
    void finished();

private:
    QVariantMap mPreconditions;
    QVariantMap mEffects;
    QString mName;
};
#endif // TASK_H

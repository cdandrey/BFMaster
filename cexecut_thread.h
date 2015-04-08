#ifndef CEXECUT_THREAD_H
#define CEXECUT_THREAD_H

#include <QThread>
#include <QQueue>

class CExecutObject;

class CExecutThread : public QThread
{
    Q_OBJECT

public:

    typedef QQueue<CExecutObject*> QueueExeObject;

    explicit CExecutThread(QObject *parent = 0);
    CExecutThread(QObject *parent,const QueueExeObject &queue);
    CExecutThread(QObject *parent,CExecutObject *obj);
    ~CExecutThread();

    QString progress() const;

    void reset(const QueueExeObject &queue);

    void run();

public slots:

    void on_terminated();

signals:

    void executOperation(const QString&);
    void message(const QString&);
    void successfull();

private:

    QueueExeObject m_queue;
};


#endif // CEXECUT_THREAD_H

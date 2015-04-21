#ifndef CEXECUT_THREAD_H
#define CEXECUT_THREAD_H

#include <QThread>

class CExecutObject;

class CExecutThread : public QThread
{
    Q_OBJECT

public:

    //explicit CExecutThread(QObject *parent = 0);
    explicit CExecutThread(QObject *parent = 0,
                           const QList<CExecutObject*> &lstObj = QList<CExecutObject *>())
        : QThread(parent),m_lstObj(lstObj)
    {}

    QString progress() const;

    void run();

private slots:

    void on_terminated();

private:

    QList<CExecutObject*> m_lstObj;
signals:

    void executOperation(const QString&);
    void message(const QString&);
    void timerWork(uint);

};


#endif // CEXECUT_THREAD_H

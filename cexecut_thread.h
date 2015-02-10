#ifndef CEXECUT_THREAD_H
#define CEXECUT_THREAD_H

#include <QThread>

class CExecutObject;

class CExecutThread : public QThread
{
    Q_OBJECT

public:

    explicit CExecutThread(QObject *parent = 0,CExecutObject *obj = 0);
    ~CExecutThread();

    void reset(CExecutObject *obj);

    void run();

public slots:

    void on_terminated();

signals:

    void message(const QString&);
    void successfull();

private:

    CExecutObject *m_obj;
};


#endif // CEXECUT_THREAD_H

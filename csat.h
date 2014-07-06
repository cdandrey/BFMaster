#ifndef CSAT_H
#define CSAT_H

#include <QThread>
#include "typedef.h"

QT_BEGIN_NAMESPACE
class CBoolFunction;
class CBFView;
class CSat;
QT_END_NAMESPACE


class CSatCreater
{
public:

    explicit CSatCreater(const TStr& name):
        m_satClass(name){}

    CSat *create(CBFView *bfv);

private:

    TStr m_satClass;
};
//------------------------------------------------------------


class CSat : public QThread
{
    Q_OBJECT
public:

    explicit CSat(CBFView *bfv,QObject *parent = 0) :
        QThread(parent),m_bfv(bfv),m_satdata(),m_stopped(false){}

    virtual TStr progress() const = 0;
    virtual TStr progressFinished() const = 0;
    virtual TStr progressDescription() const = 0;

public slots:

    void on_stopped(){m_stopped = true;}

protected:

    inline void multiply(int ibegin,CBoolFunction &f,  TLst &seq,int &numDis);

    CBFView *m_bfv;
    TSatData m_satdata;
    volatile bool m_stopped;

private:

    void run() = 0;

signals:

    void successful(CBFView*);
    void terminated();
};
//------------------------------------------------------------


class CSatExhaustive : public CSat
{
    Q_OBJECT
public:
    explicit CSatExhaustive(CBFView *bfv,QThread *parent = 0);

    TStr progress() const;
    TStr progressFinished() const;
    TStr progressDescription() const;

    static TStr description(){return QObject::tr("Метод полного перебора");}
    static TStr name(){return "SatExhaustive";}

private:

    void run();
    void determine(CBoolFunction f,TLst seq, int numDis);

    TLst m_branch;    // solver branch
};
//------------------------------------------------------------


class CSatRandomClauses : public CSat
{
    Q_OBJECT
public:
    explicit CSatRandomClauses(CBFView *bfv,QThread *parent = 0);

    TStr progress() const;
    TStr progressFinished() const;
    TStr progressDescription() const;

    static TStr description(){return QObject::tr("Метод перебора случайных дизьюнктов");}
    static TStr name(){return "SatRandomClauses";}

private:

    void run();
    void determine(CBoolFunction f,TLst seq,int iDis,int numDis);

    TLst m_branch;    // solver branch
};


class CSatMinimumClauses : public CSat
{
    Q_OBJECT
public:
    explicit CSatMinimumClauses(CBFView *bfv,QThread *parent = 0);

    TStr progress() const;
    TStr progressFinished() const;
    TStr progressDescription() const;

    static TStr description(){return QObject::tr("Метод перебора минимальных дизьюнктов");}
    static TStr name(){return "SatMinimumClauses";}

private:

    void run();
    void determine(CBoolFunction f,TLst seq,int iDis,int numDis);
    int minimum(const TLLst &dis,const TLLst &var);

    TLst m_branch;    // solver branch
};

#endif // CSAT_H

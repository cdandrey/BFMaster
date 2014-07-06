#ifndef CBFGENERATE_H
#define CBFGENERATE_H

#include <QThread>
#include <typedef.h>

QT_BEGIN_NAMESPACE
class CBFView;
QT_END_NAMESPACE

class CBFGenerate : public QThread
{
    Q_OBJECT
public:

    explicit CBFGenerate(CBFView *bfv,QObject *parent = 0) :
        QThread(parent),m_bfv(bfv),m_stopped(false){

        m_createDis = 0;
        m_numDisMax = 0;
        m_combinations = 0;
        m_createCombinations = 0;
    }

    TStr progress() const;
    TStr progressFinished() const;
    TStr progressDescription() const;

public slots:

    void on_stopped(){m_stopped = true;}

private:

    void run();

    inline bool absorb(const TLst &lessDecDis,
                       const TByt &lessBitDis,
                       const TByt &mostBitDis);

    void fillMatrixCombinations(TVVecd &c);

    CBFView *m_bfv;
    volatile bool m_stopped;

    int m_createDis;
    int m_numDisMax;
    double m_combinations;
    double m_createCombinations;

signals:

    void successfull(CBFView*);

};

#endif // CBFGENERATE_H

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

    typedef unsigned long long ull;

    explicit CBFGenerate(CBFView *bfv,QObject *parent = 0);

    QString progress() const;
    QString progressFinished() const;
    QString progressDescription() const;

public slots:

    void on_stopped(){m_stopped = true;}

private:

    void run();

    inline bool absorb(const TLst &lessDecDis,
                       const TByt &lessBitDis,
                       const TByt &mostBitDis);

    CBFView *m_bfv;
    volatile bool m_stopped;

    int m_createDis;
    int m_numDisMax;
    ull m_combinations;
    ull m_createCombinations;

signals:

    void successfull(CBFView*);

};

#endif // CBFGENERATE_H

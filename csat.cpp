#include "csat.h"
#include "cboolfunction.h"

#include <QTime>
#include <QElapsedTimer>

///
/// \class CSatCreater
///

CSat *CSatCreater::create(CBFView *bfv)
{
    if (m_satClass == CSatExhaustive::name())
        return new CSatExhaustive(bfv);
    else if (m_satClass == CSatRandomClauses::name())
        return new CSatRandomClauses(bfv);
    else if (m_satClass == CSatMinimumClauses::name())
        return new CSatMinimumClauses(bfv);
    else
        return NULL;
}
//-----------------------------------------------------------------------


/// \class CSat /////////////////////////////////////////////////////////


inline void CSat::multiply(int ibegin, CBoolFunction &f, TLst &seq, int &numDis)
{
    for (int i = ibegin; i < seq.size(); ++i){

        m_satdata.quantity += f.multiply(seq.at(i),numDis,m_satdata.state,seq);

        ++m_satdata.quantity;

        if(m_satdata.state == Satisfiable){
            m_satdata.sequence = seq;
            return;
        } else if (m_satdata.state == Unsatisfiable){
           return;
        }
    }
}
//-----------------------------------------------------------------------


/// \class CSatExhaustive ///////////////////////////////////////////////


CSatExhaustive::CSatExhaustive(CBFView *bfv, QThread *parent) :
    CSat(bfv,parent)
{
    m_satdata.description = description();
    m_branch = TLst();
}
//-----------------------------------------------------------------------


TStr CSatExhaustive::progress() const
{
    if (this->isFinished())
        return "";

    TStr str = tr("текущая ветка выполнения: ");

//    foreach(int val,m_branch)
//        str += TStr("%1.").arg(m_bfv->boolFunction()->varDIMACS(val));

    return str;
}
//-----------------------------------------------------------------------


TStr CSatExhaustive::progressFinished() const
{
//    if (m_stopped) {

//        return tr("\nВыполнимость функции %1 - не определена. Процесс был остановлен.\n")
//                .arg(m_bfv->boolFunctionName());

//    } else {

//        return tr("\n%1 завершил работу.\n%2 - %3.\n")
//                .arg(description())
//                .arg(m_bfv->boolFunctionName())
//                .arg(m_satdata.state);
//    }
    return "";
}
//-----------------------------------------------------------------------


TStr CSatExhaustive::progressDescription() const
{
//    return TStr("%1 %2 : %3")
//            .arg(tr("Определяется выполнимость "))
//            .arg(m_bfv->boolFunctionName())
//            .arg(description());
    return "";
}
//-----------------------------------------------------------------------


void CSatExhaustive::run()
{
    QElapsedTimer time;
    time.start();

    //determine(*m_bfv->boolFunction(),TLst(),m_bfv->boolFunction()->numDis());

    if (m_stopped) {
        emit terminated();
        return;
    }

    m_satdata.time = time.nsecsElapsed();

    //m_bfv->boolFunction()->addSatdata(name(),new TSatData(m_satdata));
    emit successful(m_bfv);
}
//-----------------------------------------------------------------------


void CSatExhaustive::determine(CBoolFunction f, TLst seq, int numDis)
{
    m_branch << 0;

    for (int x = 0; x < f.numXZ(); ++x) {

        if (m_stopped)
            return;

        ++m_satdata.quantity;
        if (f.var().at(x).size() == 0)
            continue;

        m_branch[m_branch.length() - 1] = x;

        // set new data for new solver brunch
        CBoolFunction xF = f;

        int xNumDis = numDis;

        m_satdata.state = Undefine;

        TLst xSeq = seq;
        xSeq << x;

        // multiplier function on xSeq
        multiply(seq.size(),xF,xSeq,xNumDis);

        ++m_satdata.quantity;
        if (m_satdata.state == Undefine)
            determine(xF,xSeq,xNumDis);

        if (m_satdata.state == Satisfiable)
            return;
    }

    m_satdata.state = Unsatisfiable;
    m_branch.removeLast();
}
//-----------------------------------------------------------------------


/// \class CSatRandomClauses ////////////////////////////////////////////


CSatRandomClauses::CSatRandomClauses(CBFView *bfv, QThread *parent) :
    CSat(bfv,parent)
{
    m_satdata.description = description();
    m_branch = TLst();
}
//-----------------------------------------------------------------------


TStr CSatRandomClauses::progress() const
{
    if (this->isFinished())
        return "";

    TStr str = tr("текущая ветка выполнения: ");

    for (int i = 0; i < m_branch.size(); i += 3) {
        str += TStr("%1(%2).")
                .arg(m_branch.at(i))
                .arg(m_branch.at(i + 1) - m_branch.at(i + 2));
    }

    return str;
}
//-----------------------------------------------------------------------


TStr CSatRandomClauses::progressFinished() const
{
//    if (m_stopped) {

//        return tr("\nВыполнимость функции %1 - не определена. Процесс был остановлен.\n")
//                .arg(m_bfv->boolFunctionName());

//    } else {

//        return tr("\n%1 завершил работу.\n%2 - %3.\n")
//                .arg(description())
//                .arg(m_bfv->boolFunctionName())
//                .arg(m_satdata.state);
//    }
    return "";
}
//-----------------------------------------------------------------------


TStr CSatRandomClauses::progressDescription() const
{
//    return TStr("%1 %2 : %3")
//            .arg(tr("Определяется выполнимость "))
//            .arg(m_bfv->boolFunctionName())
//            .arg(description());

    return "";
}
//-----------------------------------------------------------------------


void CSatRandomClauses::run()
{
    qsrand(QTime::currentTime().msec());

    QElapsedTimer time;
    time.start();

    //int iDis = qrand() % m_bfv->boolFunction()->numDis();

    //determine(*m_bfv->boolFunction(),TLst(),iDis,m_bfv->boolFunction()->numDis());

    if (m_stopped) {
        emit terminated();
        return;
    }

    m_satdata.time = time.nsecsElapsed();

    //m_bfv->boolFunction()->addSatdata(name(),new TSatData(m_satdata));
    emit successful(m_bfv);
}
//-----------------------------------------------------------------------


void CSatRandomClauses::determine(CBoolFunction f,TLst seq,int iDis,int numDis)
{
    m_branch << iDis << f.dis().at(iDis).size() << 0;

    int i = 0;

    foreach (int x, f.dis().at(iDis)) {

        if (m_stopped)
            return;

        m_branch[m_branch.length() - 1] = i;

        // set new data for new solver brunch
        CBoolFunction xF = f;

        int xNumDis = numDis;

        m_satdata.state = Undefine;

        TLst xSeq = seq;
        xSeq << x;

        // multiplier function on xSeq
        multiply(seq.size(),xF,xSeq,xNumDis);

        ++m_satdata.quantity;

        if (m_satdata.state == Undefine) {

            int xDis = 0;

            do {

                ++m_satdata.quantity;
                xDis = qrand() % f.numDis();

            }while(xF.dis().at(xDis).size() == 0);

            determine(xF,xSeq,xDis,xNumDis);
        }

        if (m_satdata.state == Satisfiable)
            return;

        ++i;
    }

    m_satdata.state = Unsatisfiable;
    m_branch.removeLast();
    m_branch.removeLast();
    m_branch.removeLast();
}
//-----------------------------------------------------------------------


/// \class CSatMinimumClauses ///////////////////////////////////////////


CSatMinimumClauses::CSatMinimumClauses(CBFView *bfv, QThread *parent) :
    CSat(bfv,parent)
{
    m_satdata.description = description();
    m_branch = TLst();
}
//-----------------------------------------------------------------------


TStr CSatMinimumClauses::progress() const
{
    if (this->isFinished())
        return "";

    TStr str = tr("текущая ветка выполнения: ");

    for (int i = 0; i < m_branch.size(); i += 3) {
        str += TStr("%1(%2).")
                .arg(m_branch.at(i))
                .arg(m_branch.at(i + 1) - m_branch.at(i + 2));
    }

    return str;
}
//-----------------------------------------------------------------------


TStr CSatMinimumClauses::progressFinished() const
{
//    if (m_stopped) {

//        return tr("\nВыполнимость функции %1 - не определена. Процесс был остановлен.\n")
//                .arg(m_bfv->boolFunctionName());

//    } else {

//        return tr("\n%1 завершил работу.\n%2 - %3.\n")
//                .arg(description())
//                .arg(m_bfv->boolFunctionName())
//                .arg(m_satdata.state);
//    }
}
//-----------------------------------------------------------------------


TStr CSatMinimumClauses::progressDescription() const
{
//    return TStr("%1 %2 : %3")
//            .arg(tr("Определяется выполнимость "))
//            .arg(m_bfv->boolFunctionName())
//            .arg(description());
}
//-----------------------------------------------------------------------


void CSatMinimumClauses::run()
{
//    QElapsedTimer time;
//    time.start();

//    int iDis = minimum(m_bfv->boolFunction()->dis(),
//                       m_bfv->boolFunction()->var());

//    determine(*m_bfv->boolFunction(),TLst(),iDis,m_bfv->boolFunction()->numDis());

//    if (m_stopped) {
//        emit terminated();
//        return;
//    }

//    m_satdata.time = time.nsecsElapsed();

//    m_bfv->boolFunction()->addSatdata(name(),new TSatData(m_satdata));
//    emit successful(m_bfv);
}
//-----------------------------------------------------------------------


void CSatMinimumClauses::determine(CBoolFunction f,TLst seq,int iDis,int numDis)
{
    m_branch << iDis << f.dis().at(iDis).size() << 0;

    int i = 0;

    foreach (int x, f.dis().at(iDis)) {

        if (m_stopped)
            return;

        m_branch[m_branch.length() - 1] = i;

        // set new data for new solver brunch
        CBoolFunction xF = f;

        int xNumDis = numDis;

        m_satdata.state = Undefine;

        TLst xSeq = seq;
        xSeq << x;

        // multiplier function on xSeq
        multiply(seq.size(),xF,xSeq,xNumDis);

        ++m_satdata.quantity;

        if (m_satdata.state == Undefine) {

            int xDis = 0;

            do {

                ++m_satdata.quantity;
                xDis = minimum(xF.dis(),xF.var());

            }while(xF.dis().at(xDis).size() == 0);

            determine(xF,xSeq,xDis,xNumDis);
        }

        if (m_satdata.state == Satisfiable)
            return;

        ++i;
    }

    m_satdata.state = Unsatisfiable;
    m_branch.removeLast();
    m_branch.removeLast();
    m_branch.removeLast();
}
//-----------------------------------------------------------------------


int CSatMinimumClauses::minimum(const TLLst &dis,const TLLst &var)
{
    int minNum = 0;
    int minLen = 999999999;
    int minVarDegree = 0;

    int len;
    int varDegree;
    int disSize = dis.size();

    for (int num = 0; num < disSize; ++num) {

        len = dis.at(num).size();

        ++m_satdata.quantity;
        if (len == 0)
            continue;

        ++m_satdata.quantity;
        if (len < minLen) {

            minNum = num;
            minLen = len;
            minVarDegree = 0;

        } else if (len == minLen) {

            m_satdata.quantity += 2;
            if (minVarDegree == 0) {
                m_satdata.quantity += dis.at(minNum).size();
                foreach(int x,dis.at(minNum))
                    minVarDegree += var.at(x).size();
            }

            varDegree = 0;
            m_satdata.quantity += dis.at(num).size();
            foreach(int x,dis.at(num))
                varDegree += var.at(x).size();

            ++m_satdata.quantity;
            if (varDegree > minVarDegree) {
                minNum = num;
                minLen = len;
                minVarDegree = varDegree;
            }

        } else {
            ++m_satdata.quantity;
        }
    }

    return minNum;
}
//-------------------------------------------------------------------


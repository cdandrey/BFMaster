#include "cbool_formula.h"
#include "ccombinasion.h"

#include <QDebug>
#include <QSet>
#include <QTime>
#include <QElapsedTimer>

const int  CBoolFormula::LimitGenerate(1000);
const char CBoolFormula::ChNeg('Z');
const char CBoolFormula::ChNull('u');
const char CBoolFormula::ChPos('X');

const char* CBoolFormula::NameSat = "Методы решения задачи выполнимости";
const char* CBoolFormula::NameSatExhaustive = "Метод полного перебора";
const char* CBoolFormula::NameSatMinClaus = "Метод минимального дизъюнкта";

bool BFParam::isValid()
{
    return (numLit > 0
            && numNotLit > 0
            && numNotLit <= numLit
            && numClaus > 0
            && minLenClaus > 0
            && minLenClaus <= numLit
            && maxLenClaus >= minLenClaus
            && maxLenClaus <= numLit
            ) ? true : false;
}
//---------------------------------------------------------------


CBoolFormula::CBoolFormula(const BFParam &p) :
    m_prm(p),
    m_claus(QList<QList<int> >()),
    m_lits(QMap<int,QList<int> >()),
    m_isCreate(false),
    m_isChange(false),
    m_isTerminate(false)
{
}
//---------------------------------------------------------------


CBoolFormula::CBoolFormula(const QList<QList<int> > &cnf) :
    m_prm(BFParam()),
    m_claus(sort(cnf)),
    m_lits(toLits(m_claus)),
    m_isCreate(true),
    m_isChange(false),
    m_isTerminate(false)
{
    updateParam();
}
//---------------------------------------------------------------


QString CBoolFormula::dimacs() const
{
    QString str = QString("p cnf %1 %2\n")
            .arg(numLit(true))
            .arg(numClaus());

    foreach (QList<int> iClaus, m_claus) {
        foreach (int x, iClaus)
            str.append(QString("%1 ").arg(x));
        str.append("0\n");
    }

    return str;
}
//---------------------------------------------------------------


int CBoolFormula::absorb(const QList<QList<int> > &lstClaus,
                         const QList<QByteArray> &lstMask,
                         const QList<int> &claus,
                         const QByteArray &mask) const
{
    for (int i = 0; i < lstClaus.size(); ++i) {

        bool isEqualent = false;

        if (lstClaus.at(i).size() < claus.size())
            isEqualent = equalent(lstClaus.at(i),lstMask.at(i),mask);
        else
            isEqualent = equalent(claus,mask,lstMask.at(i));

        if (isEqualent)
            return i;
    }

    return -1;
}
//---------------------------------------------------------------


bool CBoolFormula::equalent(const QList<int> &lc,const QByteArray &mlc, const QByteArray &mrc) const
{
    foreach (int x, lc)
        if (mlc.at(qAbs(x)) != mrc.at(qAbs(x)))
            return false;

    return true;
}
//---------------------------------------------------------------


CBoolFormula& CBoolFormula::generate()
{
    if (!m_prm.isValid())
        return *this;

    int n(m_prm.numLit);
    int nnot(m_prm.numNotLit);
    int m(m_prm.numClaus);
    int kmin(m_prm.minLenClaus);
    int kmax(m_prm.maxLenClaus);

    m_claus = QList<QList<int> >();
    m_isCreate = false;
    m_isTerminate = false;

    qsrand(QTime::currentTime().msec());

    // generate clauses is include all lits

    QList<int> litUnused;

    for (int i = 1; i <= n; ++i)
        litUnused << i;

    for (int i = 1; i <= nnot; ++i)
        litUnused << -i;

    QList<QByteArray> maskes;   // mask of claus including to m_claus

    int kdiff = kmax - kmin + 1;
    int nsum = n + nnot + 1;

    while (litUnused.size() > 0) {

        QByteArray iMask(n + 1,ChNull);
        QList<int> litUnusedTmp(litUnused);
        QSet<int> litAccess(litUnused.toSet());

        int k = (qrand() % kdiff) + kmin;    // get random number lits in iClaus [kmin..kmax]

        for (; k > 0; --k) {

            int i = -1;

            if (litAccess.size() > 0) {

                // get number lit from access value of litUnused
                do {

                    i = (qrand() % litUnused.size());     // get random index of list numbers lit [0..size - 1]
                    litAccess.remove(litUnused.at(i));    // reduce number of accese literals for iClaus

                } while (litAccess.size() > 0
                         && iMask.at(qAbs(litUnused.at(i))) != ChNull);
            }

            // checked last value i, i < 0 when is not random
            if (i >= 0 && iMask.at(qAbs(litUnused.at(i))) == ChNull) {

                // last random lit is correct, add his to iClaus
                //iClaus << litUnused.at(i);
                iMask[qAbs(litUnused.at(i))] = toChar(litUnused.at(i));
                litUnused.removeAt(i);

            } else {

                // set litUnused has not access lit for iClaus
                // get lit on range [-nnot..n]

                do {

                    i = (qrand() % nsum) - nnot;    // get random number for lit from [-nnot..n]

                } while (i == 0 || iMask.at(qAbs(i)) != ChNull);

                //iClaus << i;
                iMask[qAbs(i)] = toChar(i);
            }
        }   // end get random iClaus

        QList<int> iClaus(toClaus(iMask));
        if (absorb(m_claus,maskes,iClaus,iMask) > 0) {

            //qDebug() << "absorb : " << iClaus;
            litUnused.swap(litUnusedTmp);
        }
        else {

            m_claus << iClaus;
            maskes << iMask;
            //qDebug() << iClaus << " " << iMask;
        }
    }   // end if all lit is included in formula


    // generate it's necessary claus
    for (int i = LimitGenerate; m_claus.size() < m && i > 0; --i) {

        QByteArray iMask(n + 1,ChNull);

        int k = (qrand() % kdiff) + kmin;    // get random number lits in iClaus [kmin..kmax]

        for (; k > 0; --k) {

            if (m_isTerminate) {
                m_claus.clear();
                m_lits.clear();
                return *this;
            }

            int lit = 0;

            do {

                lit = (qrand() % nsum) - nnot;    // get random number for lit from [-nnot..n]

            } while (lit == 0 || iMask.at(qAbs(lit)) != ChNull);

            iMask[qAbs(lit)] = toChar(lit);
        }

        QList<int> iClaus(toClaus(iMask));
        if (absorb(m_claus,maskes,iClaus,iMask) == -1) {
            m_claus << iClaus;
            maskes << iMask;
            i = LimitGenerate;
            //qDebug() << m_claus.at(m_claus.size() - 1) << " " << iMask;
        }
    }

    //qDebug() << "---- " << m_claus.size() << " --------------------------------------";

    m_lits = toLits(m_claus);
    updateParam();
    m_isCreate = true;

    return *this;
}
//---------------------------------------------------------------


int CBoolFormula::maxLit(const QList<QList<int> > &clauses)
{
    int max = 0;
    foreach (QList<int> iClaus, clauses)
        foreach (int x, iClaus)
            if (max < qAbs(x))
                max = qAbs(x);

    return max;
}
//---------------------------------------------------------------


int CBoolFormula::numLit(bool val) const
{
    if (m_isCreate && m_isChange)
        updateParam();

    return val ? m_prm.numLit : m_prm.numNotLit;
}
//---------------------------------------------------------------


int CBoolFormula::numClaus() const
{
    if (m_isCreate && m_isChange)
        updateParam();

    return m_prm.numClaus;
}
//---------------------------------------------------------------


int CBoolFormula::numLenClaus(bool min) const
{
    if (m_isCreate && m_isChange)
        updateParam();

    return min ? m_prm.minLenClaus : m_prm.maxLenClaus;
}
//---------------------------------------------------------------


void CBoolFormula::setParam(const BFParam &p)
{
    m_prm = p;

    m_claus.clear();
    m_lits.clear();

    m_isCreate = false;
    m_isChange = false;
}
//---------------------------------------------------------------


QList<QList<int> > CBoolFormula::fromDimacs(const QByteArray &text)
{
    QRegExp rx("\\s*p\\s+cnf\\s+(\\d+)\\s+(\\d+)");
    rx.setCaseSensitivity(Qt::CaseInsensitive);

    int pos = 0;
    if ((pos = rx.indexIn(text,pos)) == -1)
        return QList<QList<int> >();

    pos += rx.matchedLength();

    QList<QList<int> > cnf;
    QList<int> icnf;

    rx.setPattern("(\\d+)|(-\\d+)");

    while ((pos = rx.indexIn(text,pos)) != -1) {

        if (rx.cap(1) == "0") {

            if (icnf.size() < 2) // в дизъюнкте должно минимум две переменные
                return QList<QList<int> >();

            cnf.append(icnf);
            icnf.clear();

        } else if (!rx.cap(1).isEmpty()) {
            icnf.append(rx.cap(1).toInt());
        } else if (!rx.cap(2).isEmpty()) {
            icnf.append(rx.cap(2).toInt());
        }

        pos += rx.matchedLength();
    }

    return cnf;
}
//---------------------------------------------------------------


QList<QList<int> > CBoolFormula::sort(const QList<QList<int> > &clauses)
{
    QList<QList<int> > sortClauses;

    unsigned n = maxLit(clauses);

    foreach (QList<int> iClaus, clauses)
        sortClauses << toClaus(toMask(iClaus,n));

    return sortClauses;
}
//---------------------------------------------------------------


QList<int> CBoolFormula::toClaus(const QByteArray &mask)
{
    QList<int> claus;

    for (int i = 1; i < mask.size(); ++i) {
        if (mask.at(i) == ChPos)
            claus << i;
        else if (mask.at(i) == ChNeg)
            claus << -i;
    }

    return claus;
}
//---------------------------------------------------------------


QMap<int,QList<int> > CBoolFormula::toLits(const QList<QList<int> > &clauses)
{
    QMap<int,QList<int> > lits;

    for (int i = 0; i < clauses.size(); ++i)
        foreach (int x, clauses.at(i))
            lits[x] << i;

    return lits;
}
//---------------------------------------------------------------


QByteArray CBoolFormula::toMask(const QList<int> &claus, unsigned len)
{
    QByteArray mask(len,ChNull);
    foreach (int x, claus)
        mask[qAbs(x)] = toChar(x);

    return mask;
}
//---------------------------------------------------------------


void CBoolFormula::satMinClaus()
{
    PtrSatData satData(new BFSatData);
    QElapsedTimer timer;
    timer.start();
    satData.data()->log = QByteArray("MinClause");

    for (int i = 0; i < 1000000; ++i)
        satData.data()->time = i;

    satData.data()->time = timer.nsecsElapsed();
    m_satData.insert(MinClaus,satData);
}
//---------------------------------------------------------------


void CBoolFormula::updateParam() const
{
    // update value number of clauses
    m_prm.numClaus = m_claus.size();

    // update value number of literals
    m_prm.numLit = 0;
    m_prm.numNotLit = 0;
    foreach (int lit, m_lits.keys())
        (lit > 0) ? ++m_prm.numLit : ++m_prm.numNotLit;

    // update value min and max lenght of claus
    if (m_prm.numClaus > 0 && m_claus.at(0).size() > 0) {
        m_prm.minLenClaus = m_claus.at(0).size();
        m_prm.maxLenClaus = m_claus.at(0).size();
    } else {
        m_prm.minLenClaus = 0;
        m_prm.maxLenClaus = 0;
    }

    foreach (QList<int> iClaus, m_claus) {

        int iSize = iClaus.size();

        if (m_prm.minLenClaus > iSize)
            m_prm.minLenClaus = iSize;

        if (m_prm.maxLenClaus < iSize)
            m_prm.maxLenClaus = iSize;
    }

    // reset flag changed of bool formulas
    m_isChange = false;
}
//---------------------------------------------------------------


#include <QRegExp>
#include <QSet>
#include <QTime>
#include <QElapsedTimer>

#include "cbool_formula.h"

const char CBoolFormula::ChNeg('Z');
const char CBoolFormula::ChNull('u');
const char CBoolFormula::ChPos('X');

const QMap<CBoolFormula::ESatState,QString>
    CBoolFormula::satStateAlias({{CBoolFormula::Undefine,"не определенно"},
                                {CBoolFormula::Satisfiable,"выполнима"},
                                {CBoolFormula::Unsatisfiable,"не выполнима"}});

const char CBoolFormula::NameSat[] = "Методы решения задачи выполнимости";
const char CBoolFormula::NameSatExhaustive[] = "Метод полного перебора";
const char CBoolFormula::NameSatMinClaus[] = "Метод минимального дизъюнкта";

const QMap<CBoolFormula::ESatAlgorithm,QString>
    CBoolFormula::satAlgAlias({{CBoolFormula::Exhaustive,CBoolFormula::NameSatExhaustive},
                               {CBoolFormula::MinClaus,CBoolFormula::NameSatMinClaus}});


QString CBoolFormula::m_progressMessage("");
const int  CBoolFormula::LimitGenerate(1000);

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
    m_satData({{Exhaustive,PtrSatData(new TSatData())},
               {MinClaus,PtrSatData(new TSatData())}}),
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
    m_satData({{Exhaustive,PtrSatData(new TSatData)},
               {MinClaus,PtrSatData(new TSatData)}}),
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


void CBoolFormula::removeLit(int lit, TLstLst &cls, TMapIntLst &lts)
{
    // remove clauses that contain lit
    foreach (int iCls, lts[lit]) {
        m_qnt += cls.at(iCls).size();
        foreach (int iLit, cls.at(iCls))
            lts[iLit].removeOne(iCls);
        cls[iCls].clear();
    }

    // remove not lit from clauses
    m_qnt += lts[-lit].size();
    foreach (int iCls, lts[-lit])
        cls[iCls].removeOne(-lit);

    lts[-lit].clear();
}
//---------------------------------------------------------------


CBoolFormula::ESatState CBoolFormula::removeOneLits(TLstLst &cls, TMapIntLst &lts, TLst &removed)
{
    // search literals are lastes in claus
    TLst oneLits;           // list literals one contains in claus
    int numNotEmpty = 0;    // number of not empty clauses
    for(int i = 0; i < cls.size(); ++i){
        ++m_qnt;
        if (cls.at(i).size() == 1) {
            int lit = cls[i].takeLast();
            ++m_qnt;
            if (removed.contains(-lit))
                return Unsatisfiable;
            oneLits << lit;
            removed << lit;
        } else if (cls.at(i).size() > 1) {
            ++numNotEmpty;
        }
    }

    // if is not claus contains more one lit and is not conlict that formula Satisfiable
    if (numNotEmpty == 0)
        return Satisfiable;

    // if lits not found then a further reduction is impossible
    if (oneLits.isEmpty())
        return Undefine;

    // removes one lits of clauses
    foreach (int lit, oneLits)
        removeLit(lit,cls,lts);

    return removeOneLits(cls,lts,removed);
}
//---------------------------------------------------------------


QString CBoolFormula::print(const CBoolFormula::TLst &lst)
{
    QString str = "";
    foreach (int l, lst) {
        str.append(QString::number(l));
        str.append(" ");
    }
    str.remove(str.length() - 1,1);

    return str;
}
//---------------------------------------------------------------


QString CBoolFormula::print(const TLstLst &lstLst)
{
    QString str = "";

    foreach (TLst lst, lstLst) {
        str.append(print(lst));
        str.append(", ");
    }

    str.remove(str.length() - 2,2);

    return str;
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
    m_isTerminate = false;
    m_progressMessage = "";
    QString messPattern = "Уровень решения: %1 (количество ветвлений: %2)";

    m_state = Undefine;
    QElapsedTimer timer;
    m_qnt = 0;

    TLstLst rMinClaus;          // number claus with min len and max freq lits of r-levels
    QList<TLstLst> rClaus;      // clauses of r-levels
    QList<TMapIntLst> rLits;    // lits of r-levels
    TLstLst rOrder;             // orders lits satisfaebility of r-levels

    timer.start();

    // create 0 levels
    rClaus << m_claus;
    rLits << m_lits;
    rMinClaus << satMinClaus(rClaus.last(),rLits.last());
    rOrder << TLst();

    m_progressMessage = QString(messPattern)
                .arg(rMinClaus.size())
                .arg(rMinClaus.last().size());

    while (!rMinClaus.isEmpty()) {
        while (!rMinClaus.last().isEmpty()) {

            if (m_isTerminate)
                return;

            rOrder.last() << rMinClaus.last().takeLast();
            removeLit(rOrder.last().last(),rClaus.last(),rLits.last());
            m_state = removeOneLits(rClaus.last(),rLits.last(),rOrder.last());

            if (m_state == Unsatisfiable) {
                 rOrder.last().removeLast();
                 m_progressMessage = QString(messPattern)
                             .arg(rMinClaus.size())
                             .arg(rMinClaus.last().size());
            } else {
                break;
            }
        };

        if (m_state == Satisfiable)
            break;

        if (m_state == Undefine) {
            // go to the next level solutions
            rClaus << rClaus.last();
            rLits << rLits.last();
            rMinClaus << satMinClaus(rClaus.last(),rLits.last());
            rOrder << rOrder.last();
            m_progressMessage = QString(messPattern)
                        .arg(rMinClaus.size())
                        .arg(rMinClaus.last().size());
        } else {
            // return to the prev level solutions
            m_progressMessage = QString(messPattern)
                        .arg(rMinClaus.size())
                        .arg(rMinClaus.last().size());
            rClaus.removeLast();
            rLits.removeLast();
            rMinClaus.removeLast();
            rOrder.removeLast();
        }
    }

    m_satData[MinClaus].data()->log = "";
    m_satData[MinClaus].data()->order = rOrder.isEmpty() ? TLst() : rOrder.last();
    m_satData[MinClaus].data()->state = m_state;
    m_satData[MinClaus].data()->time = timer.nsecsElapsed();
    m_satData[MinClaus].data()->qnt = m_qnt;
}
//---------------------------------------------------------------


void CBoolFormula::satMinClausLog()
{
    m_isTerminate = false;

    QString log = "";
    QString logPattern = "Уровень решения %1 : %2 : %3\n";
    m_progressMessage = "";
    QString messPattern = "Уровень решения: %1 (количество ветвлений: %2)";

    m_state = Undefine;
    QElapsedTimer timer;
    m_qnt = 0;

    TLstLst rMinClaus;          // number claus with min len and max freq lits of r-levels
    QList<TLstLst> rClaus;      // clauses of r-levels
    QList<TMapIntLst> rLits;    // lits of r-levels
    TLstLst rOrder;             // orders lits satisfaebility of r-levels

    timer.start();

    // create 0 levels
    rClaus << m_claus;
    rLits << m_lits;
    rMinClaus << satMinClaus(rClaus.last(),rLits.last());
    rOrder << TLst();

    m_progressMessage = QString(messPattern)
                .arg(rMinClaus.size())
                .arg(rMinClaus.last().size());

    log.append(QString(logPattern)
            .arg(rMinClaus.size())
            .arg("минимальные конъюнкции")
            .arg(print(rMinClaus)));

    while (!rMinClaus.isEmpty()) {
        while (!rMinClaus.last().isEmpty()) {

            if (m_isTerminate)
                return;

            rOrder.last() << rMinClaus.last().takeLast();
            log.append(QString(logPattern)
                         .arg(rMinClaus.size())
                         .arg("удаляем переменную")
                         .arg(rOrder.last().last()));

            removeLit(rOrder.last().last(),rClaus.last(),rLits.last());
            m_state = removeOneLits(rClaus.last(),rLits.last(),rOrder.last());
            log.append(QString(logPattern)
                         .arg(rMinClaus.size())
                         .arg("КНФ")
                         .arg(print(rClaus.last())));

            log.append(QString(logPattern)
                         .arg(rMinClaus.size())
                         .arg("набор выполнимости")
                         .arg(print(rOrder.last())));

            log.append(QString(logPattern)
                         .arg(rMinClaus.size())
                         .arg("статус")
                         .arg(satStateAlias[m_state]));

            log.append("\n");


            if (m_state == Unsatisfiable) {
                 rOrder.last().removeLast();
                 m_progressMessage = QString(messPattern)
                             .arg(rMinClaus.size())
                             .arg(rMinClaus.last().size());
            } else {
                break;
            }
        };

        if (m_state == Satisfiable)
            break;

        if (m_state == Undefine) {
            // go to the next level solutions
            rClaus << rClaus.last();
            rLits << rLits.last();
            rMinClaus << satMinClaus(rClaus.last(),rLits.last());
            rOrder << rOrder.last();

            m_progressMessage = QString(messPattern)
                        .arg(rMinClaus.size())
                        .arg(rMinClaus.last().size());
            log.append(QString(logPattern)
                    .arg(rMinClaus.size())
                    .arg("минимальные конъюнкции")
                    .arg(print(rMinClaus)));
        } else {
            // return to the prev level solutions
            m_progressMessage = QString(messPattern)
                        .arg(rMinClaus.size())
                        .arg(rMinClaus.last().size());
            log.append(QString(logPattern)
                    .arg(rMinClaus.size())
                    .arg("минимальные конъюнкции")
                    .arg(print(rMinClaus)));

            rClaus.removeLast();
            rLits.removeLast();
            rMinClaus.removeLast();
            rOrder.removeLast();
        }
    }

    m_satData[MinClaus].data()->log = log;
    m_satData[MinClaus].data()->order = rOrder.isEmpty() ? TLst() : rOrder.last();
    m_satData[MinClaus].data()->state = m_state;
    m_satData[MinClaus].data()->time = timer.nsecsElapsed();
    m_satData[MinClaus].data()->qnt = m_qnt;
}
//---------------------------------------------------------------


CBoolFormula::TLst CBoolFormula::satMinClaus(const TLstLst &cls, const TMapIntLst &lts)
{
    // calculate freq lits of clauses
    TLst freq;
    for (int i = 0; i < cls.size(); ++i) {
        m_qnt += cls.at(i).size();
        int iFreq = 0;
        foreach (int lit, cls.at(i))
            iFreq += lts[lit].size();
        freq << iFreq;
    }

    // find claus with min len and max freq of lits
    int minNumCls = 0;
    int minLenCls = 9999999;
    int lenCls;

    for (int i = 0; i < cls.size(); ++i) {

        lenCls = cls.at(i).size();

        ++m_qnt;
        if (lenCls == 0)
            continue;

        ++m_qnt;
        if (lenCls < minLenCls) {

            minNumCls = i;
            minLenCls = lenCls;

        } else {

            ++m_qnt;

            // if len equal when determ freq lits of clauses
            if (lenCls == minLenCls) {

                ++m_qnt;
                if (freq.at(i) > freq.at(minNumCls)) {

                    minNumCls = i;
                    minLenCls = lenCls;
                }
            }
        }
    }

    return cls.at(minNumCls);
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


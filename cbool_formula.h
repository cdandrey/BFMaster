#ifndef CBOOL_FORMULA_H
#define CBOOL_FORMULA_H

#include <QByteArray>
#include <QList>
#include <QMap>

class CBoolFormula
{
public:

    struct CParam
    {
        CParam();
        bool isValid();

        int numLit;          // number literals in bool formula
        int numNotLit;       // number negative literals
        int numClaus;        // number clause in bool formula
        int minLenClaus;     // minimal number literals in clause of bool formula
        int maxLenClaus;     // maximal number literals in clause of bool formula
    };

public:

    explicit CBoolFormula(const CParam &p = CParam());
    explicit CBoolFormula(const QList<QList<int> > &cnf);

           void                  breakGenerate() {m_isTerminate = true;}
           QString               dimacs() const;
           CBoolFormula&         generate();

           bool                  isCreate() const {return m_isCreate;}
           bool                  isTerminated() const {return m_isTerminate;}

           const QList<QList<int> >& claus() const {return m_claus;}
           const QMap<int,QList<int> >& lits() const {return m_lits;}

           int                   numLit(bool val = true) const;
           int                   numClaus() const;
           int                   numLenClaus(bool min = true) const;
           int                   progressGenerate() const {return m_claus.size();}
           void                  setParam(const CParam &p);

    static QList<QList<int> >    fromDimacs(const QByteArray &text);
    static int                   maxLit(const QList<QList<int> > &clauses);
    static QList<QList<int> >    sort(const QList<QList<int> > &clauses);
    static char                  toChar(int val) {return (val > 0) ? ChPos : ChNeg;}
    static QList<int>            toClaus(const QByteArray &maskClaus);
    static QMap<int,QList<int> > toLits(const QList<QList<int> > &clauses);
    static QByteArray            toMask(const QList<int> &claus,unsigned len);

    static const char       ChNeg;          // char negative literal
    static const char       ChNull;         // char void literal
    static const char       ChPos;          // char positive literal

private:

           int                   absorb(const QList<QList<int> > &lstClaus,
                                        const QList<QByteArray> &lstMask,
                                        const QList<int> &claus,
                                        const QByteArray &mask) const;

           bool                  equalent(const QList<int> &lc,
                                          const QByteArray &mlc,
                                          const QByteArray &mrc) const;

           void                  updateParam() const;

private:

    mutable CParam          m_prm;          // parametrs of the formula

    QList<QList<int> >      m_claus;        // list clauses of formula
    QMap<int,QList<int> >   m_lits;         // map literals of formula,
                                            // key - literal, value - list clauses included this literal

    bool                    m_isCreate;     // flag formula has created
    mutable bool            m_isChange;     // flag formula has changed
    volatile bool           m_isTerminate;  // flag break generate formula

    static const int        LimitGenerate;  // number of re-creating claus, if create claus is absorb or is there
};


#endif // CBOOL_FORMULA_H

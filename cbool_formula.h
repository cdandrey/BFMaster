#ifndef CBOOL_FORMULA_H
#define CBOOL_FORMULA_H

#include <QByteArray>
#include <QMap>
#include <QSharedPointer>

struct BFParam
{
    bool isValid();

    int numLit;          // number literals in bool formula
    int numNotLit;       // number negative literals
    int numClaus;        // number clause in bool formula
    int minLenClaus;     // minimal number literals in clause of bool formula
    int maxLenClaus;     // maximal number literals in clause of bool formula
};

class CBoolFormula
{
public:

    enum ESatState{Undefine,Satisfiable,Unsatisfiable};
    enum ESatAlgorithm{Exhaustive,MinClaus};

    typedef QList<int>            TLst;
    typedef QList<QList<int> >    TLstLst;
    typedef QMap<int,QList<int> > TMapIntLst;


    typedef struct {
        QString     log = "";    // steps of work algorithm  determine satisfiability
        TLst        order = TLst();  // set variables (implicant) bool formula is satisfiable
        ESatState   state = Undefine;  // sat propety bool formula: satisfaible or unsatisfiable or undefine
        qint64      time = 0;   // time(ns) of work algorithm determine satisfiability
        qint64      qnt = 0;    // quantity operations
    } TSatData;

    typedef QSharedPointer<TSatData> PtrSatData;
    typedef QMap<ESatAlgorithm,PtrSatData> TMapSatAlgorithmData;

public:

    explicit CBoolFormula(const BFParam &p);
    explicit CBoolFormula(const TLstLst &cnf);

           void              breakExecut() {m_isTerminate = true;}
           QString           dimacs() const;
           CBoolFormula&     generate();

           bool              isCreate() const {return m_isCreate;}
           bool              isTerminated() const {return m_isTerminate;}

           const TLstLst&    claus() const {return m_claus;}
           const TMapIntLst& lits() const {return m_lits;}

           int               numLit(bool val = true) const;
           int               numClaus() const;
           int               numLenClaus(bool min = true) const;
           int               progressGenerate() const {return m_claus.size();}
           QString           progressSat() const {return m_progressMessage;}
           void              setParam(const BFParam &p);

    static TLstLst           fromDimacs(const QByteArray &text);
    static int               maxLit(const TLstLst &clauses);
    static TLstLst           sort(const TLstLst &clauses);
    static char              toChar(int val) {return (val > 0) ? ChPos : ChNeg;}
    static TLst              toClaus(const QByteArray &maskClaus);
    static TMapIntLst        toLits(const TLstLst &clauses);
    static QByteArray        toMask(const TLst &claus,unsigned len);

    static const char       ChNeg;          // char negative literal
    static const char       ChNull;         // char void literal
    static const char       ChPos;          // char positive literal

    static const QMap<ESatState,QString> satStateAlias;

    static const char NameSat[];
    static const char NameSatExhaustive[];
    static const char NameSatMinClaus[];

    static const QMap<ESatAlgorithm,QString> satAlgAlias;

public:
    // member-functions SAT problem (determinated satisfiability)
    void satMinClaus();
    void satMinClausLog();
    TLst satMinClaus(const TLstLst &cls,const TMapIntLst &lts);

    // function access for data of sat propety
    QString   satLog(ESatAlgorithm alg) const {return m_satData[alg].data()->log;}
    TLst      satOrder(ESatAlgorithm alg) const {return m_satData[alg].data()->order;}
    ESatState satState(ESatAlgorithm alg) const {return m_satData[alg].data()->state;}
    qint64    satTime(ESatAlgorithm alg) const {return m_satData[alg].data()->time;}
    qint64    satQnt(ESatAlgorithm alg) const {return m_satData[alg].data()->qnt;}

private:

    int  absorb(const TLstLst &lstClaus,
                const QList<QByteArray> &lstMask,
                const TLst &claus,
                const QByteArray &mask) const;

    bool equalent(const TLst &lc,
                   const QByteArray &mlc,
                   const QByteArray &mrc) const;

    // function remove lit from bool formula
    void removeLit(int lit,TLstLst &cls,TMapIntLst &lts);

    // function find all cluases with contain one lit,remove it and check conflict lit * -lit
    ESatState removeOneLits(TLstLst &cls,TMapIntLst &lts,TLst &removed);

    QString print(const TLst &lst);
    QString print(const TLstLst &lstLst);

    void updateParam() const;

private:

    mutable BFParam         m_prm;          // parametrs of the formula

    TLstLst                 m_claus;        // list clauses of formula
    TMapIntLst              m_lits;         // map literals of formula,
                                            // key - literal, value - list clauses included this literal
    TMapSatAlgorithmData    m_satData;      // results of work sat problem algorithm

    bool                    m_isCreate;     // flag formula has created
    mutable bool            m_isChange;     // flag formula has changed
    volatile bool           m_isTerminate;  // flag break generate formula

    ESatState               m_state;        // sat propety bool formula: satisfaible or unsatisfiable or undefine
    qint64                  m_qnt;          // quantity operations

    static QString          m_progressMessage;      // out message for view in GUI
    static const int        LimitGenerate;  // number of re-creating claus, if create claus is absorb or is there
};


#endif // CBOOL_FORMULA_H

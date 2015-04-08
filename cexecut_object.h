#ifndef CEXECUT_OBJECT_H
#define CEXECUT_OBJECT_H

#include <QString>

class CBoolFormula;

class CExecutObject
{
public:

    virtual ~CExecutObject(){}

    virtual QString aliasObject() const = 0;
    virtual CBoolFormula *dataObject() const = 0;
    virtual QString nameObject() const = 0;
    virtual void resetObject(const QString&,CBoolFormula*) {}

    virtual QString progress() const = 0;
    virtual QString progressFinished() const = 0;
    virtual QString progressDescription() const = 0;

    virtual bool isNullObject() const = 0;
    virtual bool run() const = 0;
    virtual void terminated() const = 0;
};


//
class CExecutGenerateFormula : public CExecutObject
{
public:

    explicit CExecutGenerateFormula(const QString &name = "",CBoolFormula *bf = NULL);

    QString aliasObject() const {return "EXE_GENERATE";}
    CBoolFormula *dataObject() const {return m_bf;}
    QString nameObject() const {return m_name;}
    void resetObject(const QString &name,CBoolFormula *bf);

    QString progress() const;
    QString progressFinished() const;
    QString progressDescription() const;

    bool isNullObject() const {return (m_bf == NULL) ? true : false;}
    bool run() const;
    void terminated() const;

private:

    QString m_name;             // uniq name of bool formula in system
    CBoolFormula *m_bf;         // bool formula
    int m_beginNumClaus;        // begin value

};


//
class CExecutSatMinClaus : public CExecutObject
{
public:

    explicit CExecutSatMinClaus(const QString &name = "",CBoolFormula *bf = NULL)
        : m_name(name),m_bf(bf)
    {}

    QString aliasObject() const {return "EXE_SATMINCLAUS";}
    CBoolFormula *dataObject() const {return m_bf;}
    QString nameObject() const {return m_name;}

    void resetObject(const QString &name,CBoolFormula *bf);

    QString progress() const;
    QString progressFinished() const;
    QString progressDescription() const;

    bool isNullObject() const {return (m_bf == NULL) ? true : false;}
    bool run() const;
    void terminated() const;

private:

    QString m_name;             // uniq name of bool formula in system
    CBoolFormula *m_bf;         // bool formula

};

#endif // CEXECUT_OBJECT_H

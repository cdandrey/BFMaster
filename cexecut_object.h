#ifndef CEXECUT_OBJECT_H
#define CEXECUT_OBJECT_H

#include <QString>

class CBoolFormula;

class CExecutObject
{
public:

    enum EModeExecut {Normal,Logging,Testing};

    virtual ~CExecutObject(){}

    virtual QString aliasObject() const = 0;
    virtual CBoolFormula *dataObject() const = 0;
    virtual QString nameObject() const = 0;
    virtual void resetObject(const QString&,CBoolFormula*,EModeExecut mode = Normal) = 0;

    virtual QString log() const {return "";}
    virtual QString progress() const = 0;
    virtual QString progressFinished() const = 0;
    virtual QString progressDescription() const = 0;

    virtual bool isNullObject() const = 0;
    virtual void run() const = 0;
    virtual void terminated() const = 0;
};


//
class CExecutGenerateFormula : public CExecutObject
{
public:

    explicit CExecutGenerateFormula(const QString &name = "",CBoolFormula *bf = NULL,EModeExecut mode = Normal);

    QString aliasObject() const {return "EXE_GENERATE";}
    CBoolFormula *dataObject() const {return m_bf;}
    QString nameObject() const {return m_name;}
    void resetObject(const QString &name,CBoolFormula *bf,EModeExecut mode = Normal);

    inline QString progress() const;
    inline QString progressFinished() const;
    inline QString progressDescription() const;

    bool isNullObject() const {return (m_bf == NULL) ? true : false;}
    inline void run() const;
    inline void terminated() const;

private:

    QString m_name;             // uniq name of bool formula in system
    CBoolFormula *m_bf;         // bool formula
    int m_beginNumClaus;        // begin value
    EModeExecut m_mode;         // mode executing operation

};


//
class CExecutSatMinClaus : public CExecutObject
{
public:

    explicit CExecutSatMinClaus(const QString &name = "",CBoolFormula *bf = NULL,EModeExecut mode = Normal)
        : m_name(name),m_bf(bf),m_mode(mode)
    {}

    QString aliasObject() const {return "EXE_SATMINCLAUS";}
    CBoolFormula *dataObject() const {return m_bf;}
    QString nameObject() const {return m_name;}

    void resetObject(const QString &name,CBoolFormula *bf,EModeExecut mode = Normal);

           QString log() const;
    inline QString progress() const;
    inline QString progressFinished() const;
    inline QString progressDescription() const;

    bool isNullObject() const {return (m_bf == NULL) ? true : false;}
    inline void run() const;
    inline void terminated() const;


private:

    QString m_name;             // uniq name of bool formula in system
    CBoolFormula *m_bf;         // bool formula
    EModeExecut m_mode;         // mode executing operation

};

#endif // CEXECUT_OBJECT_H

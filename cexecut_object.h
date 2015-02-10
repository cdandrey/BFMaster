#ifndef CEXECUT_OBJECT_H
#define CEXECUT_OBJECT_H

#include <QString>

class CBoolFormula;

class CExecutObject
{
public:

    virtual ~CExecutObject(){}

    virtual QString nameObject() const = 0;

    virtual QString progress() const = 0;
    virtual QString progressFinished() const = 0;
    virtual QString progressDescription() const = 0;

    virtual bool isNullObject() const = 0;

    virtual bool run() = 0;

    virtual void terminated() = 0;
};


class CExecutGenerateFormula : public CExecutObject
{
public:
    CExecutGenerateFormula();
    CExecutGenerateFormula(const QString &name,CBoolFormula *bf);

    CBoolFormula *dataObject() {return m_bf;}
    QString nameObject() const {return m_name;}

    QString progress() const;
    QString progressFinished() const;
    QString progressDescription() const;

    bool isNullObject() const {return (m_bf == NULL) ? true : false;}
    bool run();
    void terminated();

private:

    QString m_name;             // uniq name of bool formula in system
    CBoolFormula *m_bf;         // bool formula
    int m_beginNumClaus;        // begin value

};

#endif // CEXECUT_OBJECT_H

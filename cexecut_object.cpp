#include "cexecut_object.h"
#include "cbool_formula.h"
#include <QDebug>

CExecutGenerateFormula::CExecutGenerateFormula(const QString &name, CBoolFormula *bf)
    : m_name(name),
      m_bf(bf),
      m_beginNumClaus(m_bf ? m_bf->numClaus() : 0)
{
}
//---------------------------------------------------------------


void CExecutGenerateFormula::resetObject(const QString &name, CBoolFormula *bf)
{
    if (m_name == name && m_bf == bf)
        return;

    m_name = name;
    m_bf = bf;
    m_beginNumClaus = bf->numClaus();
}
//---------------------------------------------------------------


QString CExecutGenerateFormula::progress() const
{
    return QString("созданно дизъюнктов: %1")
            .arg(m_bf->progressGenerate());
}
//---------------------------------------------------------------


QString CExecutGenerateFormula::progressFinished() const
{
    QString str = "";

    if (!m_bf->isCreate()) {

        str = "Функция не созданна.";

    } else {

        if (m_beginNumClaus > m_bf->numClaus()) {

            str = QString("Cоздано максимально возможное количество дизьюнктов: %1")
                    .arg(m_bf->numClaus());

        } else if (m_beginNumClaus < m_bf->numClaus()) {

            str = QString("Создано минимально возможное количество дизьюнктов: %1")
                    .arg(m_bf->numClaus());
        } else {

            str = QString("Всего cозданно дизъюнктов: %1")
                    .arg(m_bf->numClaus());
        }


        str += QString("\nФункция: %1 - создана успешно.\n")
                .arg(m_name);
    }

    return str;
}
//-----------------------------------------------------------------------


QString CExecutGenerateFormula::progressDescription() const
{
    return QString("%1 %2 ")
            .arg("Создается функция")
            .arg(m_name);
}
//-----------------------------------------------------------------------


bool CExecutGenerateFormula::run() const
{
    m_bf->generate();
    return m_bf->isCreate();
}
//-----------------------------------------------------------------------

void CExecutGenerateFormula::terminated() const
{
    m_bf->breakExecut();
}
//-----------------------------------------------------------------------


// SatMinClause

void CExecutSatMinClaus::resetObject(const QString &name, CBoolFormula *bf)
{
    if (m_name == name && m_bf == bf)
        return;

    m_name = name;
    m_bf = bf;
}
//---------------------------------------------------------------

QString CExecutSatMinClaus::progress() const
{
    return QString("progress satMinClause");
    //.arg(m_bf->progressGenerate());
}
//---------------------------------------------------------------


QString CExecutSatMinClaus::progressFinished() const
{
    QString str = "finished MinClaus";

    return str;
}
//-----------------------------------------------------------------------


QString CExecutSatMinClaus::progressDescription() const
{
    return QString("%1.%2 %3")
            .arg(CBoolFormula::NameSatMinClaus)
            .arg("Определяется выполнимость функции: ")
            .arg(m_name);
}
//-----------------------------------------------------------------------


bool CExecutSatMinClaus::run() const
{
    m_bf->satMinClaus();
    return true;
}
//-----------------------------------------------------------------------


void CExecutSatMinClaus::terminated() const
{
    m_bf->breakExecut();
}
//-----------------------------------------------------------------------

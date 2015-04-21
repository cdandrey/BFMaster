#include "cexecut_object.h"
#include "cbool_formula.h"

CExecutGenerateFormula::CExecutGenerateFormula(const QString &name, CBoolFormula *bf, EModeExecut mode)
    : m_name(name),
      m_bf(bf),
      m_beginNumClaus(m_bf ? m_bf->numClaus() : 0),
      m_mode(mode)
{
}
//---------------------------------------------------------------


void CExecutGenerateFormula::resetObject(const QString &name, CBoolFormula *bf, EModeExecut mode)
{
    if (m_name == name && m_bf == bf)
        return;

    m_name = name;
    m_bf = bf;
    m_beginNumClaus = bf->numClaus();
    m_mode = mode;
}
//---------------------------------------------------------------


inline QString CExecutGenerateFormula::progress() const
{
    return QString("созданно дизъюнктов: %1")
            .arg(m_bf->progressGenerate());
}
//---------------------------------------------------------------


inline QString CExecutGenerateFormula::progressFinished() const
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


inline QString CExecutGenerateFormula::progressDescription() const
{
    return QString("%1 %2 ")
            .arg("Создается функция")
            .arg(m_name);
}
//-----------------------------------------------------------------------


inline void CExecutGenerateFormula::run() const
{
    m_bf->generate();
}
//-----------------------------------------------------------------------


inline void CExecutGenerateFormula::terminated() const
{
    m_bf->breakExecut();
}
//-----------------------------------------------------------------------


// SatMinClause

void CExecutSatMinClaus::resetObject(const QString &name, CBoolFormula *bf, EModeExecut mode)
{
    m_mode = mode;

    if (m_name == name && m_bf == bf)
        return;

    m_name = name;
    m_bf = bf;
}
//---------------------------------------------------------------


inline QString CExecutSatMinClaus::progress() const
{
    return m_bf->progressSat();
}
//---------------------------------------------------------------


inline QString CExecutSatMinClaus::progressFinished() const
{
    return QString("%1 - %2")
            .arg(m_name)
            .arg(CBoolFormula::satStateAlias[m_bf->satState(CBoolFormula::MinClaus)]);
}
//-----------------------------------------------------------------------


inline QString CExecutSatMinClaus::progressDescription() const
{
    return QString("%1.%2 %3")
            .arg(CBoolFormula::NameSatMinClaus)
            .arg("Определяется выполнимость функции: ")
            .arg(m_name);
}
//-----------------------------------------------------------------------


inline void CExecutSatMinClaus::run() const
{
    switch(m_mode){
    case Normal:
        m_bf->satMinClaus();
        break;
    case Logging:
        m_bf->satMinClausLog();
        break;
    default:
        break;
    }
}
//-----------------------------------------------------------------------


inline void CExecutSatMinClaus::terminated() const
{
    m_bf->breakExecut();
}
//-----------------------------------------------------------------------


QString CExecutSatMinClaus::log() const
{
    return m_bf->satLog(CBoolFormula::MinClaus);
}
//-----------------------------------------------------------------------

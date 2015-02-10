#include "cbool_formula.h"
#include "cexecut_object.h"

CExecutGenerateFormula::CExecutGenerateFormula() :
    m_name(""),
    m_bf(NULL)
{
}
//---------------------------------------------------------------


CExecutGenerateFormula::CExecutGenerateFormula(const QString &name, CBoolFormula *bf) :
    m_name(name),
    m_bf(bf),
    m_beginNumClaus(bf->numClaus())
{
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


bool CExecutGenerateFormula::run()
{
    m_bf->generate();
    return m_bf->isCreate();
    m_bf = NULL;
}
//-----------------------------------------------------------------------


void CExecutGenerateFormula::terminated()
{
    m_bf->breakGenerate();
}
//---------------------------------------------------------------



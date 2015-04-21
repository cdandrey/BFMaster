#include <QAction>
#include <QActionGroup>
#include <QComboBox>
#include <QLabel>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QVBoxLayout>

#include "cwidget_bf_view.h"
#include "cbool_formula.h"
#include "ctoolbar_header.h"

const QString CWidgetBFView::TableStyle("cellspacing = '3' "
                           "cellpadding = '5' "
                           "border = '0' "
                           "style = 'border-style:solid;'");

const QString CWidgetBFView::BgColorC("bgcolor = '#cccccc'");
const QString CWidgetBFView::BgColorE("bgcolor = '#eeeeee'");
const QString CWidgetBFView::BgColorF("bgcolor = '#ffffff'");

CWidgetBFView::CWidgetBFView(QWidget *parent) :
    QWidget(parent),
    m_bfName(NULL),
    m_bf(NULL)
{
    m_header = new CToolBarHeader();

    m_lableNameFormula = new QLabel(this);
    m_lableNameFormula->setFixedHeight(m_header->minimumHeight());
    m_lableNameFormula->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    on_setLableNameFormula();

    m_actViewFormula = new QAction(QIcon("://ico/bf_view.png"),
                                   tr("Общая информация функции"),this);

    m_actViewDimacs = new QAction(QIcon("://ico/bf_viewdimacs.png"),
                                  tr("Представление функции в формате DIMACS"),this);

    m_actViewClaus = new QAction(QIcon("://ico/bf_viewdis.png"),
                                 tr("Список дизъюнктов функции"),this);

    m_actViewClausSort = new QAction(QIcon("://ico/bf_viewdissort.png"),
                                     tr("Упорядоченный список дизъюнктов функции"),this);

    m_actViewLits = new QAction(QIcon("://ico/bf_viewvar.png"),
                                tr("Список переменных функции"),this);

    m_actViewLitsSort = new QAction(QIcon("://ico/bf_viewvarsort.png"),
                                    tr("Упорядоченный список переменных функции"),this);

    m_actViewSat = new QAction(QIcon("://ico/bf_viewsat_add.png"),
                               tr("Отображать общий отчет решения SAT"),this);

    m_groupActView = new QActionGroup(this);
    m_groupActView->addAction(m_actViewFormula);
    m_groupActView->addAction(m_actViewDimacs);
    m_groupActView->addAction(m_actViewClaus);
    m_groupActView->addAction(m_actViewClausSort);
    m_groupActView->addAction(m_actViewLits);
    m_groupActView->addAction(m_actViewLitsSort);
    m_groupActView->addAction(m_actViewSat);

    m_actViewFormula->setCheckable(true);
    m_actViewDimacs->setCheckable(true);
    m_actViewClaus->setCheckable(true);
    m_actViewClausSort->setCheckable(true);
    m_actViewLits->setCheckable(true);
    m_actViewLitsSort->setCheckable(true);
    m_actViewSat->setCheckable(true);

    m_header->insertWidget(m_header->actHint(),m_lableNameFormula);
    m_header->insertSpace(m_header->actHint(),16);
    m_header->insertActions(m_header->actHint(),m_groupActView->actions());
    m_header->insertAction(m_header->actHint(),m_actViewSat);
    m_header->insertStretch(m_header->actHint());

    m_edit = new QTextEdit();
    m_edit->setFont(QFont("Arial",12));
    m_edit->setReadOnly(true);
    m_edit->document()->setDocumentMargin(10);
    m_edit->setEnabled(m_bf != NULL);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    this->setLayout(vbox);

    vbox->setMenuBar(m_header);
    vbox->addWidget(m_edit);
    vbox->setStretch(1,100);

    connect(m_header->actHint(),SIGNAL(triggered()),this,SLOT(triggered_actHide()));
    connect(m_actViewFormula,SIGNAL(toggled(bool)),this,SLOT(on_viewFormula(bool)));
    connect(m_actViewDimacs,SIGNAL(toggled(bool)),this,SLOT(on_viewDimacs(bool)));
    connect(m_actViewClaus,SIGNAL(toggled(bool)),this,SLOT(on_viewClaus(bool)));
    connect(m_actViewClausSort,SIGNAL(toggled(bool)),this,SLOT(on_viewClausSort(bool)));
    connect(m_actViewLits,SIGNAL(toggled(bool)),this,SLOT(on_viewLits(bool)));
    connect(m_actViewLitsSort,SIGNAL(toggled(bool)),this,SLOT(on_viewLitsSort(bool)));
    connect(m_actViewSat,SIGNAL(toggled(bool)),this,SIGNAL(getLogSelectSat(bool)));

    connect(this,SIGNAL(setHtml(QString)),m_edit,SLOT(setHtml(QString)));
    connect(this,SIGNAL(setText(QString)),m_edit,SLOT(setPlainText(QString)));
}
//------------------------------------------------------------------


void CWidgetBFView::on_set(const QString &name, CBoolFormula *bf)
{
    m_bfName = const_cast<QString*>(&name);
    m_bf = bf;

    if (m_bf != NULL && m_bfName != NULL){
        m_edit->setEnabled(true);
    } else {
        m_edit->clear();
        m_edit->setEnabled(false);
    }

    on_viewUpdate();
}
//------------------------------------------------------------------


void CWidgetBFView::on_viewClaus(bool isToggle)
{
    if (!isToggle || !isBoolFormula())
        return;

    int num = m_bf->numClaus();
    int numView = num > 1000 ? 1000 : num; // количество отображаемых дизьюнктов

    QString html("");

    if (numView < num)
        html = QObject::tr("<p><i> Функция большая"
             ", выводится только первые тысячу дизьюнктов</i></p>");

    html += QString("<table %1 width='100%'>").arg(TableStyle);
    html += QString("<tr><td colspan = '3'>%1</td></tr><tr>&nbsp;</tr>")
            .arg(m_actViewClaus->toolTip());

    QString trPattern("<tr><td width='3%'>%1</td><td %2 width='7%'><b>%3</b> (%4)</td><td %5>");
    QString trColor(BgColorE);
    for (int i = 0; i < numView; ++i) {

        html += trPattern.arg(i + 1).arg(BgColorC).arg(htmlClaus(i + 1))
                 .arg(m_bf->claus().at(i).size()).arg(trColor);

        foreach (int lit, m_bf->claus().at(i))
            html += QString("%1 ").arg(htmlLit(lit));

        html += "</td></tr>";

        trColor = (trColor == BgColorE) ? BgColorF : BgColorE;
    }

    html += "</table>";

    emit setHtml(html);
}
//------------------------------------------------------------------


void CWidgetBFView::on_viewClausSort(bool isToggle)
{
    if (!isToggle || !isBoolFormula())
        return;

    // сортируем список дизьюнктов в порядке возростяния
    // числа литералов которые они содержат
    int maxSize = m_bf->numLenClaus(false);

    QList<QList<int> > sizes;
    for (int i = 0; i <= maxSize; ++i)
        sizes << QList<int>();

    for (int i = 0; i < m_bf->claus().size(); ++i)
        sizes[m_bf->claus().at(i).size()] << i;

    QList<int> lstClausSort;
    foreach (QList<int> lst, sizes)
        foreach (int iClaus, lst)
            lstClausSort << iClaus;

    // формируем вывод списка дизьюнктов

    int num = m_bf->numClaus();
    int numView = num > 1000 ? 1000 : num; // количество отображаемых дизьюнктов

    QString html = "";

    if (numView < num)
        html = tr("<p><i> Функция большая"
             ", выводится только первые тысячу дизьюнктов</i></p>");

    html += QString("<table %1 width='100%'>").arg(TableStyle);
    html += QString("<tr><td colspan = '3'>%1</td></tr><tr>&nbsp;</tr>")
            .arg(m_actViewClausSort->toolTip());

    QString trPattern("<tr><td  width='3%'>%1</td><td %2 width='7%'><b>%3</b> (%4)</td><td %5>");
    QString trColor(BgColorE);
    for (int i = 0; i < numView; ++i) {

        int iCls = lstClausSort.at(i);

        html += trPattern.arg(i + 1).arg(BgColorC).arg(htmlClaus(iCls + 1))
                    .arg(m_bf->claus().at(iCls).size()).arg(trColor);

        foreach (int lit, m_bf->claus().at(iCls))
            html += QString("%1 ").arg(htmlLit(lit));

        html += "</td></tr>";

        trColor = (trColor == BgColorE) ? BgColorF : BgColorE;
    }

    html += "</table>";

    emit setHtml(html);
}
//------------------------------------------------------------------


void CWidgetBFView::on_viewDimacs(bool isToggle)
{
    if (!isToggle || !isBoolFormula())
        return;

    emit setText(QString("%1\n\n%2")
                 .arg(m_actViewDimacs->toolTip())
                 .arg(m_bf->dimacs()));
}
//------------------------------------------------------------------


void CWidgetBFView::on_viewFormula(bool isToggle)
{
    if (!isToggle || !isBoolFormula())
        return;

    QString html("");

    html.append(QString("<table %1>").arg(TableStyle));
    html.append(QString("<tr><td colspan = '2'>%1</td></tr><tr>&nbsp;</tr>")
                .arg(m_actViewFormula->toolTip()));

    html.append(QString("<tr><td %1><b>%2</b></td><td %3>%4</td></tr>")
                .arg(BgColorC).arg(tr("Имя функции"))
                .arg(BgColorE).arg(*m_bfName));

    html.append(QString("<tr><td %1><b>%2</b></td><td %3>%4</td></tr>")
                .arg(BgColorC).arg(tr("Количество переменных"))
                .arg(BgColorE).arg(m_bf->numLit(true)));

    html.append(QString("<tr><td %1><b>%2</b></td><td %3>%4</td></tr>")
                .arg(BgColorC).arg(tr("Количество противоположных переменных"))
                .arg(BgColorE).arg(m_bf->numLit(false)));

    html.append(QString("<tr><td %1><b>%2</b></td><td %3>%4</td></tr>")
                .arg(BgColorC).arg(tr("Количество дизьюнктов"))
                .arg(BgColorE).arg(m_bf->numClaus()));

    html.append(QString("<tr><td %1><b>%2</b></td><td %3>%4</td></tr>")
                .arg(BgColorC).arg(tr("Минимальное количество переменных в дизьюнкте"))
                .arg(BgColorE).arg(m_bf->numLenClaus(true)));

    html.append(QString("<tr><td %1><b>%2</b></td><td %3>%4</td></tr>")
                .arg(BgColorC).arg(tr("Максимальное количество переменных в дизьюнкте"))
                .arg(BgColorE).arg(m_bf->numLenClaus(false)));

    html.append("</table>");

    html.append(QString("<p>&nbsp;</p><table %1>")
                .arg(TableStyle));
    html.append(QString("<tr><td colspan = '6'>%1</td></tr>")
                .arg(tr("Выполнимость функции")));

    html.append(QString("<tr %1><td><b>%2</b></td><td><b>%3</b></td>"
                        "<td><b>%4</b></td><td><b>%5</b></td><td><b>%6</b></td></tr>")
                .arg(BgColorC)
                .arg(tr("Метод"))
                .arg(tr("Статус"))
                .arg(tr("Набор"))
                .arg(tr("Операции"))
                .arg(tr("Время, мс"))
                );

    QMapIterator<CBoolFormula::ESatAlgorithm,QString> itAlg(m_bf->satAlgAlias);
    while (itAlg.hasNext()) {
        itAlg.next();
        html.append(QString("<tr><td %1><b>%2</b></td><td>%3</td>"
                            "<td>%4</td><td>%5</td><td>%6</td></tr>")
                    .arg(BgColorE)
                    .arg(itAlg.value())
                    .arg(CBoolFormula::satStateAlias[m_bf->satState(itAlg.key())])
                    .arg(htmlLstLit(m_bf->satOrder(itAlg.key())," "))
                    .arg(m_bf->satQnt(itAlg.key()))
                    .arg(static_cast<double>(m_bf->satTime(itAlg.key())/10.0e6))   // divided 10^6 convert from ns to ms
                    );
    }

    html.append("</table>");

    emit setHtml(html);
}
//------------------------------------------------------------------


void CWidgetBFView::on_viewLits(bool isToggle)
{
    if (!isToggle || !isBoolFormula())
        return;

    int num = m_bf->numLit(true);
    QList<int> lstLit(m_bf->lits().keys());
    int numView = num > 100 ? 100 : lstLit.size(); // количество отображаемых переменных

    QString html = "";

    if (numView < num)
        html = tr("<p><i> Функция большая"
             ", выводится только первые сто переменных</i></p>");

    html += QString("<table %1 width='100%'>").arg(TableStyle);
    html += QString("<tr><td colspan = '3'>%1</td></tr><tr>&nbsp;</tr>")
            .arg(m_actViewLits->toolTip());

    int i = 1;
    QString trPattern("<tr><td  width='3%'>%1</td><td %2 width='7%'><b>%3 </b>(%4)</td><td %5>");
    QString trColor(BgColorE);
    foreach (int lit, lstLit) {

        if (lit < 0)
            continue;

        // выводим i-й литерал
        html += trPattern.arg(i).arg(BgColorC).arg(htmlLit(lit))
                         .arg(m_bf->lits().value(lit).size()).arg(trColor);

        foreach (int d, m_bf->lits().value(lit))
            html += QString("%1 ").arg(htmlClaus(d + 1));

        html += "</td></tr>";

        // выводим литерал противоположный i-му если он существует
        int notLit = -lit;
        if (m_bf->lits().find(notLit) != m_bf->lits().end()) {
            ++i;
            html += trPattern.arg(i).arg(BgColorC).arg(htmlLit(notLit))
                             .arg(m_bf->lits().value(notLit).size()).arg(trColor);

            foreach (int d, m_bf->lits().value(notLit))
                html += QString("%1 ").arg(htmlClaus(d + 1));

            html += "</td></tr>";
        }

        if (i >= numView)
            break;

        ++i;
        trColor = (trColor == BgColorE) ? BgColorF : BgColorE;
    }

    html += "</table>";

    emit setHtml(html);
}
//------------------------------------------------------------------


void CWidgetBFView::on_viewLitsSort(bool isToggle)
{
    if (!isToggle || !isBoolFormula())
        return;

    // сортируем список литералов в порядке возростяния
    // числа дизьюнктов в которых они содержатся
    int maxSize = 0;
    QList<int> lstLit(m_bf->lits().keys());
    foreach (int lit, lstLit)
        if (maxSize < m_bf->lits().value(lit).size())
            maxSize = m_bf->lits().value(lit).size();

    QList<QList<int> > maskLitSize;
    for (int i = 0; i <= maxSize; ++i)
        maskLitSize << QList<int>();

    int numNot = m_bf->numLit(false);
    for (int i = numNot; i < lstLit.size(); ++i)    // перебираем положительные литералы
        maskLitSize[m_bf->lits().value(lstLit.at(i)).size()] << lstLit.at(i);
    for (int i = numNot - 1; i >= 0; --i)            // перебираем отрицательные литералы
        maskLitSize[m_bf->lits().value(lstLit.at(i)).size()] << lstLit.at(i);

    QList<int> lstLitSort;
    foreach (QList<int> lst, maskLitSize)
        foreach (int lit, lst)
            lstLitSort << lit;

    // формируем вывод списка литералов
    int num = m_bf->numLit(true);
    int numView = num > 100 ? 100 : lstLitSort.size(); // количество выводимых литералов

    QString html("");

    if (numView < num)
        html = tr("<p><i> Функция большая"
             ", выводится только первые сто переменных</i></p>");

    html += QString("<table %1 width='100%'>").arg(TableStyle);
    html += QString("<tr><td colspan = '3'>%1</td></tr><tr>&nbsp;</tr>")
            .arg(m_actViewLitsSort->toolTip());

    QString trPattern("<tr><td  width='3%'>%1</td><td %2 width='7%'><b>%3 </b>(%4)</td><td %5>");
    QString trColor(BgColorE);

    for (int i = 0; i < numView; ++i) {

        int lit = lstLitSort.at(i);

        html += trPattern.arg(i + 1).arg(BgColorC).arg(htmlLit(lit))
                    .arg(m_bf->lits().value(lit).size()).arg(trColor);

        foreach (int d, m_bf->lits().value(lit))
            html += QString("%1 ").arg(htmlClaus(d + 1));

        html += "</td></tr>";

        trColor = (trColor == BgColorE) ? BgColorF : BgColorE;
    }

    html += "</table>";

    emit setHtml(html);
}
//------------------------------------------------------------------


void CWidgetBFView::on_viewUpdate()
{
    if (!isBoolFormula()) {
        checked_Action()->setChecked(false);
        return;
    }

    // update lable
    on_setLableNameFormula();

    // update edit and control panel
    QAction *act = checked_Action();
    act->setChecked(false);
    act->setChecked(true);
}
//------------------------------------------------------------------


QAction *CWidgetBFView::checked_Action() const
{
    foreach(QAction* act,m_groupActView->actions())
        if (act->isChecked())
            return act;

    return m_actViewFormula;
}
//------------------------------------------------------------------


bool CWidgetBFView::isBoolFormula()
{
    if (m_bf != NULL && m_bfName != NULL && !m_bfName->isEmpty()) {
        if (m_bf->isCreate()) {
            m_edit->setEnabled(true);
            return true;
        } else {
            emit message(tr("%1 - функция не создана.")
                         .arg(*m_bfName));
        }
    }

    triggered_actHide();
    return false;
}
//------------------------------------------------------------------


inline QString CWidgetBFView::htmlLit(int lit) const
{
    return (lit > 0) ? QString("%1<sub>%2</sub>")
                        .arg(CBoolFormula::ChPos).arg(lit) :
                       QString("%1<sub>%2</sub>")
                       .arg(CBoolFormula::ChNeg).arg(qAbs(lit));
}
//------------------------------------------------------------------


inline QString CWidgetBFView::htmlClaus(int claus) const
{
    return QString("C<sub>%1</sub>").arg(claus);
}
//------------------------------------------------------------------


QString CWidgetBFView::htmlLstLit(const QList<int> &lst, const QString &divider) const
{
    QString html = "";
    foreach (int lit, lst) {
        html.append(QString("%1%2")
                    .arg(htmlLit(lit))
                    .arg(divider)
                    );
    }

    return html.left(html.length() - divider.length());
}
//------------------------------------------------------------------


void CWidgetBFView::on_setLableNameFormula()
{
    if (m_bfName != NULL && !m_bfName->isEmpty())
        m_lableNameFormula->setText(*m_bfName);
    else
        m_lableNameFormula->setText(tr("<нет функции>"));
}
//------------------------------------------------------------------


void CWidgetBFView::triggered_actHide()
{
    m_bf = NULL;
    m_bfName = NULL;
    on_setLableNameFormula();
    m_edit->clear();
    m_edit->setEnabled(false);
}
//------------------------------------------------------------------


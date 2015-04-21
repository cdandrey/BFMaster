#include <QAction>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

#include "cwidget_tree_sat.h"
#include "cbool_formula.h"
#include "cexecut_object.h"
#include "ctoolbar_header.h"

CWidgetTreeSat::CWidgetTreeSat(QWidget *parent) :
    QWidget(parent),
    m_bfName(NULL),
    m_bf(NULL)
{
    m_header = new CToolBarHeader(tr("Алгоритмы"));

    m_header->insertStretch(m_header->actHint());

    m_tree = new QTreeWidget();
    m_tree->setMinimumWidth(150);
    m_tree->setHeaderHidden(true);
    createTreeItem();

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    this->setLayout(vbox);

    vbox->addWidget(m_header);
    vbox->addWidget(m_tree);

    m_actVisible = new QAction(tr("Отображать список алгоритмов"),this);
    m_actVisible->setCheckable(true);

    connect(m_header->actHint(),SIGNAL(triggered()),this,SLOT(hide()));
    connect(m_actVisible,SIGNAL(toggled(bool)),this,SLOT(setVisible(bool)));
    connect(m_header->actHint(),SIGNAL(triggered(bool)),m_actVisible,SLOT(setChecked(bool)));
}
//------------------------------------------------------------------


CWidgetTreeSat::~CWidgetTreeSat()
{
    qDeleteAll(m_obj);
}
//------------------------------------------------------------------


void CWidgetTreeSat::on_disabledHide(bool disabled)
{
    m_header->actHint()->setDisabled(disabled);
}
//------------------------------------------------------------------


void CWidgetTreeSat::createTreeItem()
{
    QTreeWidgetItem *itTop = new QTreeWidgetItem(m_tree);
    itTop->setText(0,tr(CBoolFormula::NameSat));

    QTreeWidgetItem *itExh = new QTreeWidgetItem(itTop);
    itExh->setText(0,tr(CBoolFormula::NameSatExhaustive));
    itExh->setCheckState(0,Qt::Unchecked);
    m_obj.insert(itExh,new CExecutSatMinClaus());

    QTreeWidgetItem *itMCl = new QTreeWidgetItem(itTop);
    itMCl->setText(0,tr(CBoolFormula::NameSatMinClaus));
    itMCl->setCheckState(0,Qt::Unchecked);
    m_obj.insert(itMCl,new CExecutSatMinClaus());

    m_tree->expandAll();
}
//------------------------------------------------------------------


bool CWidgetTreeSat::isEmpty()
{
    return (m_bf == NULL || !m_bf->isCreate() || m_bfName == NULL || m_bfName->isEmpty());
}
//------------------------------------------------------------------


void CWidgetTreeSat::on_returnLogSelectSat(bool isGet)
{
    if (isEmpty() || !isGet)
        return;

    for (TMapItemExeObject::iterator it = m_obj.begin();
         it != m_obj.end(); ++it)
        if (it.key() == m_tree->currentItem()) {
            emit logSelectSat(it.value()->log());
            return;
        }
}
//------------------------------------------------------------------


void CWidgetTreeSat::on_runChecked()
{
    if (isEmpty())
        return;

    QList<CExecutObject*> lstObj;


    for (TMapItemExeObject::iterator it = m_obj.begin();
         it != m_obj.end(); ++it)
    {
        if (it.key()->checkState(0)) {
            it.value()->resetObject(*m_bfName,m_bf,CExecutObject::Normal);
            lstObj << it.value();
        }
    }

    emit execut(lstObj);
}
//------------------------------------------------------------------


void CWidgetTreeSat::on_runLogChecked()
{
    if (isEmpty())
        return;

    QList<CExecutObject*> lstObj;


    for (TMapItemExeObject::iterator it = m_obj.begin();
         it != m_obj.end(); ++it)
    {
        if (it.key()->checkState(0)) {
            it.value()->resetObject(*m_bfName,m_bf,CExecutObject::Logging);
            lstObj << it.value();
        }
    }

    emit execut(lstObj);
}
//------------------------------------------------------------------


void CWidgetTreeSat::on_set(const QString &bfName, CBoolFormula *bf)
{
    if (bf == NULL || bfName == NULL || bfName.isEmpty())
        return;

    m_bfName = const_cast<QString*>(&bfName);
    m_bf = bf;

    for (TMapItemExeObject::iterator it = m_obj.begin();it != m_obj.end(); ++it)
        it.value()->resetObject(*m_bfName,m_bf,CExecutObject::Normal);
}
//------------------------------------------------------------------

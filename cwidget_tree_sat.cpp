#include "cwidget_tree_sat.h"

#include <QAction>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

#include "csat.h"
#include "ctoolbar_header.h"

CWidgetTreeSat::CWidgetTreeSat(QWidget *parent) :
    QWidget(parent)
{
    setStyleSheet("QTreeWidget{border: 0px solid #000;"
                              "border-width: 0px 1px 0px 1px;}");

    m_header = new CToolBarHeader(tr("Алгоритмы"));

    m_header->setStyleSheet("QToolBar {background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #999, stop:1.0 #555);"
                                      "border: 1px solid #000;}"
                            "QLabel   {color: white; font-size: 12px;}");

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

    connect(m_tree,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this,SLOT(clicked_treeWidgetItem(QTreeWidgetItem*)));

    m_actVisible = new QAction(tr("Отображать список алгоритмов"),this);
    m_actVisible->setCheckable(true);

    connect(m_header->actHint(),SIGNAL(triggered()),this,SLOT(hide()));
    connect(m_actVisible,SIGNAL(toggled(bool)),this,SLOT(setVisible(bool)));
    connect(m_header->actHint(),SIGNAL(triggered(bool)),m_actVisible,SLOT(setChecked(bool)));
}
//------------------------------------------------------------------


void CWidgetTreeSat::on_disabledHide(bool disabled)
{
    m_header->actHint()->setDisabled(disabled);
}
//------------------------------------------------------------------


void CWidgetTreeSat::on_locked()
{
    m_tree->setEnabled(false);
}
//------------------------------------------------------------------


void CWidgetTreeSat::on_unlocked()
{
    m_tree->setEnabled(true);
}
//------------------------------------------------------------------


void CWidgetTreeSat::on_runNextChecked()
{
    static int index = 0;

    if (!m_runChecked)
        return;

    while (index < m_tree->topLevelItem(0)->childCount() &&
           m_tree->topLevelItem(0)->child(index)->checkState(0) == Qt::Unchecked)
        ++index;

    if (index < m_tree->topLevelItem(0)->childCount()) {
        emit run(m_tree->topLevelItem(0)->child(index)->text(m_runType));
        ++index;
    } else {
        m_runChecked = false;
        index = 0;
    }
}
//------------------------------------------------------------------


void CWidgetTreeSat::on_startRunChecked()
{
    m_runChecked = true;
    m_runType = Run;
    on_runNextChecked();
}
//------------------------------------------------------------------


void CWidgetTreeSat::on_startRunLogChecked()
{
    m_runChecked = true;
    m_runType = RunLog;
    on_runNextChecked();
}
//------------------------------------------------------------------


void CWidgetTreeSat::createTreeItem()
{
    QTreeWidgetItem *itTop = new QTreeWidgetItem(m_tree);
    itTop->setText(0,tr("Выполнимость"));

    QTreeWidgetItem *itExh = new QTreeWidgetItem(itTop);
    itExh->setText(0,CSatExhaustive::description());
    itExh->setText(Run,CSatExhaustive::name());
    itExh->setCheckState(0,Qt::Unchecked);

    QTreeWidgetItem *itMCl = new QTreeWidgetItem(itTop);
    itMCl->setText(0,CSatMinimumClauses::description());
    itMCl->setText(Run,CSatMinimumClauses::name());
    itMCl->setCheckState(0,Qt::Unchecked);

    QTreeWidgetItem *itRCl = new QTreeWidgetItem(itTop);
    itRCl->setText(0,CSatRandomClauses::description());
    itRCl->setText(Run,CSatRandomClauses::name());
    itRCl->setCheckState(0,Qt::Unchecked);

    m_tree->expandAll();
}
//------------------------------------------------------------------


void CWidgetTreeSat::clicked_treeWidgetItem(QTreeWidgetItem *item)
{
    emit run(item->text(Run));
}
//------------------------------------------------------------------

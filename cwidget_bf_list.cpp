#include "cwidget_bf_list.h"
#include "cmodel_bf_list.h"
#include "ctoolbar_header.h"

#include <QAction>
#include <QLabel>
#include <QListView>
#include <QListWidget>
#include <QVBoxLayout>


CWidgetBFList::CWidgetBFList(QWidget *parent)
    : QWidget(parent)
{
    m_header = new CToolBarHeader(tr("Функции"));

    m_header->insertStretch(m_header->actHint());

    m_list = new QListView();
    m_list->setMinimumWidth(150);

    m_model = new CModelBFList();
    m_list->setModel(m_model);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    this->setLayout(vbox);

    vbox->addWidget(m_header);
    vbox->addWidget(m_list);

    m_actVisible = new QAction(tr("Отображать список функций"),this);
    m_actVisible->setCheckable(true);

    connect(m_header->actHint(),SIGNAL(triggered()),this,SLOT(hide()));
    connect(m_actVisible,SIGNAL(toggled(bool)),this,SLOT(setVisible(bool)));

    connect(m_header->actHint(),SIGNAL(triggered(bool)),
            m_actVisible,SLOT(setChecked(bool)));

    connect(this,SIGNAL(append(QString,CBoolFormula*)),
            m_model,SLOT(on_append(QString,CBoolFormula*)));

    connect(this,SIGNAL(remove(QString)),
            m_model,SLOT(on_remove(QString)));

    connect(m_list,SIGNAL(clicked(QModelIndex)),
            m_model,SLOT(on_select(QModelIndex)));

    connect(m_model,SIGNAL(message(QString)),
            this,SIGNAL(message(QString)));

    connect(m_model,SIGNAL(selected(QString,CBoolFormula*)),
            this,SIGNAL(selected(QString,CBoolFormula*)));
}
//------------------------------------------------------------------


CWidgetBFList::~CWidgetBFList()
{
    m_header->~CToolBarHeader();
    m_list->~QListView();
    m_model->~CModelBFList();
    m_actVisible->~QAction();
}
//------------------------------------------------------------------


QString CWidgetBFList::currentBFName() const
{
    return m_model->data(m_list->currentIndex(),Qt::DisplayRole).toString();
}
//------------------------------------------------------------------


QString CWidgetBFList::currentBFDimacs() const
{
    return m_model->data(m_list->currentIndex(),CModelBFList::DimacsRole).toString();
}
//------------------------------------------------------------------


bool CWidgetBFList::rename(const QString &name, const QString &newName)
{
    return m_model->rename(name,newName);
}
//------------------------------------------------------------------


void CWidgetBFList::on_disabledHide(bool disabled)
{
    m_header->actHint()->setDisabled(disabled);
}
//------------------------------------------------------------------

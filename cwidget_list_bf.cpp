#include "cwidget_list_bf.h"

#include <QAction>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>

#include "cbf_view.h"
#include "ctoolbar_header.h"

CWidgetListBF::CWidgetListBF(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("QListWidget{border: 0px solid #000;"
                              "border-width: 0px 1px 0px 1px;}");

    m_header = new CToolBarHeader(tr("Функции"));

    m_header->setStyleSheet("QToolBar {background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #999, stop:1.0 #555);"
                                      "border: 1px solid #000;}"
                            "QLabel   {color: white; font-size: 12px;}");

    m_header->insertStretch(m_header->actHint());

    m_list = new QListWidget();
    m_list->setMinimumWidth(150);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    this->setLayout(vbox);

    vbox->addWidget(m_header);
    vbox->addWidget(m_list);

    connect(m_list,SIGNAL(itemClicked(QListWidgetItem*)),
            this,SLOT(clicked_itemList(QListWidgetItem*)));

    m_actVisible = new QAction(tr("Отображать список функций"),this);
    m_actVisible->setCheckable(true);

    connect(m_header->actHint(),SIGNAL(triggered()),this,SLOT(hide()));
    connect(m_actVisible,SIGNAL(toggled(bool)),this,SLOT(setVisible(bool)));
    connect(m_header->actHint(),SIGNAL(triggered(bool)),m_actVisible,SLOT(setChecked(bool)));
}
//------------------------------------------------------------------


CWidgetListBF::~CWidgetListBF()
{
    qDeleteAll(m_bfv);
}
//------------------------------------------------------------------


CBFView *CWidgetListBF::bfCurrent()
{
    if (m_list->currentItem()) {

        return m_bfv[m_list->currentItem()];
    }

    return NULL;
}
//------------------------------------------------------------------


void CWidgetListBF::on_append(CBFView *bfv)
{
    emit view(bfv);

    if (!unique(bfv))
        return;

    QListWidgetItem *it = new QListWidgetItem(m_list);
    it->setText(bfv->boolFunctionName());
    it->setIcon(QIcon(":/ico/lstbf_item.png"));
    m_list->setCurrentItem(it);
    m_bfv.insert(it,bfv);
}
//------------------------------------------------------------------


void CWidgetListBF::on_remove(CBFView *bfv)
{
    TMapItemBF::iterator it = m_bfv.begin();
    for (;it != m_bfv.end(); ++it)
        if (it.value()->boolFunctionName() == bfv->boolFunctionName()) {

            m_list->takeItem(m_list->row(it.key()));

            delete m_bfv[it.key()];
            m_bfv.remove(it.key());

            break;
        }


    if (m_list->count() > 0) {

        m_list->setCurrentRow(m_list->count() - 1);
        emit send(m_bfv[m_list->currentItem()]);
    }
}
//------------------------------------------------------------------


void CWidgetListBF::on_disabledHide(bool disabled)
{
    m_header->actHint()->setDisabled(disabled);
}
//------------------------------------------------------------------


void CWidgetListBF::on_updateCurrentItem()
{
    QListWidgetItem *item = m_list->currentItem();

    if (item){
        item->setText(m_bfv[item]->boolFunctionName());
        emit send(m_bfv[item]);
        emit view(m_bfv[item]);
    }
}
//------------------------------------------------------------------


bool CWidgetListBF::unique(CBFView *bfv) const
{
    TMapItemBF::const_iterator it = m_bfv.begin();

    for (;it != m_bfv.end(); ++it) {
        if (it.value()->boolFunctionName() == bfv->boolFunctionName())
            return false;
    }

    return true;
}
//------------------------------------------------------------------


void CWidgetListBF::clicked_itemList(QListWidgetItem *item)
{
    emit send(m_bfv[item]);
    emit view(m_bfv[item]);
}
//------------------------------------------------------------------

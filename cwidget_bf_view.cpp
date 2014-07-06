#include "cwidget_bf_view.h"

#include <QAction>
#include <QActionGroup>
#include <QComboBox>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QVBoxLayout>

#include "ctoolbar_header.h"
#include "cbf_view.h"

CWidgetBFView::CWidgetBFView(QWidget *parent) :
    QWidget(parent)
{
    setStyleSheet("QTextEdit {border: 0px;}");

    m_header = new CToolBarHeader();

    m_combo = new QComboBox();
    m_combo->setFixedHeight(m_header->minimumHeight());
    m_combo->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    m_combo->setStyleSheet("QComboBox {background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #999, stop:1.0 #555);"
                        " border-radius: 3px;"
                        "color: #fff;}");
                       /*"QComboBox::drop-down {border: 0px;color: #fff;}"
                       "QComboBox::down-arrow {color: #fff;}");*/

    m_actView = new QAction(QIcon(":/ico/bf_view.png"),CBFView::descriptView,this);
    m_actViewDIMACS = new QAction(QIcon(":/ico/bf_viewdimacs.png"),CBFView::descriptViewDIMACS,this);
    m_actViewDis = new QAction(QIcon(":/ico/bf_viewdis.png"),CBFView::descriptViewDis,this);
    m_actViewDisSort = new QAction(QIcon(":/ico/bf_viewdissort.png"),CBFView::descriptViewDisSort,this);
    m_actViewVar = new QAction(QIcon(":/ico/bf_viewvar.png"),CBFView::descriptViewVar,this);
    m_actViewVarSort = new QAction(QIcon(":/ico/bf_viewvarsort.png"),CBFView::descriptViewVarSort,this);

    m_groupActView = new QActionGroup(this);
    m_groupActView->addAction(m_actView);
    m_groupActView->addAction(m_actViewDIMACS);
    m_groupActView->addAction(m_actViewDis);
    m_groupActView->addAction(m_actViewDisSort);
    m_groupActView->addAction(m_actViewVar);
    m_groupActView->addAction(m_actViewVarSort);

    m_actViewSat = new QAction(QIcon(":/ico/bf_viewsat_add.png"),tr("Отображать общий отчет решения SAT"),this);

    m_actView->setCheckable(true);
    m_actViewDIMACS->setCheckable(true);
    m_actViewDis->setCheckable(true);
    m_actViewDisSort->setCheckable(true);
    m_actViewVar->setCheckable(true);
    m_actViewVarSort->setCheckable(true);
    m_actViewSat->setCheckable(true);

    m_actView->setChecked(true);
    m_actViewSat->setChecked(true);

    m_header->insertWidget(m_header->actHint(),m_combo);
    m_header->insertSpace(m_header->actHint(),16);
    m_header->insertActions(m_header->actHint(),m_groupActView->actions());
    m_header->insertAction(m_header->actHint(),m_actViewSat);
    m_header->insertStretch(m_header->actHint());

    m_edit = new QTextEdit();
    m_edit->setFont(QFont("Arial",12));
    m_edit->setReadOnly(true);
    m_edit->document()->setDocumentMargin(10);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    this->setLayout(vbox);

    vbox->setMenuBar(m_header);
    vbox->addWidget(m_edit);
    vbox->setStretch(1,100);

    on_view(m_combo->currentText());

    connect(m_header->actHint(),SIGNAL(triggered()),this,SLOT(triggered_actHide()));
    connect(m_actView,SIGNAL(toggled(bool)),this,SLOT(on_view(bool)));
    connect(m_actViewDIMACS,SIGNAL(toggled(bool)),this,SLOT(on_view(bool)));
    connect(m_actViewDis,SIGNAL(toggled(bool)),this,SLOT(on_view(bool)));
    connect(m_actViewDisSort,SIGNAL(toggled(bool)),this,SLOT(on_view(bool)));
    connect(m_actViewVar,SIGNAL(toggled(bool)),this,SLOT(on_view(bool)));
    connect(m_actViewVarSort,SIGNAL(toggled(bool)),this,SLOT(on_view(bool)));
    connect(m_actViewSat,SIGNAL(toggled(bool)),this,SLOT(on_view()));
    connect(m_combo,SIGNAL(currentTextChanged(QString)),this,SLOT(on_view(QString)));
}
//------------------------------------------------------------------


void CWidgetBFView::on_view(CBFView *bfv)
{
    if (!bfv) {
        on_view(m_combo->currentText());
        return;
    }

    TStr name = bfv->boolFunctionName();
    if (name.isEmpty()){
        on_view(m_combo->currentText());
        return;
    }

    if (m_combo->count() == 0) {
        m_bfv.insert(name,bfv);
        m_combo->addItem(name);
        m_edit->setVisible(true);
        m_groupActView->setEnabled(true);
        m_actViewSat->setEnabled(true);
    } else if (m_combo->findText(name) != -1) {
        m_bfv[name] = bfv;
        bool isUpdate = name == m_combo->currentText() ? true : false;
        m_combo->setCurrentText(name);
        if (isUpdate)
            on_view(name);
    } else {
        m_bfv.insert(name,bfv);
        m_combo->addItem(name);
        m_combo->setCurrentText(name);
    }
}
//------------------------------------------------------------------


void CWidgetBFView::on_viewSat(CBFView *bfv)
{
    m_actView->setChecked(true);
    m_actViewSat->setChecked(true);
    on_view(bfv);
}
//------------------------------------------------------------------


void CWidgetBFView::on_remove(const QString &name)
{
    m_bfv.remove(name);
    m_combo->removeItem(m_combo->findText(name));
}
//------------------------------------------------------------------


void CWidgetBFView::triggered_actHide()
{
    on_remove(m_combo->currentText());
}
//------------------------------------------------------------------


void CWidgetBFView::on_view(const QString &name)
{
    if (m_bfv.find(name) == m_bfv.end()) {
        m_edit->clear();
        m_edit->setVisible(false);
        m_groupActView->setEnabled(false);
        m_actViewSat->setEnabled(false);
        return;
    }

    TStr html = "";

    if (m_actView->isChecked()) {
        html += m_bfv[name]->view();
        html += "<p>&nbsp;</p>";
    }

    if (m_actViewSat->isChecked()) {
        html += m_bfv[name]->viewSat();
        html += "<p>&nbsp;</p>";
    }

    if (m_actViewDIMACS->isChecked()){

        m_edit->setPlainText(m_bfv[name]->viewDIMACS());
        return;

    } else if (m_actViewDis->isChecked()){

        html += m_bfv[name]->viewDis();

    } else if (m_actViewDisSort->isChecked()){

        html += m_bfv[name]->viewDisSort();

    } else if (m_actViewVar->isChecked()) {

        html += m_bfv[name]->viewVar();

    } else if (m_actViewVarSort->isChecked()) {

        html += m_bfv[name]->viewVarSort();
    }

    m_edit->setHtml(html);
}
//------------------------------------------------------------------


void CWidgetBFView::on_view(bool toggel)
{
    if (toggel)
        on_view(m_combo->currentText());
}
//------------------------------------------------------------------


void CWidgetBFView::on_view()
{
    on_view(m_combo->currentText());
}
//------------------------------------------------------------------






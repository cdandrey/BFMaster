#include <QAction>
#include <QLabel>
#include <QPlainTextEdit>
#include <QVBoxLayout>

#include "cwidget_consol.h"
#include "ctoolbar_header.h"

CWidgetConsol::CWidgetConsol(QWidget *parent) :
    QWidget(parent)
{
    m_header = new CToolBarHeader(tr("Консоль"));

    m_header->insertStretch(m_header->actHint());

    m_actVisible = new QAction(tr("Отображать консоль"),this);
    m_actVisible->setCheckable(true);


    m_edit = new QPlainTextEdit();
    m_edit->setFont(QFont("Courier",12));
    m_edit->setReadOnly(true);
    m_edit->document()->setDocumentMargin(10);
    m_edit->setMaximumBlockCount(1000);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setMargin(0);
    vbox->setSpacing(0);
    this->setLayout(vbox);

    vbox->setMenuBar(m_header);
    vbox->addWidget(m_edit);

    connect(m_header->actHint(),SIGNAL(triggered(bool)),this,SLOT(hide()));
    connect(m_actVisible,SIGNAL(toggled(bool)),this,SLOT(setVisible(bool)));
    connect(m_actVisible,SIGNAL(toggled(bool)),m_actVisible,SLOT(setChecked(bool)));
    connect(m_header->actHint(),SIGNAL(triggered(bool)),m_actVisible,SLOT(setChecked(bool)));
}
//------------------------------------------------------------------


void CWidgetConsol::messageAppend(const QString &message)
{
    this->setVisible(true);
    m_edit->appendPlainText(message);
}
//------------------------------------------------------------------


void CWidgetConsol::messageSet(const QString &message)
{
    this->setVisible(true);
    m_edit->setPlainText(message);
}
//------------------------------------------------------------------


void CWidgetConsol::executingOperation(const QString &operation)
{
    if (operation == "") {
        m_header->setTitle(tr("Консоль"));
    } else {
        m_header->setTitle(operation);
    }
}
//------------------------------------------------------------------


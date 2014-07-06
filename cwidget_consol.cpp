#include "cwidget_consol.h"

#include <QAction>
#include <QLabel>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QVBoxLayout>

#include "ctoolbar_header.h"

CWidgetConsol::CWidgetConsol(QWidget *parent) :
    QWidget(parent)
{
    setStyleSheet("QPlainTextEdit {border: 0px solid #000;"
                                  "border-width: 0px 0px 0px 1px;}");

    m_header = new CToolBarHeader(tr("Консоль"));
    m_header->setStyleSheet("QToolBar {background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #999, stop:1.0 #555);"
                                      "border: 0px solid #000;"
                                      "border-width: 1px 0px 1px 1px;}"
                            "QLabel   {color: white; font-size: 12px;}");

    m_header->insertStretch(m_header->actHint());

    m_actVisible = new QAction(tr("Отображать консоль"),this);
    m_actVisible->setCheckable(true);


    m_edit = new QPlainTextEdit();
    m_edit->setFont(QFont("Courier",12));
    m_edit->setReadOnly(true);
    m_edit->setLineWrapMode(QPlainTextEdit::NoWrap);
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
    m_edit->appendPlainText(message);
}
//------------------------------------------------------------------


void CWidgetConsol::messageSet(const QString &message)
{
    m_edit->setPlainText(message);
}
//------------------------------------------------------------------


void CWidgetConsol::executingOperation(const QString &operation)
{
    if (operation == "finished") {
        m_header->setTitle(tr("Консоль"));
    } else {
        m_header->setTitle(operation);
    }
}
//------------------------------------------------------------------


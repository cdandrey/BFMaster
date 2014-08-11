#include "ctoolbar_header.h"

#include <QAction>
#include <QLabel>

CToolBarHeader::CToolBarHeader(QString title,QWidget *parent) :
    QToolBar(parent)
{
    setIconSize(QSize(16,16));
    setMinimumHeight(28);
    setMovable(false);
    setStyleSheet("QToolBar {background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #999, stop:1.0 #555);"
                  "border-width: 1px 0px 1px 0px;"
                  "border-style: solid;"
                  "border-color: #000;}"
                  "QLabel{color: white; font-size: 12px;}"
                  "QToolButton:hover {"
                  "border: 0px;"
                  "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #bbb, stop:1.0 #777);}"
                  );

    m_label = new QLabel(title);
    m_label->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_label->setMargin(5);

    m_actHint = new QAction(QIcon(":/ico/wd_hint_d.png"),tr("Закрыть окно"),this);

    addWidget(m_label);
    addAction(m_actHint);
}
//------------------------------------------------------------------


void CToolBarHeader::insertSpace(QAction *before,int width)
{
    QWidget *space = new QWidget();
    space->setFixedWidth(width);
    space->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);

    insertWidget(before,space);
}
//------------------------------------------------------------------


void CToolBarHeader::insertStretch(QAction *before)
{
    QWidget *stretch = new QWidget();
    stretch->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    insertWidget(before,stretch);
}
//------------------------------------------------------------------


QString CToolBarHeader::title() const
{
    return m_label->text();
}
//------------------------------------------------------------------


void CToolBarHeader::setTitle(const QString &title)
{
    m_label->setText(title);
}
//------------------------------------------------------------------

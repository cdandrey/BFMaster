#include <QAction>
#include <QLabel>

#include "ctoolbar_header.h"

CToolBarHeader::CToolBarHeader(QString title,QWidget *parent) :
    QToolBar(parent)
{
    setIconSize(QSize(16,16));
    setMinimumHeight(24);
    setMaximumHeight(24);
    setMovable(false);

    m_labelTitle = new QLabel(title);
    m_labelTitle->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    m_labelTitle->setMargin(5);

    m_actHint = new QAction(QIcon(":/ico/wd_hint_d.png"),tr("Закрыть окно"),this);

    addWidget(m_labelTitle);
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
    return m_labelTitle->text();
}
//------------------------------------------------------------------


void CToolBarHeader::setTitle(const QString &title)
{
    m_labelTitle->setText(title);
}
//------------------------------------------------------------------

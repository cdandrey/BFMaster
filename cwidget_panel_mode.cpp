#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

#include "ctoolbar_header.h"
#include "cwidget_panel_mode.h"

CWidgetPanelMode::CWidgetPanelMode(QWidget *parent) :
    QFrame(parent)
{
    this->setFixedWidth(88);

    m_header = new CToolBarHeader("",this);
    m_header->actHint()->setEnabled(false);
    m_header->actHint()->setVisible(false);

    m_modeToolBar = new QToolBar(this);
    m_modeToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_modeToolBar->setOrientation(Qt::Vertical);
    m_modeToolBar->setMovable(false);
    m_modeToolBar->setIconSize(QSize(80,32));
    m_modeToolBar->setFont(QFont("Tahoma",7,QFont::Bold));

    m_actModeRand = new QAction(QIcon("://ico/ctrl_rand.png"),tr("Функции"),this);
    m_actModeEdit = new QAction(QIcon("://ico/ctrl_edit.png"),tr("Редактор"),this);
    m_actModeCalc = new QAction(QIcon("://ico/ctrl_calc.png"),tr("Калькулятор"),this);
    m_actModeTest = new QAction(QIcon("://ico/ctrl_test.png"),tr("Тестирование"),this);

    m_actModeRand->setCheckable(true);
    m_actModeEdit->setCheckable(true);
    m_actModeCalc->setCheckable(true);
    m_actModeTest->setCheckable(true);

    QActionGroup *group = new QActionGroup(this);
    group->addAction(m_actModeRand);
    group->addAction(m_actModeEdit);
    group->addAction(m_actModeCalc);
    group->addAction(m_actModeTest);

    m_modeToolBar->addActions(group->actions());

    m_runToolBar = new QToolBar(this);
    m_runToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_runToolBar->setOrientation(Qt::Vertical);
    m_runToolBar->setMovable(false);
    m_runToolBar->setIconSize(QSize(80,32));
    m_runToolBar->setFont(QFont("Tahoma",7,QFont::Bold));

    m_actRun = new QAction(QIcon(":/ico/ctrl_run.png"),tr("Выполнить"),this);
    m_actRun->setShortcut(QString("F10"));
    m_actRunLog = new QAction(QIcon(":/ico/ctrl_runlog.png"),tr("Выполнить\n с логом"),this);
    m_actRunLog->setShortcut(QString("F11"));

    m_runToolBar->addSeparator();
    m_runToolBar->addAction(m_actRun);
    m_runToolBar->addAction(m_actRunLog);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setMargin(0);
    vbox->setSpacing(0);
    vbox->addWidget(m_header);
    vbox->addWidget(m_modeToolBar);
    vbox->addStretch(100);
    vbox->addWidget(m_runToolBar);

    this->setLayout(vbox);

    connect(m_actModeRand,SIGNAL(toggled(bool)),this,SIGNAL(toggledRand(bool)));
    connect(m_actModeEdit,SIGNAL(toggled(bool)),this,SIGNAL(toggledEdit(bool)));
    connect(m_actModeCalc,SIGNAL(toggled(bool)),this,SIGNAL(toggledCalc(bool)));
    connect(m_actModeTest,SIGNAL(toggled(bool)),this,SIGNAL(toggledTest(bool)));
    connect(m_actRun,SIGNAL(triggered()),this,SIGNAL(triggeredRun()));
    connect(m_actRunLog,SIGNAL(triggered()),this,SIGNAL(triggeredRunLog()));
}
//------------------------------------------------------------------


void CWidgetPanelMode::on_checkedRand()
{
    m_actModeRand->setChecked(true);
}
//------------------------------------------------------------------


void CWidgetPanelMode::on_checkedEdit()
{
    m_actModeEdit->setChecked(true);
}
//------------------------------------------------------------------


void CWidgetPanelMode::on_checkedCalc()
{
    m_actModeCalc->setChecked(true);
}
//------------------------------------------------------------------


void CWidgetPanelMode::on_checkedTest()
{
    m_actModeTest->setChecked(true);
}
//------------------------------------------------------------------


void CWidgetPanelMode::on_locked()
{
    m_modeToolBar->setEnabled(false);
    m_runToolBar->setEnabled(false);
}
//------------------------------------------------------------------


void CWidgetPanelMode::on_unlocked()
{
    m_modeToolBar->setEnabled(true);
    m_runToolBar->setEnabled(true);
}
//------------------------------------------------------------------


void CWidgetPanelMode::on_runLocked()
{
    m_modeToolBar->setEnabled(false);
    m_actRunLog->setEnabled(false);
    m_actRun->setIcon(QIcon(":/ico/ctrl_stop.png"));
    disconnect(m_actRun,SIGNAL(triggered()),this,SIGNAL(triggeredRun()));
    connect(m_actRun,SIGNAL(triggered()),this,SIGNAL(triggeredStopRun()));
}
//------------------------------------------------------------------


void CWidgetPanelMode::on_runUnlocked()
{
    m_modeToolBar->setEnabled(true);
    m_actRunLog->setEnabled(true);
    m_actRun->setIcon(QIcon(":/ico/ctrl_run.png"));
    disconnect(m_actRun,SIGNAL(triggered()),this,SIGNAL(triggeredStopRun()));
    connect(m_actRun,SIGNAL(triggered()),this,SIGNAL(triggeredRun()));
}
//------------------------------------------------------------------

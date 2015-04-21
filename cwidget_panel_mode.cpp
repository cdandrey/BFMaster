#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

#include "ctoolbar_header.h"
#include "cwidget_panel_mode.h"

const char CWidgetPanelMode::IconActRun[] = "://ico/ctrl_run.png";
const char CWidgetPanelMode::IconActRunLog[] = "://ico/ctrl_runlog.png";
const char CWidgetPanelMode::IconActStop[] = ":/ico/ctrl_stop.png";
const char CWidgetPanelMode::IconActStopLog[] = ":/ico/ctrl_stoplog.png";

const char CWidgetPanelMode::TextActRun[] = "Выполнить";
const char CWidgetPanelMode::TextActRunLog[] = "Выполнить\n с логом";
const char CWidgetPanelMode::TextActStop[] = "Остановить";

CWidgetPanelMode::CWidgetPanelMode(QWidget *parent) :
    QFrame(parent),
    m_isTriggeredRun(false),
    m_isTriggeredRunLog(false)
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
    //m_actModeEdit->setCheckable(true);
    //m_actModeCalc->setCheckable(true);
    //m_actModeTest->setCheckable(true);

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

    m_actRun = new QAction(QIcon(IconActRun),tr(TextActRun),this);
    m_actRun->setShortcut(QString("F10"));
    m_actRunLog = new QAction(QIcon(IconActRunLog),tr(TextActRunLog),this);
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
    connect(m_actRun,SIGNAL(triggered()),this,SLOT(triggered_Run()));
    connect(m_actRunLog,SIGNAL(triggered()),this,SLOT(triggered_RunLog()));
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


void CWidgetPanelMode::on_changeRunStop(bool change)
{
    if (change) {
        m_actRun->setIcon(QIcon(IconActStop));
        m_actRun->setText(tr(TextActRun));
        disconnect(m_actRun,SIGNAL(triggered()),this,SLOT(triggered_Run()));
        connect(m_actRun,SIGNAL(triggered()),this,SIGNAL(terminate()));
    } else {
        m_actRun->setIcon(QIcon(IconActRun));
        m_actRun->setText(tr(TextActRun));
        connect(m_actRun,SIGNAL(triggered()),this,SLOT(triggered_Run()));
        disconnect(m_actRun,SIGNAL(triggered()),this,SIGNAL(terminate()));
    }
}
//------------------------------------------------------------------


void CWidgetPanelMode::on_changeRunLogStop(bool change)
{
    if (change) {
        m_actRunLog->setIcon(QIcon(IconActStopLog));
        m_actRunLog->setText(tr(TextActStop));
        disconnect(m_actRunLog,SIGNAL(triggered()),this,SLOT(triggered_RunLog()));
        connect(m_actRunLog,SIGNAL(triggered()),this,SIGNAL(terminate()));
    } else {
        m_actRunLog->setIcon(QIcon(IconActRunLog));
        m_actRunLog->setText(tr(TextActRunLog));
        connect(m_actRunLog,SIGNAL(triggered()),this,SLOT(triggered_RunLog()));
        disconnect(m_actRunLog,SIGNAL(triggered()),this,SIGNAL(terminate()));
    }
}
//------------------------------------------------------------------


void CWidgetPanelMode::on_resetTriggered()
{
    m_isTriggeredRun = false;
    m_isTriggeredRunLog = false;
}
//------------------------------------------------------------------


void CWidgetPanelMode::triggered_Run()
{
    m_isTriggeredRun = true;
    m_isTriggeredRunLog = false;
    emit run();
}
//------------------------------------------------------------------


void CWidgetPanelMode::triggered_RunLog()
{
    m_isTriggeredRun = false;
    m_isTriggeredRunLog = true;
    emit runLog();
}
//------------------------------------------------------------------


void CWidgetPanelMode::on_locked()
{
    m_modeToolBar->setEnabled(false);
    m_actRun->setEnabled(m_isTriggeredRun);
    m_actRunLog->setEnabled(m_isTriggeredRunLog);
    on_changeRunStop(m_isTriggeredRun);
    on_changeRunLogStop(m_isTriggeredRunLog);
}
//------------------------------------------------------------------


void CWidgetPanelMode::on_unlocked()
{
    m_modeToolBar->setEnabled(true);
    m_actRun->setEnabled(true);
    m_actRunLog->setEnabled(true);
    on_changeRunStop(false);
    on_changeRunLogStop(false);
    on_resetTriggered();
}
//------------------------------------------------------------------

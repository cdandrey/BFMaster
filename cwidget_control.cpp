#include "cwidget_control.h"
#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <QVBoxLayout>

#include "ctoolbar_header.h"

CWidgetControl::CWidgetControl(QWidget *parent)
    : QWidget(parent)
{
    m_actBFRand = new QAction(QIcon(":/ico/ctrl_rand.png"),tr("Функции"),this);
    m_actBFEdit = new QAction(QIcon(":/ico/ctrl_edit.png"),tr("Редактор"),this);
    m_actBFCalc = new QAction(QIcon(":/ico/ctrl_calc.png"),tr("Калькулятор"),this);
    m_actBFTest = new QAction(QIcon(":/ico/ctrl_test.png"),tr("Тестирование"),this);

    m_actBFRand->setCheckable(true);
    m_actBFEdit->setCheckable(true);
    m_actBFCalc->setCheckable(true);
    m_actBFTest->setCheckable(true);

    QActionGroup *group = new QActionGroup(this);
    group->addAction(m_actBFRand);
    group->addAction(m_actBFEdit);
    group->addAction(m_actBFCalc);
    group->addAction(m_actBFTest);

    m_actRun = new QAction(QIcon(":/ico/ctrl_run.png"),tr("Выполнить"),this);
    m_actRun->setShortcut(QString("F10"));
    m_actRunLog = new QAction(QIcon(":/ico/ctrl_runlog.png"),tr("Выполнить с логом"),this);
    m_actRunLog->setShortcut(QString("F11"));

    m_header = new CToolBarHeader();
    m_header->setStyleSheet("QToolBar {background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #999, stop:1.0 #555);"
                            "border: 0px solid #000;"
                            "border-width: 1px 0px 1px 0px}");

    m_header->actHint()->setEnabled(false);
    m_header->actHint()->setVisible(false);

    m_stateToolBar = new QToolBar();
    m_stateToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_stateToolBar->setOrientation(Qt::Vertical);
    m_stateToolBar->setMovable(false);
    m_stateToolBar->setIconSize(QSize(80,32));
    m_stateToolBar->setFont(QFont("Tahoma",7,QFont::Bold));

    m_stateToolBar->addAction(m_actBFRand);
    m_stateToolBar->addAction(m_actBFEdit);
    m_stateToolBar->addAction(m_actBFCalc);
    m_stateToolBar->addAction(m_actBFTest);

    m_runToolBar = new QToolBar();
    m_runToolBar->setOrientation(Qt::Vertical);
    m_runToolBar->setMovable(false);
    m_runToolBar->setIconSize(QSize(80,32));

    m_runToolBar->addSeparator();
    m_runToolBar->addAction(m_actRun);
    m_runToolBar->addAction(m_actRunLog);

    QVBoxLayout *vbox = new QVBoxLayout();
    this->setLayout(vbox);

    vbox->setMenuBar(m_header);
    vbox->addWidget(m_stateToolBar);
    vbox->addStretch(100);
    vbox->addWidget(m_runToolBar);
    vbox->setMargin(0);
    vbox->setSpacing(0);

    connect(m_actBFRand,SIGNAL(toggled(bool)),this,SIGNAL(toggledRand(bool)));
    connect(m_actBFEdit,SIGNAL(toggled(bool)),this,SIGNAL(toggledEdit(bool)));
    connect(m_actBFCalc,SIGNAL(toggled(bool)),this,SIGNAL(toggledCalc(bool)));
    connect(m_actBFTest,SIGNAL(toggled(bool)),this,SIGNAL(toggledTest(bool)));
    connect(m_actRun,SIGNAL(triggered()),this,SIGNAL(triggeredRun()));
    connect(m_actRunLog,SIGNAL(triggered()),this,SIGNAL(triggeredRunLog()));
}
//------------------------------------------------------------------


void CWidgetControl::on_checkedRand()
{
    m_actBFRand->setChecked(true);
}
//------------------------------------------------------------------


void CWidgetControl::on_checkedEdit()
{
    m_actBFEdit->setChecked(true);
}
//------------------------------------------------------------------


void CWidgetControl::on_checkedCalc()
{
    m_actBFCalc->setChecked(true);
}
//------------------------------------------------------------------


void CWidgetControl::on_checkedTest()
{
    m_actBFTest->setChecked(true);
}
//------------------------------------------------------------------


void CWidgetControl::on_locked()
{
    m_stateToolBar->setEnabled(false);
    m_runToolBar->setEnabled(false);
}
//------------------------------------------------------------------


void CWidgetControl::on_unlocked()
{
    m_stateToolBar->setEnabled(true);
    m_runToolBar->setEnabled(true);
}
//------------------------------------------------------------------


void CWidgetControl::on_runLocked()
{
    m_stateToolBar->setEnabled(false);
    m_actRunLog->setEnabled(false);
    m_actRun->setIcon(QIcon(":/ico/ctrl_stop.png"));
    disconnect(m_actRun,SIGNAL(triggered()),this,SIGNAL(triggeredRun()));
    connect(m_actRun,SIGNAL(triggered()),this,SIGNAL(triggeredStopRun()));
}
//------------------------------------------------------------------


void CWidgetControl::on_runUnlocked()
{
    m_stateToolBar->setEnabled(true);
    m_actRunLog->setEnabled(true);
    m_actRun->setIcon(QIcon(":/ico/ctrl_run.png"));
    disconnect(m_actRun,SIGNAL(triggered()),this,SIGNAL(triggeredStopRun()));
    connect(m_actRun,SIGNAL(triggered()),this,SIGNAL(triggeredRun()));
}
//------------------------------------------------------------------

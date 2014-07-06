#include <QtDebug>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QScrollBar>
#include <QSplitter>
#include <QTextStream>
#include <QTimerEvent>

#include "cbf_generate.h"
#include "cbf_view.h"
#include "csat.h"
#include "cwidget_bf_generate.h"
#include "cwidget_bf_view.h"
#include "cwidget_consol.h"
#include "cwidget_control.h"
#include "cwidget_list_bf.h"
#include "cwidget_tree_sat.h"
#include "cwindow_main.h"

CWindowMain::CWindowMain(QWidget *parent)
    : QMainWindow(parent)
{
    setStyleSheet("QSplitter::handle:vertical {height: 0px;}"
                  "QSplitter::handle:horizontal {width: 0px;}");

    m_widgetBFGenerate  = new CWidgetBFGenerate(this);
    m_widgetBFView      = new CWidgetBFView(this);
    m_widgetConsol      = new CWidgetConsol(this);
    m_widgetConsol->setVisible(false);
    m_widgetControl     = new CWidgetControl(this);
    m_widgetListBF      = new CWidgetListBF(this);
    m_widgetTreeSat     = new CWidgetTreeSat(this);

    QWidget *wBFViewBFGenerate = new QWidget();
    QVBoxLayout *boxBFViewBFGenerate = new QVBoxLayout();
    boxBFViewBFGenerate->addWidget(m_widgetBFGenerate);
    boxBFViewBFGenerate->addWidget(m_widgetBFView);
    boxBFViewBFGenerate->setMargin(0);
    boxBFViewBFGenerate->setSpacing(0);
    boxBFViewBFGenerate->setStretch(1,100);
    wBFViewBFGenerate->setLayout(boxBFViewBFGenerate);

    QSplitter *spListBFTreeAlg = new QSplitter(Qt::Vertical);
    spListBFTreeAlg->addWidget(m_widgetListBF);
    spListBFTreeAlg->addWidget(m_widgetTreeSat);

    QSplitter *spTreeView = new QSplitter(Qt::Horizontal);
    spTreeView->addWidget(spListBFTreeAlg);
    spTreeView->addWidget(wBFViewBFGenerate);
    spTreeView->setStretchFactor(1,100);

    QSplitter *spMain = new QSplitter(Qt::Vertical);
    spMain->addWidget(spTreeView);
    spMain->addWidget(m_widgetConsol);
    spMain->setStretchFactor(0,100);

    QWidget *widgetCenter = new QWidget(this);
    setCentralWidget(widgetCenter);

    QHBoxLayout *hbox = new QHBoxLayout();
    centralWidget()->setLayout(hbox);

    hbox->addWidget(m_widgetControl);
    hbox->addWidget(spMain);
    hbox->setMargin(0);
    hbox->setSpacing(0);
    hbox->setStretch(1,100);

    m_actOpen = new QAction(QIcon(":/ico/main_open.png"),tr("&Открыть"),this);
    m_actOpen->setShortcut(QKeySequence::Open);
    connect(m_actOpen,SIGNAL(triggered()),this,SLOT(on_open()));

    m_actSave = new QAction(QIcon(":/ico/main_save.png"),tr("&Сохранить"),this);
    m_actSave->setShortcut(QKeySequence::Save);
    connect(m_actSave,SIGNAL(triggered()),this,SLOT(on_save()));

    m_actSaveAs = new QAction(QIcon(":/ico/main_save_as.png"),tr("&Сохранить как"),this);
    m_actSaveAs->setShortcut(QKeySequence::SaveAs);
    connect(m_actSaveAs,SIGNAL(triggered()),this,SLOT(on_save_as()));

    mainMenu = menuBar()->addMenu(tr("Файл"));
    mainMenu->addAction(m_actOpen);
    mainMenu->addAction(m_actSave);
    mainMenu->addAction(m_actSaveAs);

    mainMenu = menuBar()->addMenu(tr("Вид"));
    mainMenu->addAction(m_widgetBFGenerate->actVisible());
    mainMenu->addAction(m_widgetConsol->actVisible());

    mainMenu = menuBar()->addMenu(tr("Операции"));
    mainMenu = menuBar()->addMenu(tr("Помощь"));

    m_timerGenerate = 0;
    m_timerRunSat = 0;

    // m_widgetControl
    connect(m_widgetControl,SIGNAL(triggeredRun()),
            this,SLOT(on_satCheckedRun()));
    connect(m_widgetControl,SIGNAL(triggeredRunLog()),
            this,SLOT(on_satCheckedRunLog()));

    connect(m_widgetControl,SIGNAL(toggledRand(bool)),
            m_widgetListBF,SLOT(setVisible(bool)));
    connect(m_widgetControl,SIGNAL(toggledRand(bool)),
            m_widgetListBF,SLOT(on_disabledHide(bool)));

    connect(m_widgetControl,SIGNAL(toggledRand(bool)),
            m_widgetBFGenerate,SLOT(setVisible(bool)));
    connect(m_widgetControl,SIGNAL(toggledRand(bool)),
            m_widgetBFGenerate->actVisible(),SLOT(setChecked(bool)));
    connect(m_widgetControl,SIGNAL(toggledRand(bool)),
            m_widgetBFGenerate->actVisible(),SLOT(setEnabled(bool)));

    connect(m_widgetControl,SIGNAL(toggledRand(bool)),
            m_widgetTreeSat,SLOT(setVisible(bool)));
    connect(m_widgetControl,SIGNAL(toggledRand(bool)),
            m_widgetTreeSat,SLOT(on_disabledHide(bool)));

    // m_widgetListBF
    connect(m_widgetListBF,SIGNAL(send(CBFView*)),
            m_widgetBFGenerate,SLOT(on_set(CBFView*)));

    connect(m_widgetListBF,SIGNAL(view(CBFView*)),
            m_widgetBFView,SLOT(on_view(CBFView*)));

    // m_widgetBFGenerate
    connect(m_widgetBFGenerate,SIGNAL(generate(CBFView*)),
            this,SLOT(on_generateStarted(CBFView*)));

    connect(m_widgetBFGenerate,SIGNAL(remove(CBFView*)),
            m_widgetListBF,SLOT(on_remove(CBFView*)));

    connect(m_widgetBFGenerate,SIGNAL(remove(QString)),
            m_widgetBFView,SLOT(on_remove(QString)));

    // m_widgetTreeSat
    connect(m_widgetTreeSat,SIGNAL(run(QString)),this,SLOT(on_satRun(QString)));

    // main
    connect(this,SIGNAL(messageAppend(QString)),
            m_widgetConsol,SLOT(messageAppend(QString)));

    connect(this,SIGNAL(messageSet(QString)),
            m_widgetConsol,SLOT(messageSet(QString)));

    connect(this,SIGNAL(executingOperation(QString)),
            m_widgetConsol,SLOT(executingOperation(QString)));

    m_widgetControl->on_checkedRand();
}
//-------------------------------------------------------------------


CWindowMain::~CWindowMain()
{
}
//-------------------------------------------------------------------


void CWindowMain::on_open()
{
    TLStr lstFiles =  QFileDialog::getOpenFileNames(this,tr("Открыть функцию"),"",
                                                    tr("Булева функция(*.%1)").arg(CBFView::suffix()));

    foreach (TStr fileName, lstFiles) {

        QFile file(fileName);
        QTextStream stream(&file);

        if (!file.open(QIODevice::ReadOnly)){
            emit messageAppend(TStr("%1 : %2")
                               .arg(tr("Невозможно открыть файл"))
                               .arg(fileName));
            continue;
        }

        QRegExp regDIMACS("^\\s*p\\s+cnf\\s+\\d+\\s+\\d+\\s*$");
        regDIMACS.setCaseSensitivity(Qt::CaseInsensitive);

        TStr line = "";
        int pos = -1;

        while (pos == -1 && !stream.atEnd()) {
            line = stream.readLine();
            pos = regDIMACS.indexIn(line);
        }

        if (pos == -1) {
            emit messageAppend(TStr("%1 : %2")
                               .arg(tr("ОШИБКА! Неизвестный формат файла"))
                               .arg(fileName));
            continue;
        }

        TLLst cnf;

        QRegExp regDigit("\\d+|-\\d+");

        while (!stream.atEnd()) {

            line = stream.readLine();

            int pos = 0;
            TLst icnf;
            while (((pos = regDigit.indexIn(line, pos)) != -1)
                   && (regDigit.cap(0) != "0"))
            {
                icnf << regDigit.cap(0).toInt();
                pos += regDigit.matchedLength();
            }

            if (icnf.size() > 1)
                cnf << icnf;
            else
                emit messageAppend(TStr("%1 : %2")
                                   .arg(tr("ОШИБКА! Дизъюнкт сожержит меньше двух переменных."))
                                   .arg(fileName));
        }

        if (cnf.isEmpty())
            continue;

        CBoolFunction *bf = new CBoolFunction(cnf);
        QFileInfo fileInfo(file);
        CBFView *bfv = new CBFView(bf,fileInfo.baseName(),fileInfo.absolutePath());

        m_widgetListBF->on_append(bfv);
        m_widgetBFGenerate->on_set(bfv);

        file.close();
    }
}
//------------------------------------------------------------------


void CWindowMain::on_save()
{
    CBFView *bfv = m_widgetListBF->bfCurrent();

    if (bfv == NULL)
        return;

    TStr fileName = "";
    if (bfv->pathFile().isEmpty()){
        fileName = QFileDialog::getSaveFileName(this,tr("Сохранить функцию"),bfv->boolFunctionName(),
                                                tr("Булева функция(*.%1)").arg(CBFView::suffix()));
    } else {
        fileName = bfv->absoluteFileName();
    }

    on_save(fileName,bfv);
}
//------------------------------------------------------------------


void CWindowMain::on_save_as()
{
    CBFView *bfv = m_widgetListBF->bfCurrent();

    TStr fileName = "";
    fileName = QFileDialog::getSaveFileName(this,tr("Сохранить функцию"),bfv->boolFunctionName(),
                                                tr("Булева функция(*.%1)").arg(CBFView::suffix()));
    on_save(fileName,bfv);
}
//------------------------------------------------------------------


void CWindowMain::on_save(const TStr &fileName, CBFView *bfv)
{
    QFile file(fileName);
    QTextStream stream(&file);

    if (!file.open(QIODevice::WriteOnly)){
        emit messageAppend(TStr("%1 : %2")
                           .arg(tr("Невозможно открыть файл"))
                           .arg(fileName));
        return;
    }

    stream << bfv->viewDIMACS();
    QFileInfo fileInfo(file);
    bfv->setBoolFunctionName(fileInfo.baseName());
    bfv->setPathFile(fileInfo.absolutePath());
    m_widgetListBF->on_updateCurrentItem();

    file.close();
}
//------------------------------------------------------------------


void CWindowMain::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerGenerate) {

        emit executingOperation(m_bfGenerate->progressDescription());
        emit messageAppend(m_bfGenerate->progress());

    } else if (event->timerId() == m_timerRunSat) {

        emit executingOperation(m_sat->progressDescription());
        emit messageAppend(m_sat->progress());
    }
}
//------------------------------------------------------------------


void CWindowMain::on_generateFinished()
{
    killTimer(m_timerGenerate);
    m_timerGenerate = 0;

    emit executingOperation(tr("finished"));
    emit messageAppend(m_bfGenerate->progressFinished());
}
//------------------------------------------------------------------


void CWindowMain::on_generateStarted(CBFView *bfv)
{
    m_bfGenerate = new CBFGenerate(bfv);

    if (m_bfGenerate == NULL)
        return;

    connect(m_bfGenerate,SIGNAL(started()),m_widgetControl,SLOT(on_locked()));
    connect(m_bfGenerate,SIGNAL(finished()),m_widgetControl,SLOT(on_unlocked()));

    connect(m_bfGenerate,SIGNAL(started()),m_widgetTreeSat,SLOT(on_locked()));
    connect(m_bfGenerate,SIGNAL(finished()),m_widgetTreeSat,SLOT(on_unlocked()));
    connect(m_bfGenerate,SIGNAL(finished()),m_widgetTreeSat,SLOT(on_runNextChecked()));

    connect(m_bfGenerate,SIGNAL(started()),m_widgetBFGenerate,SLOT(on_generateLocked()));
    connect(m_bfGenerate,SIGNAL(finished()),m_widgetBFGenerate,SLOT(on_generateUnlocked()));

    connect(m_widgetBFGenerate,SIGNAL(stopped()),m_bfGenerate,SLOT(on_stopped()));

    connect(m_bfGenerate,SIGNAL(finished()),
            this,SLOT(on_generateFinished()));
    connect(m_bfGenerate,SIGNAL(successfull(CBFView*)),
            m_widgetListBF,SLOT(on_append(CBFView*)));
    connect(m_bfGenerate,SIGNAL(finished()),m_bfGenerate,SLOT(deleteLater()));

    m_widgetConsol->actVisible()->toggled(true);

    emit executingOperation(m_bfGenerate->progressDescription());

    int sizeTask = bfv->boolFunction()->numX() * bfv->boolFunction()->numDis();
    if (sizeTask < 1000)
        m_timerGenerate = startTimer(1);
    else if (sizeTask < 50000)
        m_timerGenerate = startTimer(500);
    else
        m_timerGenerate = startTimer(5000);

    m_bfGenerate->start();
}
//------------------------------------------------------------------


void CWindowMain::on_satFinished()
{
    killTimer(m_timerRunSat);
    m_timerRunSat = 0;

    emit executingOperation(tr("finished"));
    emit messageAppend(m_sat->progressFinished());
}
//------------------------------------------------------------------


void CWindowMain::on_satRun(const QString &name)
{
    if (m_widgetListBF->bfCurrent() == NULL)
        return;

    CSatCreater *satCreater = new CSatCreater(name);

    m_sat = satCreater->create(m_widgetListBF->bfCurrent());

    delete satCreater;

    if (m_sat == NULL)
        return;

    connect(m_sat,SIGNAL(started()),m_widgetControl,SLOT(on_runLocked()));
    connect(m_sat,SIGNAL(finished()),m_widgetControl,SLOT(on_runUnlocked()));

    connect(m_sat,SIGNAL(started()),m_widgetTreeSat,SLOT(on_locked()));
    connect(m_sat,SIGNAL(finished()),m_widgetTreeSat,SLOT(on_unlocked()));
    connect(m_sat,SIGNAL(finished()),m_widgetTreeSat,SLOT(on_runNextChecked()));

    connect(m_sat,SIGNAL(started()),m_widgetBFGenerate,SLOT(on_locked()));
    connect(m_sat,SIGNAL(finished()),m_widgetBFGenerate,SLOT(on_unlocked()));

    connect(m_sat,SIGNAL(terminated()),this,SLOT(on_satFinished()));
    connect(m_sat,SIGNAL(successful(CBFView*)),this,SLOT(on_satFinished()));
    connect(m_sat,SIGNAL(finished()),m_sat,SLOT(deleteLater()));

    connect(m_widgetControl,SIGNAL(triggeredStopRun()),
            m_sat,SLOT(on_stopped()));

    connect(m_sat,SIGNAL(successful(CBFView*)),
            m_widgetBFView,SLOT(on_viewSat(CBFView*)));

    m_widgetConsol->actVisible()->toggled(true);

    emit executingOperation(m_sat->progressDescription());

    m_timerRunSat = startTimer(100);

    m_sat->start();
}
//------------------------------------------------------------------


void CWindowMain::on_satCheckedRun()
{
    if (m_widgetListBF->bfCurrent() == NULL)
        return;

    m_widgetTreeSat->on_startRunChecked();
}
//------------------------------------------------------------------


void CWindowMain::on_satCheckedRunLog()
{
    if (m_widgetListBF->bfCurrent() == NULL)
        return;

    m_widgetTreeSat->on_startRunLogChecked();
}
//------------------------------------------------------------------


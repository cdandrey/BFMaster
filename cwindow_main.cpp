#include "cwindow_main.h"
#include "cbool_formula.h"
#include "cexecut_object.h"
#include "cexecut_thread.h"
#include "csat.h"
#include "cwidget_bf_generate.h"
#include "cwidget_bf_list.h"
#include "cwidget_bf_view.h"
#include "cwidget_consol.h"
#include "cwidget_panel_mode.h"
#include "cwidget_tree_sat.h"

#include <QtDebug>
#include <QCloseEvent>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QScrollBar>
#include <QSplitter>
#include <QTextStream>
#include <QTimerEvent>

const QString CWindowMain::BFSuffix("bf");

CWindowMain::CWindowMain(QWidget *parent)
    : QMainWindow(parent),
      m_executObject(NULL),
      m_executThread(NULL),
      m_executTimer(0)
{
    m_widgetBFGenerate  = new CWidgetBFGenerate(this);
    m_widgetBFList      = new CWidgetBFList(this);
    m_widgetBFView      = new CWidgetBFView(this);
    m_widgetConsol      = new CWidgetConsol(this);
    m_widgetConsol->setVisible(false);
    m_widgetPanelMode   = new CWidgetPanelMode(this);
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
    spListBFTreeAlg->addWidget(m_widgetBFList);
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

    hbox->addWidget(m_widgetPanelMode);
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

    // m_widgetPanelMode
    connect(m_widgetPanelMode,SIGNAL(triggeredRun()),
            this,SLOT(on_satCheckedRun()));
    connect(m_widgetPanelMode,SIGNAL(triggeredRunLog()),
            this,SLOT(on_satCheckedRunLog()));

    connect(m_widgetPanelMode,SIGNAL(toggledRand(bool)),
            m_widgetBFList,SLOT(setVisible(bool)));
    connect(m_widgetPanelMode,SIGNAL(toggledRand(bool)),
            m_widgetBFList,SLOT(on_disabledHide(bool)));

    connect(m_widgetPanelMode,SIGNAL(toggledRand(bool)),
            m_widgetBFGenerate,SLOT(setVisible(bool)));
    connect(m_widgetPanelMode,SIGNAL(toggledRand(bool)),
            m_widgetBFGenerate->actVisible(),SLOT(setChecked(bool)));
    connect(m_widgetPanelMode,SIGNAL(toggledRand(bool)),
            m_widgetBFGenerate->actVisible(),SLOT(setEnabled(bool)));

    connect(m_widgetPanelMode,SIGNAL(toggledRand(bool)),
            m_widgetTreeSat,SLOT(setVisible(bool)));
    connect(m_widgetPanelMode,SIGNAL(toggledRand(bool)),
            m_widgetTreeSat,SLOT(on_disabledHide(bool)));

    // m_widgetBFView
    connect(m_widgetBFView,SIGNAL(message(QString)),
            this,SIGNAL(messageAppend(QString)));

    connect(this,SIGNAL(triggeredView()),
            m_widgetBFView,SIGNAL(triggered()));

    // m_widgetBFList
    connect(m_widgetBFList,SIGNAL(selected(QString,CBoolFormula*)),
            m_widgetBFGenerate,SLOT(on_set(QString,CBoolFormula*)));

    connect(m_widgetBFList,SIGNAL(message(QString)),
            this,SIGNAL(messageAppend(QString)));

    connect(m_widgetBFList,SIGNAL(selected(QString,CBoolFormula*)),
            m_widgetBFView,SLOT(on_set(QString,CBoolFormula*)));

    // m_widgetBFGenerate
    connect(m_widgetBFGenerate,SIGNAL(generate(QString,CBoolFormula*)),
            m_widgetBFList,SIGNAL(append(QString,CBoolFormula*)));

    connect(m_widgetBFGenerate,SIGNAL(generate(QString,CBoolFormula*)),
            this,SLOT(on_executed(QString,CBoolFormula*)));

    connect(m_widgetBFGenerate,SIGNAL(regenerate(QString,CBoolFormula*)),
            this,SLOT(on_executed(QString,CBoolFormula*)));

    connect(m_widgetBFGenerate,SIGNAL(remove(QString)),
            m_widgetBFList,SIGNAL(remove(QString)));

    // m_widgetTreeSat
    connect(m_widgetTreeSat,SIGNAL(run(QString)),
            this,SLOT(on_satRun(QString)));

    // main
    connect(this,SIGNAL(messageAppend(QString)),
            m_widgetConsol,SLOT(messageAppend(QString)));

    connect(this,SIGNAL(messageSet(QString)),
            m_widgetConsol,SLOT(messageSet(QString)));

    connect(this,SIGNAL(executingOperation(QString)),
            m_widgetConsol,SLOT(executingOperation(QString)));

    connect(this,SIGNAL(append(QString,CBoolFormula*)),
            m_widgetBFList,SIGNAL(append(QString,CBoolFormula*)));

    connect(this,SIGNAL(locked(bool)),
            m_widgetBFGenerate,SLOT(on_locked(bool)));

    m_widgetPanelMode->on_checkedRand();
}
//-------------------------------------------------------------------


CWindowMain::~CWindowMain()
{
    if (m_executObject != NULL)
        delete m_executObject;

    if (m_executThread != NULL)
        delete m_executThread;

    delete m_widgetBFGenerate;
    delete m_widgetBFList;
    delete m_widgetBFView;
    delete m_widgetConsol;
    delete m_widgetPanelMode;
    delete m_widgetTreeSat;

    m_actOpen->~QAction();
    m_actSave->~QAction();
    m_actSaveAs->~QAction();
    mainMenu->~QMenu();
}
//-------------------------------------------------------------------


void CWindowMain::closeEvent(QCloseEvent *e)
{
    if (m_executObject != NULL && m_executThread != NULL) {

        int btn = QMessageBox::warning(this,this->windowTitle(),
                                       tr("Приложение выполняет операцию: %1\n\n"
                                          "Прервать операцию и закрыть приложение?")
                                        .arg(m_executObject->progressDescription()),
                                       QMessageBox::Yes,
                                       QMessageBox::No,
                                       QMessageBox::Cancel);

        if (btn == QMessageBox::No
            || btn == QMessageBox::Cancel)
        {
            e->ignore();
            return;
        }
    }

    e->accept();
}
//-------------------------------------------------------------------


void CWindowMain::on_executed(const QString &name,CBoolFormula *bf)
{
    if (m_executObject != NULL || m_executThread != NULL)
        return;

    m_executObject = new CExecutGenerateFormula(name,bf);
    m_executThread = new CExecutThread(this,m_executObject);

    connect(m_widgetBFGenerate,SIGNAL(terminated()),
            m_executThread,SLOT(on_terminated()));

    connect(m_executThread,SIGNAL(finished()),
            m_widgetBFGenerate,SLOT(on_toggleRemove()));

    connect(m_executThread,SIGNAL(finished()),
            this,SLOT(on_finished()));

    startExec();
}
//------------------------------------------------------------------


void CWindowMain::on_finished()
{
    killTimer(m_executTimer);
    m_executTimer = 0;

    emit executingOperation(tr("finished"));
    emit messageAppend(m_executObject->progressFinished());
    emit locked(false);
    emit triggeredView();

    delete m_executObject;
    delete m_executThread;

    m_executObject = NULL;
    m_executThread = NULL;
}
//------------------------------------------------------------------


void CWindowMain::on_satFinished()
{
    killTimer(m_executTimer);
    m_executTimer = 0;

    emit executingOperation(tr("finished"));
    emit messageAppend(m_sat->progressFinished());
}
//------------------------------------------------------------------


void CWindowMain::on_satRun(const QString &name)
{
    Q_UNUSED(name);
//    if (m_widgetBFList->bfCurrent() == NULL)
//        return;

//    CSatCreater *satCreater = new CSatCreater(name);

//    m_sat = satCreater->create(m_widgetBFList->bfCurrent());

//    delete satCreater;

//    if (m_sat == NULL)
//        return;

//    connect(m_sat,SIGNAL(started()),m_widgetPanelMode,SLOT(on_runLocked()));
//    connect(m_sat,SIGNAL(finished()),m_widgetPanelMode,SLOT(on_runUnlocked()));

//    connect(m_sat,SIGNAL(started()),m_widgetTreeSat,SLOT(on_locked()));
//    connect(m_sat,SIGNAL(finished()),m_widgetTreeSat,SLOT(on_unlocked()));
//    connect(m_sat,SIGNAL(finished()),m_widgetTreeSat,SLOT(on_runNextChecked()));

//    connect(m_sat,SIGNAL(started()),m_widgetBFGenerate,SLOT(on_locked()));
//    connect(m_sat,SIGNAL(finished()),m_widgetBFGenerate,SLOT(on_unlocked()));

//    connect(m_sat,SIGNAL(terminated()),this,SLOT(on_satFinished()));
//    connect(m_sat,SIGNAL(successful(CBFView*)),this,SLOT(on_satFinished()));
//    connect(m_sat,SIGNAL(finished()),m_sat,SLOT(deleteLater()));

//    connect(m_widgetPanelMode,SIGNAL(triggeredStopRun()),
//            m_sat,SLOT(on_stopped()));

//    connect(m_sat,SIGNAL(successful(CBFView*)),
//            m_widgetBFView,SLOT(on_viewSat(CBFView*)));

//    m_widgetConsol->actVisible()->toggled(true);

//    emit executingOperation(m_sat->progressDescription());

//    m_executTimer = startTimer(100);

//    m_sat->start();
}
//------------------------------------------------------------------


void CWindowMain::on_satCheckedRun()
{
//    if (m_widgetBFList->bfCurrent() == NULL)
//        return;

//    m_widgetTreeSat->on_startRunChecked();
}
//------------------------------------------------------------------


void CWindowMain::on_satCheckedRunLog()
{
//    if (m_widgetBFList->bfCurrent() == NULL)
//        return;

//    m_widgetTreeSat->on_startRunLogChecked();
}
//------------------------------------------------------------------


void CWindowMain::on_open()
{
    QStringList lstFiles =  QFileDialog::getOpenFileNames(this,tr("Открыть функцию"),"",
                                                    tr("Булева функция(*.%1)").arg(BFSuffix));

    foreach (QString fileName, lstFiles) {

        QFile file(fileName);

        if (file.open(QIODevice::ReadOnly)) {

            QByteArray text(file.readAll());
            file.close();

            QList<QList<int> > cnf(CBoolFormula::fromDimacs(text));

            if (!cnf.isEmpty()) {

                emit m_widgetBFList->append(QFileInfo(file).baseName(),
                                    new CBoolFormula(cnf));
            } else {
                emit messageAppend(tr("ОШИБКА!Неизвестный формат файла функции."));
            }

        } else {
            emit messageAppend(TStr("%1 : %2")
                               .arg(tr("Невозможно открыть файл"))
                               .arg(fileName));
        }
    }
}
//------------------------------------------------------------------


void CWindowMain::on_save()
{
    QString bfName(m_widgetBFList->currentBFName());

    if (bfName.isEmpty()) {
        emit messageAppend(tr("ОШИБКА!Не выбрана функция из списка."));
        return;
    }

    QString fileName(QString("%1.%2").arg(bfName).arg(BFSuffix));

    QFileInfo fileInfo(fileName);

    if (!fileInfo.isFile()) {
        QString fileName = QFileDialog::getSaveFileName(this,tr("Сохранить функцию"),bfName,
                                                tr("Булева функция(*.%1)").arg(BFSuffix));

        fileInfo.setFile(fileName);

        if (!m_widgetBFList->rename(bfName,fileInfo.baseName())) {
            emit messageAppend(tr("Функция не сохранена."));
            return;
        }
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        emit messageAppend(tr("ОШИБКА!Невозможно открыть файл %1!").arg(fileName));
        return;
    }

    file.write(m_widgetBFList->currentBFDimacs().toLatin1());
    file.close();
    emit messageAppend(tr("Функция %1 - сохранена.").arg(m_widgetBFList->currentBFName()));
}
//------------------------------------------------------------------


void CWindowMain::on_save_as()
{
    QString bfName(m_widgetBFList->currentBFName());

    if (bfName.isEmpty()) {
        emit messageAppend(tr("ОШИБКА!Не выбрана функция из списка."));
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,tr("Сохранить функцию"),bfName,
                                            tr("Булева функция(*.%1)").arg(BFSuffix));

    QFileInfo fileInfo(fileName);

    if (!m_widgetBFList->rename(bfName,fileInfo.baseName())) {
        emit messageAppend(tr("Функция не сохранена."));
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        emit messageAppend(tr("ОШИБКА!Невозможно открыть файл %1!").arg(fileName));
        return;
    }

    file.write(m_widgetBFList->currentBFDimacs().toLatin1());
    file.close();
    emit messageAppend(tr("Функция %1 - сохранена.").arg(m_widgetBFList->currentBFName()));
}
//------------------------------------------------------------------


void CWindowMain::startExec()
{
    emit executingOperation(m_executObject->progressDescription());
    emit locked(true);

    m_executTimer = startTimer(100);
    m_executThread->start();
}
//------------------------------------------------------------------


void CWindowMain::timerEvent(QTimerEvent *)
{
    if (m_executObject != NULL && !m_executObject->isNullObject()) {
        //emit executingOperation(m_executObject->progressDescription());
        emit messageAppend(m_executObject->progress());
        qDebug() << m_executObject->progress();
    }
}
//------------------------------------------------------------------

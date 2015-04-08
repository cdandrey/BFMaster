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
      m_executThread(NULL)
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
            m_widgetTreeSat,SLOT(on_runChecked()));

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

    // m_widgetBFList
    connect(m_widgetBFList,SIGNAL(generate()),
            m_widgetBFGenerate,SLOT(on_generate()));

    connect(m_widgetBFList,SIGNAL(selected(QString,CBoolFormula*)),
            m_widgetBFGenerate,SLOT(on_set(QString,CBoolFormula*)));

    connect(m_widgetBFList,SIGNAL(selected(QString,CBoolFormula*)),
            m_widgetTreeSat,SLOT(on_set(QString,CBoolFormula*)));

    connect(m_widgetBFList,SIGNAL(message(QString)),
            this,SIGNAL(messageAppend(QString)));

    connect(m_widgetBFList,SIGNAL(selected(QString,CBoolFormula*)),
            m_widgetBFView,SLOT(on_set(QString,CBoolFormula*)));

    // m_widgetBFGenerate
    connect(m_widgetBFGenerate,SIGNAL(append(QString,CBoolFormula*)),
            m_widgetBFList,SIGNAL(appendgen(QString,CBoolFormula*)));

    connect(m_widgetBFGenerate,SIGNAL(replace(QString,CBoolFormula*)),
            m_widgetBFList,SIGNAL(replace(QString,CBoolFormula*)));

    connect(m_widgetBFGenerate,SIGNAL(execut(CExecutObject*)),
            this,SLOT(on_execut(CExecutObject*)));

    connect(m_widgetBFGenerate,SIGNAL(remove(QString)),
            m_widgetBFList,SIGNAL(remove(QString)));

    // m_widgetTreeSat
    connect(m_widgetTreeSat,SIGNAL(execut(QQueue<CExecutObject*>)),
            this,SLOT(on_execut(QQueue<CExecutObject*>)));

    // main
    connect(this,SIGNAL(messageAppend(QString)),
            m_widgetConsol,SLOT(messageAppend(QString)));

    connect(this,SIGNAL(messageSet(QString)),
            m_widgetConsol,SLOT(messageSet(QString)));

    connect(this,SIGNAL(executingOperation(QString)),
            m_widgetConsol,SLOT(executingOperation(QString)));

    connect(this,SIGNAL(append(QString,CBoolFormula*)),
            m_widgetBFList,SIGNAL(append(QString,CBoolFormula*)));

    connect(this,SIGNAL(updateView()),
            m_widgetBFView,SIGNAL(triggered()));

    connect(this,SIGNAL(locked(bool)),
            m_widgetBFGenerate,SLOT(on_locked(bool)));

    connect(this,SIGNAL(locked(bool)),
            m_widgetBFList,SLOT(setEnabled(bool)));

    connect(this,SIGNAL(locked(bool)),
            m_widgetTreeSat,SLOT(setEnabled(bool)));

    m_widgetPanelMode->on_checkedRand();
}
//-------------------------------------------------------------------


CWindowMain::~CWindowMain()
{
    if (m_executThread)
        delete m_executThread;
}
//-------------------------------------------------------------------


void CWindowMain::closeEvent(QCloseEvent *e)
{
    if (m_executThread) {

        int btn = QMessageBox::warning(this,this->windowTitle(),
                                       tr("Приложение выполняет операцию: %1\n\n"
                                          "Прервать операцию и закрыть приложение?")
                                        .arg("m_executObject->progressDescription()"),
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


void CWindowMain::on_execut(CExecutObject *obj)
{
    if (!obj->isNullObject() && m_executThread == NULL) {

        m_executThread = new CExecutThread(this,obj);

        connect(m_executThread,SIGNAL(executOperation(QString)),
                m_widgetConsol,SLOT(executingOperation(QString)));

        connect(m_executThread,SIGNAL(message(QString)),
                m_widgetConsol,SLOT(messageAppend(QString)));

        connect(m_widgetBFGenerate,SIGNAL(terminated()),
                m_executThread,SLOT(on_terminated()));

        connect(m_executThread,SIGNAL(finished()),
                m_widgetBFGenerate,SLOT(on_toggleRemove()));

        connect(m_executThread,SIGNAL(finished()),
                this,SLOT(on_finished()));

        emit locked(false);

        m_executTimer = startTimer(100);
        m_executThread->start();
    }
}
//------------------------------------------------------------------


void CWindowMain::on_execut(const QQueue<CExecutObject *> &queobj)
{
    if (!queobj.isEmpty() && m_executThread == NULL) {

        m_executThread = new CExecutThread(this,queobj);

        connect(m_executThread,SIGNAL(executOperation(QString)),
                m_widgetConsol,SLOT(executingOperation(QString)));

        connect(m_executThread,SIGNAL(message(QString)),
                m_widgetConsol,SLOT(messageAppend(QString)));

        connect(m_widgetBFGenerate,SIGNAL(terminated()),
                m_executThread,SLOT(on_terminated()));

        connect(m_executThread,SIGNAL(finished()),
                m_widgetBFGenerate,SLOT(on_toggleRemove()));

        connect(m_executThread,SIGNAL(finished()),
                this,SLOT(on_finished()));

        emit locked(false);

        m_executTimer = startTimer(100);
        m_executThread->start();
    }
}
//------------------------------------------------------------------


void CWindowMain::on_finished()
{
    killTimer(m_executTimer);
    m_executTimer = 0;

    emit locked(true);
    emit updateView();

    delete m_executThread;
    m_executThread = NULL;
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


void CWindowMain::timerEvent(QTimerEvent *)
{
    if (m_executThread) {
        //emit executingOperation(m_executThread->progressDescription());
        emit messageAppend(m_executThread->progress());
    }
}
//------------------------------------------------------------------



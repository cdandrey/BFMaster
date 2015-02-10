#ifndef CWINDOW_MAIN_H
#define CWINDOW_MAIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class CExecutObject;
class CExecutThread;
class CBoolFormula;
class CSat;
class CWidgetBFGenerate;
class CWidgetBFList;
class CWidgetBFView;
class CWidgetConsol;
class CWidgetPanelMode;
class CWidgetTreeSat;
class QAction;
class QMenu;
QT_END_NAMESPACE

class CWindowMain : public QMainWindow
{
    Q_OBJECT

public:

    explicit CWindowMain(QWidget *parent = 0);
    ~CWindowMain();

protected:

    virtual void closeEvent(QCloseEvent*);
    virtual void timerEvent(QTimerEvent*);

private slots:

    void on_executed(const QString &name,CBoolFormula *bf);
    void on_finished();

    void on_satFinished();
    void on_satRun(const QString&);
    void on_satCheckedRun();
    void on_satCheckedRunLog();

    void on_open();
    void on_save();
    void on_save_as();

private:

    void startExec();

private:

    CExecutObject       *m_executObject;
    CExecutThread       *m_executThread;
    int                  m_executTimer;

    CWidgetBFGenerate   *m_widgetBFGenerate;
    CWidgetBFList       *m_widgetBFList;
    CWidgetBFView       *m_widgetBFView;
    CWidgetConsol       *m_widgetConsol;
    CWidgetPanelMode    *m_widgetPanelMode;
    CWidgetTreeSat      *m_widgetTreeSat;

    QAction             *m_actOpen;
    QAction             *m_actSave;
    QAction             *m_actSaveAs;
    QMenu               *mainMenu;

    CSat                *m_sat;

    static const QString BFSuffix;

signals:

    void messageAppend(const QString &text);
    void messageSet(const QString &text);
    void executingOperation(const QString &text);

    void append(const QString &name, CBoolFormula *bf);
    void locked(bool);
    void triggeredView();

};

#endif // CWINDOW_MAIN_H

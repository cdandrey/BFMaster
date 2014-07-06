#ifndef CWINDOW_MAIN_H
#define CWINDOW_MAIN_H

#include <QMainWindow>
#include "typedef.h"

QT_BEGIN_NAMESPACE
class CBoolFunction;
class CBFGenerate;
class CBFView;
class CSat;
class CWidgetBFGenerate;
class CWidgetBFView;
class CWidgetConsol;
class CWidgetControl;
class CWidgetListBF;
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

private:

    QAction             *m_actOpen;
    QAction             *m_actSave;
    QAction             *m_actSaveAs;
    QMenu               *mainMenu;

    CWidgetBFGenerate   *m_widgetBFGenerate;
    CWidgetBFView       *m_widgetBFView;
    CWidgetControl      *m_widgetControl;
    CWidgetConsol       *m_widgetConsol;
    CWidgetListBF       *m_widgetListBF;
    CWidgetTreeSat      *m_widgetTreeSat;

    CBFGenerate         *m_bfGenerate;
    CSat                *m_sat;

    int                 m_timerGenerate;
    int                 m_timerRunSat;

    void timerEvent(QTimerEvent *event);

private slots:

    void on_generateFinished();
    void on_generateStarted(CBFView *bfv);

    void on_satFinished();
    void on_satRun(const QString&);
    void on_satCheckedRun();
    void on_satCheckedRunLog();

    void on_open();
    void on_save();
    void on_save_as();
    void on_save(const TStr &fileName,CBFView *bfv);

signals:

    void messageAppend(const QString&);
    void messageSet(const QString&);
    void executingOperation(const QString&);

};

#endif // CWINDOW_MAIN_H

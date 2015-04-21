#ifndef CWIDGET_PANEL_MODE_H
#define CWIDGET_PANEL_MODE_H

#include <QFrame>

QT_BEGIN_NAMESPACE
class CToolBarHeader;
class QAction;
class QToolBar;
QT_END_NAMESPACE

class CWidgetPanelMode : public QFrame
{
    Q_OBJECT

public:
    explicit CWidgetPanelMode(QWidget *parent = 0);

public slots:

    void on_checkedRand();
    void on_checkedEdit();
    void on_checkedCalc();
    void on_checkedTest();


private:

    bool m_isTriggeredRun;
    bool m_isTriggeredRunLog;

    CToolBarHeader *m_header;

    QToolBar *m_modeToolBar;
    QAction *m_actModeRand;
    QAction *m_actModeEdit;
    QAction *m_actModeCalc;
    QAction *m_actModeTest;

    QToolBar *m_runToolBar;
    QAction *m_actRun;
    QAction *m_actRunLog;

    static const char IconActRun[];
    static const char IconActRunLog[];
    static const char IconActStop[];
    static const char IconActStopLog[];

    static const char TextActRun[];
    static const char TextActRunLog[];
    static const char TextActStop[];

private slots:

    void on_changeRunStop(bool change);
    void on_changeRunLogStop(bool);
    void on_resetTriggered();
    void triggered_Run();
    void triggered_RunLog();

    void on_locked();
    void on_unlocked();

signals:

    void run();
    void runLog();

    void toggledRand(bool);
    void toggledEdit(bool);
    void toggledCalc(bool);
    void toggledTest(bool);
    void terminate();
};

#endif // CWIDGET_PANEL_MODE_H

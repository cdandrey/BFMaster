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

signals:

    void toggledRand(bool);
    void toggledEdit(bool);
    void toggledCalc(bool);
    void toggledTest(bool);
    void triggeredStopRun();
    void triggeredRun();
    void triggeredRunLog();

public slots:

    void on_checkedRand();
    void on_checkedEdit();
    void on_checkedCalc();
    void on_checkedTest();

    void on_locked();
    void on_unlocked();
    void on_runLocked();
    void on_runUnlocked();

private:

    CToolBarHeader *m_header;

    QToolBar *m_modeToolBar;
    QAction *m_actModeRand;
    QAction *m_actModeEdit;
    QAction *m_actModeCalc;
    QAction *m_actModeTest;

    QToolBar *m_runToolBar;
    QAction *m_actRun;
    QAction *m_actRunLog;


};

#endif // CWIDGET_PANEL_MODE_H

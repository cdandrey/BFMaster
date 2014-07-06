#ifndef CWIDGET_CONTROL_H
#define CWIDGET_CONTROL_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class CToolBarHeader;
class QAction;
class QToolBar;
QT_END_NAMESPACE

class CWidgetControl : public QWidget
{
    Q_OBJECT

public:
    explicit CWidgetControl(QWidget *parent = 0);

    QAction * actBFRand(){return m_actBFRand;}
    QAction * actBFEdit(){return m_actBFEdit;}
    QAction * actBFCalc(){return m_actBFCalc;}
    QAction * actBFTest(){return m_actBFTest;}
    QAction * actRun(){return m_actRun;}
    QAction * actRunLog(){return m_actRunLog;}

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

    QAction *m_actSave;
    QAction *m_actOpen;
    QAction *m_actQuit;

    QAction *m_actBFRand;
    QAction *m_actBFEdit;
    QAction *m_actBFCalc;
    QAction *m_actBFTest;

    QAction *m_actRun;
    QAction *m_actRunLog;

    CToolBarHeader *m_header;
    QToolBar *m_stateToolBar;
    QToolBar *m_runToolBar;

signals:

    void toggledRand(bool);
    void toggledEdit(bool);
    void toggledCalc(bool);
    void toggledTest(bool);
    void triggeredStopRun();
    void triggeredRun();
    void triggeredRunLog();
};

#endif // CWIDGET_CONTROL_H

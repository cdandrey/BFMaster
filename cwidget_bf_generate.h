#ifndef CWIDGET_BF_GENERATE_H
#define CWIDGET_BF_GENERATE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class CBoolFormula;
class CExecutObject;
class CToolBarHeader;
class QAction;
class QLineEdit;
class QSpinBox;
QT_END_NAMESPACE

class CWidgetBFGenerate : public QWidget
{
    Q_OBJECT

public:

    explicit CWidgetBFGenerate(QWidget *parent = 0);
    ~CWidgetBFGenerate();

    QAction *actVisible(){return m_actVisible;}

private:

    // data
    QString          m_name;    // file name and name in list of bool formula
    CBoolFormula    *m_bf;      // selected in lis bool formula
    CExecutObject   *m_exeObj;  // proxy object for execut generated bool formula

    // gui
    bool             m_isTriggeredGenerate; // flag action generate is triggered

    CToolBarHeader  *m_header;

    QLineEdit       *m_lineName;

    QAction         *m_actGenerate;
    QAction         *m_actRegenerate;
    QAction         *m_actRemove;
    QAction         *m_actNewName;
    QAction         *m_actVisible;

    QSpinBox        *m_spinX;
    QSpinBox        *m_spinZ;
    QSpinBox        *m_spinD;
    QSpinBox        *m_spinMin;
    QSpinBox        *m_spinMax;

    QSpinBox* newSpin(int val = 0, int min = 0, int max = 100000);

private slots:

    void on_changeRemoveStop(bool change);
    void on_generate();
    void on_set(const QString&,CBoolFormula*);
    void on_resetTriggered();

    void on_locked();
    void on_unlocked();

    void triggered_actNewName();
    void triggered_actGenerate();
    void triggered_actRegenerate();
    void triggered_actRemove();

signals:

    void append(const QString &bfName,CBoolFormula *bf);
    void execut(QList<CExecutObject*> lstObj);
    void remove(const QString &bfName);
    void replace(const QString &bfName,CBoolFormula *bf);
    void terminated();
};

#endif // CWIDGET_BF_GENERATE_H

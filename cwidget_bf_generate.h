#ifndef CWIDGET_BF_GENERATE_H
#define CWIDGET_BF_GENERATE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class CBoolFormula;
class CToolBarHeader;
class QLineEdit;
class QAction;
class QSpinBox;
QT_END_NAMESPACE

class CWidgetBFGenerate : public QWidget
{
    Q_OBJECT

public:

    explicit CWidgetBFGenerate(QWidget *parent = 0);

    QAction *actVisible(){return m_actVisible;}

public slots:

    void on_set(const QString&,CBoolFormula*);
    void on_locked(bool lock);
    void on_toggleRemove();
    void on_toggleTerminate();

    void triggered_actNewName();
    void triggered_actGenerate();
    void triggered_actRegenerate();
    void triggered_actRemove();

private:

    // data
    CBoolFormula    *m_bf;
    QString          m_name;

    // gui
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

    volatile bool m_isRemove;

signals:

    void generate(const QString &name,CBoolFormula *bf);
    void regenerate(const QString &name,CBoolFormula *bf);
    void remove(const QString&);
    void terminated();
};

#endif // CWIDGET_BF_GENERATE_H

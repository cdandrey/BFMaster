#ifndef CWIDGET_BF_GENERATE_H
#define CWIDGET_BF_GENERATE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class CBFView;
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

    void on_set(CBFView *bfv);
    void on_locked();
    void on_unlocked();
    void on_generateLocked();
    void on_generateUnlocked();

private:

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

    CBFView         *m_bfv;

    QSpinBox* newSpin(int val = 0, int min = 0, int max = 100000);

private slots:

    void triggered_actGenerate();
    void triggered_actRegenerate();
    void triggered_actRemove();
    void triggered_actNewName();

signals:

    void generate(CBFView*);
    void remove(const QString&);
    void remove(CBFView*);
    void stopped();
};

#endif // CWIDGET_BF_GENERATE_H

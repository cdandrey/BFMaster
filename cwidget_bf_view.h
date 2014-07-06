#ifndef CWIDGET_BF_VIEW_H
#define CWIDGET_BF_VIEW_H

#include <QWidget>
#include <QMap>

QT_BEGIN_NAMESPACE
class CBFView;
class CToolBarHeader;
class QAction;
class QActionGroup;
class QComboBox;
class QTextEdit;
QT_END_NAMESPACE

class CWidgetBFView : public QWidget
{
    Q_OBJECT

public:

    explicit CWidgetBFView(QWidget *parent = 0);

public slots:

    void on_view(CBFView*);
    void on_viewSat(CBFView*);
    void on_remove(const QString&);

private:

    CToolBarHeader         *m_header;

    QComboBox       *m_combo;
    QAction         *m_actView;
    QAction         *m_actViewDIMACS;
    QAction         *m_actViewDis;
    QAction         *m_actViewDisSort;
    QAction         *m_actViewVar;
    QAction         *m_actViewVarSort;
    QActionGroup    *m_groupActView;

    QAction         *m_actViewSat;

    QTextEdit       *m_edit;

    typedef QMap<QString,CBFView*> TMapBF;

    TMapBF m_bfv;

private slots:
    void triggered_actHide();
    void on_view(const QString&);
    void on_view(bool);
    void on_view();
};

#endif // CWIDGET_BF_VIEW_H

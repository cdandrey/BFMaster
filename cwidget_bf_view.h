#ifndef CWIDGET_BF_VIEW_H
#define CWIDGET_BF_VIEW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class CBoolFormula;
class CToolBarHeader;
class QAction;
class QActionGroup;
class QLabel;
class QTextEdit;
QT_END_NAMESPACE

class CWidgetBFView : public QWidget
{
    Q_OBJECT

public:

    explicit CWidgetBFView(QWidget *parent = 0);

public slots:

    void on_set(const QString &name, CBoolFormula *bf);

    void on_viewClaus();
    void on_viewClausSort();
    void on_viewDimacs();
    void on_viewFormula();
    void on_viewLits();
    void on_viewLitsSort();

private:

    QString         *m_bfName;
    CBoolFormula    *m_bf;

    CToolBarHeader  *m_header;

    QLabel          *m_lableNameFormula;

    QAction         *m_actViewFormula;
    QAction         *m_actViewDimacs;
    QAction         *m_actViewClaus;
    QAction         *m_actViewClausSort;
    QAction         *m_actViewLits;
    QAction         *m_actViewLitsSort;
    QActionGroup    *m_groupActView;

    QAction         *m_actViewSat;

    QTextEdit       *m_edit;

    static const QString TableStyle;
    static const QString BgColorC;
    static const QString BgColorE;
    static const QString BgColorF;

           bool    checkViewFormula();
    inline QString htmlLit(int lit) const;
    inline QString htmlClaus(int claus) const;

private slots:

    void on_disconnect();
    void on_setLableNameFormula();
    void triggered_actHide();

    void triggered_actViewFormula();
    void triggered_actViewDimacs();
    void triggered_actViewClaus();
    void triggered_actViewClausSort();
    void triggered_actViewLits();
    void triggered_actViewLitsSort();

signals:

    void message(const QString &msg);
    void setHtml(const QString &text);
    void setText(const QString &text);
    void triggered();
};

#endif // CWIDGET_BF_VIEW_H

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

           bool    isBoolFormula();
    inline QString htmlLit(int lit) const;
    inline QString htmlClaus(int claus) const;
           QString htmlLstLit(const QList<int> &lst,const QString &divider = "") const;

private slots:

    void on_set(const QString &name, CBoolFormula *bf);
    void on_setLableNameFormula();

    void on_viewClaus(bool isToggle);
    void on_viewClausSort(bool isToggle);
    void on_viewDimacs(bool isToggle);
    void on_viewFormula(bool isToggle);
    void on_viewLits(bool isToggle);
    void on_viewLitsSort(bool isToggle);
    void on_viewUpdate();

    QAction* checked_Action() const;

    void triggered_actHide();

signals:

    void getLogSelectSat(bool isGet);
    void message(const QString &msg);
    void setHtml(const QString &text);
    void setText(const QString &text);
    void triggered();
};

#endif // CWIDGET_BF_VIEW_H

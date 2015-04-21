#ifndef CWIDGET_BF_LIST_H
#define CWIDGET_BF_LIST_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class CBoolFormula;
class CModelBFList;
class CToolBarHeader;
class QAction;
class QListView;
QT_END_NAMESPACE

class CWidgetBFList : public QWidget
{
    Q_OBJECT

public:
    explicit CWidgetBFList(QWidget *parent = 0);
    ~CWidgetBFList();

    QAction *actVisible(){return m_actVisible;}
    QString currentBFName() const;
    QString currentBFDimacs() const;
    CBoolFormula *currentBFPoint() const;
    bool rename(const QString &bfOldName,const QString &bfNewName);

private:

    CToolBarHeader    *m_header;
    QListView         *m_list;
    CModelBFList      *m_model;
    QAction           *m_actVisible;

private slots:

    void on_disabledHide(bool disabled);

signals:

    void append(const QString &bfName,CBoolFormula *bf);
    void append(const QStringList &bfNameLst,const QList<CBoolFormula*> &bfLst);
    void appendgen(const QString &bfName,CBoolFormula *bf);
    void generate();
    void locked(bool);
    void message(const QString &message);
    void remove(const QString &bfName);
    void replace(const QString &bfName,CBoolFormula *bf);
    void selected(const QString &bfName,CBoolFormula *bf);

};

#endif // CWIDGET_BF_LIST_H

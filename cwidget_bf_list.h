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
    bool rename(const QString &name,const QString &newName);

public slots:

    void on_disabledHide(bool disabled);

private:

    CToolBarHeader    *m_header;
    QListView         *m_list;
    CModelBFList      *m_model;
    QAction           *m_actVisible;

signals:

    void append(const QString&,CBoolFormula*);
    void append(const QStringList&,const QList<CBoolFormula*>&);
    void message(const QString&);
    void remove(const QString&);
    void selected(const QString&,CBoolFormula*);

};

#endif // CWIDGET_BF_LIST_H

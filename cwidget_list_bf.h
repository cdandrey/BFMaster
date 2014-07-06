#ifndef CWIDGET_LIST_BF_H
#define CWIDGET_LIST_BF_H

#include <QWidget>
#include <QMap>

QT_BEGIN_NAMESPACE
class CBFView;
class CToolBarHeader;
class QAction;
class QListWidget;
class QListWidgetItem;
QT_END_NAMESPACE

class CWidgetListBF : public QWidget
{
    Q_OBJECT

public:
    explicit CWidgetListBF(QWidget *parent = 0);
    ~CWidgetListBF();

    QAction *actVisible(){return m_actVisible;}
    CBFView *bfCurrent();

public slots:

    void on_append(CBFView*);
    void on_remove(CBFView*);
    void on_disabledHide(bool disabled);
    void on_updateCurrentItem();

private:

    CToolBarHeader     *m_header;
    QAction     *m_actVisible;
    QListWidget *m_list;

    typedef QMap<QListWidgetItem*,CBFView*> TMapItemBF;
    TMapItemBF m_bfv;

    bool unique(CBFView *bfv) const;

private slots:

    void clicked_itemList(QListWidgetItem*);

signals:

    void send(CBFView*);
    void view(CBFView*);
};

#endif // CWIDGET_LIST_BF_H

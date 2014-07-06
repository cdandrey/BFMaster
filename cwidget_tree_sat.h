#ifndef CWIDGET_TREE_SAT_H
#define CWIDGET_TREE_SAT_H

#include <QWidget>
#include "typedef.h"

QT_BEGIN_NAMESPACE
class CToolBarHeader;
class QAction;
class QThread;
class QTreeWidget;
class QTreeWidgetItem;
QT_END_NAMESPACE

class CWidgetTreeSat : public QWidget
{
    Q_OBJECT
public:
    explicit CWidgetTreeSat(QWidget *parent = 0);

    QAction *actVisible(){return m_actVisible;}

public slots:

    void on_disabledHide(bool disabled);
    void on_locked();
    void on_unlocked();
    void on_runNextChecked();
    void on_startRunChecked();
    void on_startRunLogChecked();

private:

    CToolBarHeader  *m_header;
    QAction         *m_actVisible;
    QTreeWidget     *m_tree;

    typedef enum {Run = 1,RunLog =2} TRunType;

    bool            m_runChecked;
    TRunType        m_runType;

    void createTreeItem();

private slots:

    void clicked_treeWidgetItem(QTreeWidgetItem*);

signals:

    void run(const QString&);
};

#endif // CWIDGET_TREE_SAT_H

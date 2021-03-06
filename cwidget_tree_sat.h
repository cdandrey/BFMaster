#ifndef CWIDGET_TREE_SAT_H
#define CWIDGET_TREE_SAT_H

#include <QWidget>
#include <QMap>

QT_BEGIN_NAMESPACE
class CBoolFormula;
class CExecutObject;
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
    ~CWidgetTreeSat();

    QAction *actVisible(){return m_actVisible;}

private:

    typedef QMap<QTreeWidgetItem*,CExecutObject*> TMapItemExeObject;

    // data
    QString      *m_bfName;
    CBoolFormula *m_bf;
    TMapItemExeObject m_obj;

    // gui
    CToolBarHeader  *m_header;
    QAction         *m_actVisible;
    QTreeWidget     *m_tree;

    enum {IndexSat = 0};

    void createTreeItem();
    bool isEmpty();

private slots:

    void on_disabledHide(bool disabled);
    void on_returnLogSelectSat(bool isGet);
    void on_runChecked();
    void on_runLogChecked();

    void on_set(const QString &bfName,CBoolFormula *bf);

signals:

    void execut(const QList<CExecutObject*> &lstObj);
    void logSelectSat(const QString&);

};

#endif // CWIDGET_TREE_SAT_H

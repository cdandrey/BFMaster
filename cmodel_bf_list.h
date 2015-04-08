#ifndef CMODEL_BF_LIST_H
#define CMODEL_BF_LIST_H

#include <QAbstractListModel>

QT_BEGIN_NAMESPACE
class CBoolFormula;
QT_END_NAMESPACE

class CModelBFList : public QAbstractListModel
{
    Q_OBJECT
public:

    explicit CModelBFList(QObject *parent = 0);
    ~CModelBFList();

    QVariant        data(const QModelIndex &index, int role) const;
    CBoolFormula    *pointData(const QModelIndex &index) const;
    int             rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool            rename(const QString &bfOldName,const QString &bfNewName);

    enum {DimacsRole = Qt::UserRole};

public slots:

    void on_append(const QString &bfName,CBoolFormula *bf);
    void on_append(const QStringList &lstName,const QList<CBoolFormula*> &lstBf);
    void on_appendgen(const QString &bfName,CBoolFormula *bf);
    void on_remove(const QString &bfName);
    void on_replacegen(const QString &bfName,CBoolFormula *bf);
    void on_select(const QModelIndex &index);

private:

    QList<CBoolFormula*> m_bfs;
    QStringList          m_names;

    bool isUniqName(const QString &name);

signals:

    void generate();
    void message(const QString&);
    void selected(const QString&,CBoolFormula*);

};

#endif // CMODEL_BF_LIST_H

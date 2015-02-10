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

    //CBoolFormula* currentFormula() const;
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool rename(const QString &name,const QString &newName);

    enum {DimacsRole = Qt::UserRole};

public slots:

    void on_append(const QString &name,CBoolFormula *bf);
    void on_append(const QStringList &lstName,const QList<CBoolFormula*> &lstBf);
    void on_remove(const QString &name);
    void on_select(const QModelIndex &index);

private:

    QList<CBoolFormula*> m_bfs;
    QStringList          m_names;

    bool isUniqName(const QString &name);

signals:

    void message(const QString&);
    void selected(const QString&,CBoolFormula*);

};

#endif // CMODEL_BF_LIST_H

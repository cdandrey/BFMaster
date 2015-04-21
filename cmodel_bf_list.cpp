#include <QIcon>

#include "cmodel_bf_list.h"
#include "cbool_formula.h"

CModelBFList::CModelBFList(QObject *parent) :
    QAbstractListModel(parent)
{
}
//------------------------------------------------------------------

CModelBFList::~CModelBFList()
{
    qDeleteAll(m_bfs);
}
//------------------------------------------------------------------


int CModelBFList::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_bfs.size();
}
//------------------------------------------------------------------


QVariant CModelBFList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || (index.row() >= m_bfs.size()))
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return m_names.at(index.row());
    case Qt::DecorationRole:
        return QIcon("://ico/lstbf_item.png");
    case DimacsRole:
        return static_cast<QVariant>(m_bfs.at(index.row())->dimacs());
    default:
        return QVariant();
    }
}
//------------------------------------------------------------------


CBoolFormula *CModelBFList::pointData(const QModelIndex &index) const
{
    if (!index.isValid() || (index.row() >= m_bfs.size()))
        return NULL;

    return m_bfs.at(index.row());
}
//------------------------------------------------------------------


void CModelBFList::on_append(const QString &bfName, CBoolFormula *bf)
{
    if (bf == NULL)
        return;

    if (bfName.isEmpty()) {
        emit message(tr("ОШИБКА! Пустое имя функции."
                        "Функции %1 не была добавлена в список."));
        delete bf;
        return;
    }

    if (m_names.indexOf(bfName) >= 0) {
        emit message(tr("ОШИБКА! Одинаковые именна функций."
                        "Функции %1 не была добавлена в список.")
                     .arg(bfName));
        delete bf;
        return;
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_bfs << bf;
    m_names << bfName;
    endInsertRows();
}
//------------------------------------------------------------------


void CModelBFList::on_append(const QStringList &lstName, const QList<CBoolFormula*> &lstBf)
{
    for (int i = 0; i < lstBf.size(); ++i)
        on_append(lstName.at(i),lstBf.at(i));
}
//------------------------------------------------------------------


void CModelBFList::on_appendgen(const QString &bfName, CBoolFormula *bf)
{
    on_append(bfName,bf);
    emit generate();
}
//------------------------------------------------------------------


void CModelBFList::on_replacegen(const QString &bfName, CBoolFormula *bf)
{
    int i;
    if (bf == NULL || bfName.isEmpty()
        || (i = m_bfs.indexOf(bf)) < 0 || i >= m_bfs.size())
        return;

    beginInsertRows(QModelIndex(), rowCount(), rowCount()); // update view in list
    m_names[i] = bfName;
    endInsertRows();

    //emit selected(bfName,bf);
    emit generate();
}
//------------------------------------------------------------------


void CModelBFList::on_remove(const QString& bfName)
{
    int row;
    if ((row = m_names.indexOf(bfName)) == -1)
        return;

    beginRemoveRows(QModelIndex(), row, row);
    delete m_bfs[row];
    m_bfs.removeAt(row);
    m_names.removeAt(row);
    endRemoveRows();

    if (row == m_names.size())
        row = m_names.size() - 1;

    if (row >= 0)
        emit selected(m_names[row],m_bfs.at(row));
    else
        emit selected("",NULL);
}
//------------------------------------------------------------------


bool CModelBFList::rename(const QString &bfOldName, const QString &bfNewName)
{
    if (bfOldName == bfNewName)
        return true;

    int row = m_names.indexOf(bfOldName);

    if (row == -1) {
        emit message(tr("ОШИБКА!Функция %1 не найдена.")
                     .arg(bfOldName));
        return false;
    }

    if (bfNewName.isEmpty()) {
        emit message(tr("ОШИБКА! Не заданно новое имя функции."));
        return false;
    }

    if (m_names.indexOf(bfNewName) >= 0) {
        emit message(tr("ОШИБКА! Функция %1 уже существует в списке.")
                     .arg(bfNewName));
        return false;
    }

    m_names[row] = bfNewName;
    emit selected(m_names.at(row),m_bfs.at(row));
    return true;
}
//------------------------------------------------------------------


void CModelBFList::on_select(const QModelIndex &index)
{
    if (index.isValid())
        emit selected(m_names.at(index.row()),m_bfs.at(index.row()));
    else
        emit selected("",NULL);
}
//------------------------------------------------------------------


bool CModelBFList::isUniqName(const QString &name)
{
    foreach(QString iName,m_names)
        if (name == iName) {
            emit message(tr("ПРЕДУПРЕЖДЕНИЕ! Одинаковые именна функций."
                            "Функции %1 не была добавлена в список.")
                         .arg(name));
            return false;
        }

    return true;
}
//------------------------------------------------------------------


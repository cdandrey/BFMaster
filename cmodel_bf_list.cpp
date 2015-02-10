#include "cmodel_bf_list.h"
#include "cbool_formula.h"

#include <QIcon>
#include <QDebug>

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


void CModelBFList::on_append(const QString &name, CBoolFormula *bf)
{
    if (bf == NULL)
        return;

    if (name.isEmpty()) {
        emit message(tr("ОШИБКА! Пустое имя функции."
                        "Функции %1 не была добавлена в список."));
        delete bf;
        return;
    }

    if (m_names.indexOf(name) >= 0) {
        emit message(tr("ОШИБКА! Одинаковые именна функций."
                        "Функции %1 не была добавлена в список.")
                     .arg(name));
        delete bf;
        return;
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_bfs << bf;
    m_names << name;
    endInsertRows();
}
//------------------------------------------------------------------


void CModelBFList::on_append(const QStringList &lstName, const QList<CBoolFormula*> &lstBf)
{
    for (int i = 0; i < lstBf.size(); ++i)
        on_append(lstName.at(i),lstBf.at(i));
}
//------------------------------------------------------------------


void CModelBFList::on_remove(const QString& name)
{
    int row;
    if ((row = m_names.indexOf(name)) == -1)
        return;

    beginRemoveRows(QModelIndex(), row, row);
    delete m_bfs[row];
    m_bfs.removeAt(row);
    m_names.removeAt(row);
    endRemoveRows();

    if (row == m_names.size())
        row = m_names.size() - 1;

    if (row >= 0)
        emit selected(m_names.at(row),m_bfs.at(row));
    else
        emit selected("",NULL);
}
//------------------------------------------------------------------


bool CModelBFList::rename(const QString &name, const QString &newName)
{
    if (name == newName)
        return true;

    int row = m_names.indexOf(name);

    if (row == -1) {
        emit message(tr("ОШИБКА!Функция %1 не найдена.")
                     .arg(name));
        return false;
    }

    if (newName.isEmpty()) {
        emit message(tr("ОШИБКА! Не заданно новое имя функции."));
        return false;
    }

    if (m_names.indexOf(newName) >= 0) {
        emit message(tr("ОШИБКА! Функция %1 уже существует в списке.")
                     .arg(newName));
        return false;
    }

    m_names[row] = newName;
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


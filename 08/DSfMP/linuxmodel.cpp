#include "linuxmodel.h"
#include <QSqlQuery>
#include <QSqlError>

const char* COLUMN_NAMES[] = {
  "linux_id",
  "name",
  "family",
  NULL
};

const char* SQL_SELECT =
  "SELECT linux.id, linux.name, linux.family FROM linux";


LinuxModel::LinuxModel(QSqlDatabase &sdb, QObject *parent)
    : QSqlTableModel(parent), m_sdb(sdb)
{
    int idx = 0;
    while(COLUMN_NAMES[idx]) {
        m_roleNames[Qt::UserRole + idx + 1] = COLUMN_NAMES[idx];
        idx++;
    }
    refresh();
}

QVariant LinuxModel::data(const QModelIndex &index, int role) const {
    QVariant value = QSqlQueryModel::data(index, role);
    if(role < Qt::UserRole)
    {
        value = QSqlQueryModel::data(index, role);
    }
    else
    {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
    }
    return value;
}

QHash<int, QByteArray> LinuxModel::roleNames() const
{
    return m_roleNames;
}

void LinuxModel::remove(int i)
{
    QSqlQuery query(m_sdb);
    query.prepare("DELETE FROM linux WHERE id = :id;");
    query.bindValue(0, i);
    query.exec();
    refresh();
}

void LinuxModel::append(QJsonObject obj)
{
    QSqlQuery query(m_sdb);
    query.prepare("INSERT INTO linux (name, family) VALUES (:name, :family)");
    query.bindValue(0, obj.value("name").toString());
    query.bindValue(1, obj.value("family").toString());
    bool success = query.exec();
    if (!success) {
        qDebug() << query.lastError().text();
    }
    refresh();
}

void LinuxModel::refresh() {
    QSqlQuery query(SQL_SELECT, m_sdb);
    this->setQuery(query);
}

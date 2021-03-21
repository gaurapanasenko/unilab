#ifndef LINUXMODEL_H
#define LINUXMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QJsonObject>

class LinuxModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit LinuxModel(QSqlDatabase &sdb, QObject *parent = nullptr);
    void refresh();
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE void remove(int i);
    Q_INVOKABLE void append(QJsonObject obj);

private:
    QHash<int, QByteArray> m_roleNames;
    QSqlDatabase &m_sdb;
};

#endif // LINUXMODEL_H

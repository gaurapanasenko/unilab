#ifndef LINUXMODEL_H
#define LINUXMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QJsonObject>

/**
 * @brief Модель бази даних дистрибутивів Linux.
 */
class LinuxModel : public QSqlTableModel
{
    Q_OBJECT
public:
    /**
     * @brief LinuxModel конструктор
     * @param sdb Підключена база даних.
     * @param parent Батьтівскій об'єкт
     */
    explicit LinuxModel(QSqlDatabase &sdb, QObject *parent = nullptr);
    /**
     * @brief Загрузити оновлені данні з бази даних.
     */
    void refresh();
    /**
     * @brief Перевантажений метод, який дозволяє QT отримати значення даних.
     * @param index Номер рядка.
     * @param role Номер стовбця.
     * @return Значення комірки у базі даних.
     */
    QVariant data(const QModelIndex &index, int role) const;
    /**
     * @brief Виводить прив'язку номеру стовбця до її назви.
     * @return Хеш таблицю.
     */
    QHash<int, QByteArray> roleNames() const;
    /**
     * @brief Видаляє елемент по id у базі данних.
     * @param i id об'єкту.
     */
    Q_INVOKABLE void remove(int i);
    /**
     * @brief Оновити значення у базі даних.
     * @param i id об'єкту.
     * @param col Назва колонки.
     * @param value Нове значення.
     */
    Q_INVOKABLE void update(int i, QString col, QString value);
    /**
     * @brief Додає новий об'єкт до бази даних.
     * Дані необхідно передавати у вигляді json.
     * Наприклад `{"name": "Ubuntu", "family": "apt"}`
     * @param obj дані об'єкту у вигляді json.
     */
    Q_INVOKABLE void append(QJsonObject obj);

private:
    /**
     * @brief Прив'язку номеру стовбця до її назви.
     */
    QHash<int, QByteArray> m_roleNames;
    /**
     * @brief Посилання на об'єкт, який підключений до бази даних.
     */
    QSqlDatabase &m_sdb;
};

#endif // LINUXMODEL_H

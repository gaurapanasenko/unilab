#ifndef LINUX_H
#define LINUX_H

#include <QString>

/**
 * @brief Клас який позначає деякий дистрибутив Linux.
 */
class Linux
{
public:
    Linux(const QString &name, const QString &family);

    /**
     * @brief Забирає назву дистрибутиву.
     * @return Назва дистрибутиву.
     */
    QString name() const;
    /**
     * @brief Задає нову назву дистрибутиву.
     * @param name Нова назва дистрибутиву.
     */
    void setName(const QString &name);

    QString family() const;
    void setFamily(const QString &family);

//signals:
    /**
     * @brief Сигнал який говорить що назва дистрибутиву була змінена.
     */
    //void nameChanged();
    //void familyChanged();

private:
    /**
     * @brief Ім'я дистрибутиву Linux.
     */
    QString m_name;
    /**
     * @brief Сімейство дистрибутиву.
     */
    QString m_family;
};

#endif // LINUX_H

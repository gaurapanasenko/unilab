#ifndef QMLTRANSLATOR_H
#define QMLTRANSLATOR_H

#include <QObject>
#include <QTranslator>
#include <QGuiApplication>

/**
 * @brief Дозволяє змінювати мову при роботі програми з QML
 */
class QmlTranslator : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief QmlTranslator конструктор
     * @param parent Посилання на батьківскій об'єкт
     */
    explicit QmlTranslator(QObject *parent = nullptr);

signals:
    /**
     * @brief Сигнал який говорить що мова була змінена.
     */
    void languageChanged();

public:
    /**
     * @brief Задає мову яка повинна відображатися
     * @param translation Код мови, наприклад "en_US".
     */
    Q_INVOKABLE void setTranslation(QString translation);

private:
    /**
     * @brief Перекладач, який зберігає поточний переклад програми.
     */
    QTranslator m_translator;
};

#endif // QMLTRANSLATOR_H

#ifndef QMLTRANSLATOR_H
#define QMLTRANSLATOR_H

#include <QObject>
#include <QTranslator>
#include <QGuiApplication>

class QmlTranslator : public QObject
{
    Q_OBJECT
public:
    explicit QmlTranslator(QObject *parent = nullptr);

signals:
    void languageChanged();

public:
    Q_INVOKABLE void setTranslation(QString translation);

private:
    QTranslator m_translator;
};

#endif // QMLTRANSLATOR_H

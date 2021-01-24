#include <QtGui>
#include "qmltranslator.h"

QmlTranslator::QmlTranslator(QObject *parent) : QObject(parent)
{

}

void QmlTranslator::setTranslation(QString translation)
{
    qApp->removeTranslator(&m_translator);
    if (m_translator.load("DSfMP_" + translation, ":/")) {
        qApp->installTranslator(&m_translator);
    } else {
        qDebug() << "Failed to load translation file: "
            << "DSfMP_" + translation;
    }
    emit languageChanged();
}

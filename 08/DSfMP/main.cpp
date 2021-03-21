#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include "qmltranslator.h"
#include "linuxmodel.h"
#include "linux.h"

#define DATABASE "db_name.sqlite"

const char * CREATE_DB = "CREATE TABLE 'linux' ("
"    'id'	INTEGER,"
"    'name'	TEXT,"
"    'family'	TEXT,"
"    PRIMARY KEY('id' AUTOINCREMENT)"
");";
const char * FILL_DB = "INSERT INTO linux (name, family) VALUES"
"('Debian', 'apt'),"
"('Ubuntu', 'apt'),"
"('OpenSUSE', 'rpm'),"
"('Arch Linux', 'pacman');";

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QmlTranslator qmlTranslator;

    bool existsDb = QFile::exists(DATABASE);

    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    QSqlQuery query(sdb);
    sdb.setDatabaseName(DATABASE);
    if (!sdb.open()) {
        qDebug() << sdb.lastError().text();
        qDebug() << "Failed to open database.";
        return -1;
    }
    if (!existsDb) {
        {
            bool success = query.exec(CREATE_DB);
            if (!success) {
                qDebug() << query.lastError().text();
                qDebug() << "Failed to create database.";
                return -1;
            }
        }
        {
            bool success = query.exec(FILL_DB);
            if (!success) {
                qDebug() << query.lastError().text();
                qDebug() << "Failed to fill database.";
                return -1;
            }
        }
    }

    LinuxModel *linuxModel = new LinuxModel(sdb, &app);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.rootContext()->setContextProperty("qmlTranslator", &qmlTranslator);
    engine.rootContext()->setContextProperty("linuxModel", linuxModel);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    QObject::connect(&qmlTranslator, &QmlTranslator::languageChanged,
                     &engine, &QQmlApplicationEngine::retranslate, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

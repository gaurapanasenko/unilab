#ifndef FILEIO_H
#define FILEIO_H

#include <QQuickItem>
#include <QUrl>
#include <QFile>
#include <QProcess>

class FileIO : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
public:
    explicit FileIO() {
    }
    ~FileIO() {}
    const QUrl& url() {
        return m_url;
    }
    void setUrl(const QUrl& url) {
        m_url = url;
    }
signals:
    void urlChanged();
public slots:
    void save(const QString& data) {
        QFile file(m_url.toLocalFile());
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        out << data;
    }
    const QString load() {
        QFile file(m_url.toLocalFile());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return "Error";

        QByteArray bytes;
        while (!file.atEnd())
            bytes.append(file.readLine());
        return bytes;
    }
    const QString run(const QString& name, const QString& data) {
        QProcess runner;
        runner.start(name);
        if (!runner.waitForStarted())
            return "Error";

        runner.write(data.toLocal8Bit());
        runner.closeWriteChannel();

        if (!runner.waitForFinished())
            return "Error";

        return runner.readAll();
    }
private:
    QUrl m_url;
};


#endif // FILEIO_H

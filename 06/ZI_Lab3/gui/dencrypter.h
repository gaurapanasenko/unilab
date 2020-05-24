#ifndef FILEIO_H
#define FILEIO_H

#include <QQuickItem>
#include <QUrl>
#include <QFile>
#include <QProcess>


class Dencrypter : public QQuickItem // name is just a joke
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QByteArray key READ key WRITE setKey NOTIFY keyChanged)
    Q_PROPERTY(QByteArray keyHex READ keyHex WRITE setKeyHex NOTIFY keyChanged)
    Q_PROPERTY(QByteArray data READ data WRITE setData NOTIFY dataChanged)
    Q_PROPERTY(QByteArray dataHex READ dataHex WRITE setDataHex NOTIFY dataChanged)
public:
    explicit Dencrypter() {
    }
    ~Dencrypter() {}
    const QUrl& url() const {
        return m_url;
    }
    void setUrl(const QUrl& url) {
        m_url = url;
        emit urlChanged();
    }
    const QByteArray& key() const {
        return m_key;
    }
    void setKey(const QByteArray &key) {
        m_key = key;
        m_keyHex = key.toHex(' ');
        emit keyChanged();
    }
    const QByteArray& keyHex() const {
        return m_keyHex;
    }
    void setKeyHex(const QByteArray& data) {
        setKey(QByteArray::fromHex(data));
    }
    const QByteArray& data() const {
        return m_data;
    }
    void setData(const QByteArray &data) {
        m_data = data;
        m_dataHex = data.toHex(' ');
        emit dataChanged();
    }
    const QByteArray& dataHex() const {
        return m_dataHex;
    }
    void setDataHex(const QByteArray& data) {
        setData(QByteArray::fromHex(data));
    }


signals:
    void urlChanged();
    void keyChanged();
    void dataChanged();
public slots:
    bool saveData(bool asHex = false) {
        return save(asHex ? m_dataHex : m_data);
    }
    bool loadData(bool asHex = false) {
        return load(asHex ? &Dencrypter::setDataHex : &Dencrypter::setData);
    }
    bool saveKey(bool asHex = false) {
        return save(asHex ? m_keyHex : m_key);
    }
    bool loadKey(bool asHex = false) {
        return load(asHex ? &Dencrypter::setKeyHex : &Dencrypter::setKey);
    }
    bool run(const QString& path) {
        QProcess runner;
        runner.start(path);
        if (!runner.waitForStarted())
            return false;

        runner.write(m_key);
        runner.write(m_data);
        runner.closeWriteChannel();

        if (!runner.waitForFinished())
            return false;

        setData(runner.readAll());
        return true;
    }
    void generateKey(int size) {
        QByteArray key;
        for(int i = 0; i < size; i++) {
            key.append(rand() % 255 + 1);
        }
        setKey(key);
    }
private:
    bool save(const QByteArray& data) {
        QFile file(m_url.toLocalFile());
        if (!file.open(QIODevice::WriteOnly))
            return false;
        file.write(data);
        return true;
    }
    bool load(void (Dencrypter::*f)(const QByteArray&)) {
        QFile file(m_url.toLocalFile());
        if (!file.open(QIODevice::ReadOnly))
            return false;
        (this->*f)(file.readAll());
        return true;
    }

    QUrl m_url;
    QByteArray m_key, m_keyHex, m_data, m_dataHex;
};


#endif // FILEIO_H

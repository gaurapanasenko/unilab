#ifndef FILEIO_H
#define FILEIO_H

#include <QQuickItem>
#include <QUrl>
#include <QFile>
#include <QProcess>


class Signer : public QQuickItem // name is just a joke
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QByteArray keyPair READ keyPair WRITE setKeyPair NOTIFY keyPairChanged)
    Q_PROPERTY(QByteArray keyPairHex READ keyPairHex WRITE setKeyPairHex NOTIFY keyPairChanged)
    Q_PROPERTY(QByteArray signature READ signature WRITE setSignature NOTIFY signatureChanged)
    Q_PROPERTY(QByteArray signatureHex READ signatureHex WRITE setSignatureHex NOTIFY signatureChanged)
    Q_PROPERTY(QByteArray data READ data WRITE setData NOTIFY dataChanged)
    Q_PROPERTY(QByteArray dataHex READ dataHex WRITE setDataHex NOTIFY dataChanged)
public:
    explicit Signer() {
    }
    ~Signer() {}


    const QUrl& url() const {
        return m_url;
    }
    void setUrl(const QUrl& url) {
        m_url = url;
        emit urlChanged();
    }


    const QByteArray& keyPair() const {
        return m_keyPair;
    }
    void setKeyPair(const QByteArray &keyPair) {
        m_keyPair = keyPair;
        m_keyPairHex = keyPair.toHex(' ');
        emit keyPairChanged();
    }
    const QByteArray& keyPairHex() const {
        return m_keyPairHex;
    }
    void setKeyPairHex(const QByteArray& data) {
        setKeyPair(QByteArray::fromHex(data));
    }


    const QByteArray& signature() const {
        return m_signature;
    }
    void setSignature(const QByteArray &signature) {
        m_signature = signature;
        m_signatureHex = signature.toHex(' ');
        emit signatureChanged();
    }
    const QByteArray& signatureHex() const {
        return m_signatureHex;
    }
    void setSignatureHex(const QByteArray& data) {
        setSignature(QByteArray::fromHex(data));
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
    void keyPairChanged();
    void signatureChanged();
    void dataChanged();
public slots:
    bool saveData(bool asHex = false) {
        return save(asHex ? m_dataHex : m_data);
    }
    bool loadData(bool asHex = false) {
        return load(asHex ? &Signer::setDataHex : &Signer::setData);
    }


    bool saveKeyPair(bool asHex = false) {
        return save(asHex ? m_keyPairHex : m_keyPair);
    }
    bool loadKeyPair(bool asHex = false) {
        return load(asHex ? &Signer::setKeyPairHex : &Signer::setKeyPair);
    }


    bool saveSignature(bool asHex = false) {
        return save(asHex ? m_signatureHex : m_signature);
    }
    bool loadSignature(bool asHex = false) {
        return load(asHex ? &Signer::setSignatureHex : &Signer::setSignature);
    }


    bool sign(const QString& path) {
        return run(path, m_keyPair + m_data, &Signer::setSignature);
    }
    bool check(const QString& path) {
        return run(path, m_signature + m_data, &Signer::setData);
    }

    bool generateKey(const QString& path) {
        return run(path, "", &Signer::setKeyPair);
    }
private:
    bool run(const QString& path, const QByteArray& data,
             void (Signer::*f)(const QByteArray&)) {
        QProcess runner;
        runner.start(path);
        if (!runner.waitForStarted())
            return false;

        qDebug() << data;
        runner.write(data);
        runner.closeWriteChannel();

        if (!runner.waitForFinished())
            return false;

        QByteArray dat, tmp;
        while (true) {
            tmp = runner.readAll();
            if (!tmp.length()) break;
            dat.append(tmp);
        }
        (this->*f)(dat);
        return true;
    }
    bool save(const QByteArray& data) {
        QFile file(m_url.toLocalFile());
        if (!file.open(QIODevice::WriteOnly))
            return false;
        file.write(data);
        return true;
    }
    bool load(void (Signer::*f)(const QByteArray&)) {
        QFile file(m_url.toLocalFile());
        if (!file.open(QIODevice::ReadOnly))
            return false;
        (this->*f)(file.readAll());
        return true;
    }

    QUrl m_url;
    QByteArray m_keyPair, m_keyPairHex, m_signature, m_signatureHex, m_data, m_dataHex;
};


#endif // FILEIO_H

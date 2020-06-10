#ifndef FILEIO_H
#define FILEIO_H

#include <QQuickItem>
#include <QUrl>
#include <QFile>
#include <QProcess>
#include <QDebug>


class Signer : public QQuickItem // name is just a joke
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(bool signStatus READ signStatus WRITE setSignStatus NOTIFY signStatusChanged)
    Q_PROPERTY(QByteArray pubKey READ pubKey WRITE setPubKey NOTIFY pubKeyChanged)
    Q_PROPERTY(QByteArray pubKeyHex READ pubKeyHex WRITE setPubKeyHex NOTIFY pubKeyChanged)
    Q_PROPERTY(QByteArray privKey READ privKey WRITE setPrivKey NOTIFY privKeyChanged)
    Q_PROPERTY(QByteArray privKeyHex READ privKeyHex WRITE setPrivKeyHex NOTIFY privKeyChanged)
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

    bool signStatus() const {
        return m_signStatus;
    }
    void setSignStatus(bool signStatus) {
        m_signStatus = signStatus;
        emit signStatusChanged();
    }


    const QByteArray& pubKey() const {
        return m_pubKey;
    }
    void setPubKey(const QByteArray &pubKey) {
        m_pubKey = pubKey;
        m_pubKeyHex = pubKey.toHex(' ');
        emit pubKeyChanged();
    }
    const QByteArray& pubKeyHex() const {
        return m_pubKeyHex;
    }
    void setPubKeyHex(const QByteArray& data) {
        setPubKey(QByteArray::fromHex(data));
    }


    const QByteArray& privKey() const {
        return m_privKey;
    }
    void setPrivKey(const QByteArray &privKey) {
        m_privKey = privKey;
        m_privKeyHex = privKey.toHex(' ');
        emit privKeyChanged();
    }
    const QByteArray& privKeyHex() const {
        return m_privKeyHex;
    }
    void setPrivKeyHex(const QByteArray& data) {
        setPrivKey(QByteArray::fromHex(data));
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
    void signStatusChanged();
    void pubKeyChanged();
    void privKeyChanged();
    void signatureChanged();
    void dataChanged();
public slots:
    bool saveData(bool asHex = false) {
        return save(asHex ? m_dataHex : m_data);
    }
    bool loadData(bool asHex = false) {
        return load(asHex ? &Signer::setDataHex : &Signer::setData);
    }


    bool savePubKey(bool asHex = false) {
        return save(asHex ? m_pubKeyHex : m_pubKey);
    }
    bool loadPubKey(bool asHex = false) {
        return load(asHex ? &Signer::setPubKeyHex : &Signer::setPubKey);
    }


    bool savePrivKey(bool asHex = false) {
        return save(asHex ? m_privKeyHex : m_privKey);
    }
    bool loadPrivKey(bool asHex = false) {
        return load(asHex ? &Signer::setPrivKeyHex : &Signer::setPrivKey);
    }


    bool saveSignature(bool asHex = false) {
        return save(asHex ? m_signatureHex : m_signature);
    }
    bool loadSignature(bool asHex = false) {
        return load(asHex ? &Signer::setSignatureHex : &Signer::setSignature);
    }


    bool sign(const QString& path) {
        return run(path, m_pubKey + m_data, &Signer::setSignature);
    }
    bool check(const QString& path) {
        return run(path, m_privKey + m_signature + m_data, &Signer::setSignStatusPriv);
    }

    bool generateKey(const QString& path) {
        return run(path, "", &Signer::setKeyPair);
    }
private:
    void setSignStatusPriv(const QByteArray& data) {
        QByteArray tr("True\n");
        qDebug() << data << tr;
        if (data == tr)
            setSignStatus(true);
        else setSignStatus(false);
    }
    void setKeyPair(const QByteArray& data) {
        setPubKey(data.left(80));
        setPrivKey(data.right(80));
    }
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
    bool m_signStatus;
    QByteArray m_privKey, m_privKeyHex, m_pubKey, m_pubKeyHex, m_signature, m_signatureHex, m_data, m_dataHex;
};


#endif // FILEIO_H

#ifndef S3S_H
#define S3S_H

#include <QFile>
#include <QMutex>
#include <QLabel>
#include <QObject>
#include <QBasicTimer>
#include <QTimerEvent>
#include <QApplication>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QDnsLookup>

#define REQUEST_TYPE "type"

///
/// a clickable label for icon-buttons
///
class s3s_ClickableLabel : public QLabel{
       Q_OBJECT
    public:
       s3s_ClickableLabel(QString text, QWidget *parent = nullptr) : QLabel(text, parent) {}
       ~s3s_ClickableLabel(){}

    signals:
       void clicked();

    protected:
       void mousePressEvent(QMouseEvent *event) { Q_UNUSED(event); emit clicked(); }
};

///
/// timeout for QDnsLookup
///
class s3s_LookupTimeout : public QObject {
    Q_OBJECT
public:
    s3s_LookupTimeout(QDnsLookup* dns, const int timeout) :
        QObject(dns)
    {
        Q_ASSERT(dns);
        if (dns && !dns->isFinished())
            m_timer.start(timeout, this);
    }
    static void set(QDnsLookup* dns, const int timeout){
        new s3s_LookupTimeout(dns, timeout);
    }

protected:
    QBasicTimer m_timer;
    void timerEvent(QTimerEvent * ev) {
        if (!m_timer.isActive() || ev->timerId() != m_timer.timerId())
            return;
        QDnsLookup* dns = static_cast<QDnsLookup*>(parent());
        if (!dns->isFinished())
        {
            dns->abort();
            m_timer.stop();
        }
    }
};

///
/// timeout for QNetworkReply custom implementation since versions < Qt 5.15 have no reply timer
///
class s3s_ReplyTimeout : public QObject {
    Q_OBJECT
public:
    enum HandleMethod { Abort, Close };
    s3s_ReplyTimeout(QNetworkReply* reply, const int timeout, HandleMethod method = Abort) :
        QObject(reply), m_method(method)
    {
        Q_ASSERT(reply);
        if (reply && reply->isRunning())
            m_timer.start(timeout, this);
    }
    static void set(QNetworkReply* reply, const int timeout, HandleMethod method = Abort)
    {
        new s3s_ReplyTimeout(reply, timeout, method);
    }

protected:
    QBasicTimer m_timer;
    HandleMethod m_method;
    void timerEvent(QTimerEvent * ev) {
        if (!m_timer.isActive() || ev->timerId() != m_timer.timerId())
            return;
        QNetworkReply *reply = static_cast<QNetworkReply*>(parent());
        if (reply->isRunning())
        {
            if (m_method == Close)
                reply->close();
            else if (m_method == Abort)
                reply->abort();
            m_timer.stop();
        }
    }
};

///
///  a custom QNetworkAccessManager able to set a property of the reply from request & timeout
///
class s3sNetworkAccessManager: public QNetworkAccessManager {
    public:
        s3sNetworkAccessManager(QObject *parent = nullptr, int timeout = 1000, bool use_timer = false): QNetworkAccessManager(parent),
            m_timeout(timeout),
            m_use_timer(use_timer)
        {
        }

    protected:
        QNetworkReply* createRequest(Operation op, const QNetworkRequest &request, QIODevice *data = nullptr)
        {
            QNetworkReply *reply = QNetworkAccessManager::createRequest(op, request, data);

            /* set timeout */
            if(m_use_timer)
                s3s_ReplyTimeout::set(reply, m_timeout);

            /* set property */
            reply->setProperty(REQUEST_TYPE, request.attribute(QNetworkRequest::User));

            return reply;
        }

    private:
        int m_timeout;
        bool m_use_timer;
};

#endif // S3S_H

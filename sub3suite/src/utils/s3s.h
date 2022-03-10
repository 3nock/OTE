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

#define REQUEST_TYPE "type"

///
/// a custom QApplication with exceptions handling
///
class s3s_Application final: public QApplication
{
public:
    s3s_Application(int &argc, char **argv) : QApplication(argc, argv) {}

    bool notify(QObject* receiver, QEvent* event) override
    {
        try {
            return QApplication::notify(receiver, event);
        }
        catch (std::exception &e) {
            qFatal("Error %s sending event %s to object %s (%s)",
                e.what(), typeid(*event).name(), qPrintable(receiver->objectName()),
                typeid(*receiver).name());
        }
        catch (...) {
            qFatal("Error <unknown> sending event %s to object %s (%s)",
                typeid(*event).name(), qPrintable(receiver->objectName()),
                typeid(*receiver).name());
        }
         return false;
    }
};


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
/// timeout for QNetworkReply(custom implementation since versions < Qt 5.15 have no reply timer
///
class s3s_ReplyTimeout : public QObject {
    Q_OBJECT
public:
    enum HandleMethod { Abort, Close };
    s3s_ReplyTimeout(QNetworkReply* reply, const int timeout, HandleMethod method = Abort) :
        QObject(reply), m_method(method)
    {
        Q_ASSERT(reply);
        if (reply && reply->isRunning()) {
            m_timer.start(timeout, this);
            connect(reply, &QNetworkReply::finished, this, &QObject::deleteLater);
        }
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
        auto reply = static_cast<QNetworkReply*>(parent());
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
///  a custom QNetworkAccessManager able to set a property of the reply from request
///
class s3sNetworkAccessManager: public QNetworkAccessManager {
    public:
        s3sNetworkAccessManager(QObject *parent = nullptr): QNetworkAccessManager(parent)
        {
        }

    protected:
        QNetworkReply* createRequest(Operation op, const QNetworkRequest &request, QIODevice *data = nullptr)
        {
            QNetworkReply *reply = QNetworkAccessManager::createRequest(op, request, data);
            /* using the timeout
             * s3s_ReplyTimeout::set(reply, 1000);
             */
            reply->setProperty(REQUEST_TYPE, request.attribute(QNetworkRequest::User));
            return reply;
        }
};

#endif // S3S_H

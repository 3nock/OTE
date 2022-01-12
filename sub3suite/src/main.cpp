/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :main function which launches the app.
*/

#include "MainWindow.h"
#include "src/utils/Config.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QSplashScreen>


/* a custom messagehandler for logging messages to log file */
void s3s_MessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
    QString log;
    QString time(QDateTime::currentDateTime().toString("hh:mm:ss"));

    switch (type) {
    case QtInfoMsg:
        log = QString("[%1] INFO: %2").arg(time).arg(msg);
        break;
    case QtDebugMsg:
        log = QString("[%1] DEBUG: %2").arg(time).arg(msg);
        break;
    case QtWarningMsg:
        log = QString("[%1] WARNING: %2").arg(time).arg(msg);
        break;
    case QtCriticalMsg:
        log = QString("[%1] CRITICAL: %2").arg(time).arg(msg);
        break;
    case QtFatalMsg:
        log = QString("[%1] FATAL: %2").arg(time).arg(msg);
        break;
    }

    /*
     * since the messagehandler isn't thread safe
     * introducing mutex lock
     */
    QMutex mutex;
    mutex.lock();

    QString date = QDateTime::currentDateTime().toString("dd-MM-yyyy");
    QFile logfile(QDir::currentPath()+"/logs/"+date+".log");

    if(logfile.open(QIODevice::WriteOnly | QIODevice::Append)){
        QTextStream ts(&logfile);
        ts << log << endl;
        logfile.flush();
        logfile.close();
    }

    mutex.unlock();
}

/* a custom QApplication with exceptions handling */
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


int main(int argc, char *argv[])
{
    /* installing the message handler */
    qInstallMessageHandler(s3s_MessageHandler);

    /* Handle DPI scaling on Windows */
#if defined(Q_OS_WIN)
    s3s_Application::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    /* create the sub3suite app */
    s3s_Application s3s_app(argc, argv);

    /* splash screen */
    QPixmap splashImage = QPixmap(":/img/res/icons/splash.png");
    splashImage.scaledToWidth(600);
    QSplashScreen splash(splashImage);
    splash.show();

    /* removing context help button from all on dialogs... */
    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);

    /* starting the application */
    MainWindow w;

    /* setting the app to the center of Screen on start */
    int x = (s3s_app.desktop()->width()-w.width()) / 2;
    int y = (s3s_app.desktop()->height()-w.height()) / 2;
    w.move(x, y-35);
    w.show();

    /* splashscreen timing */
    splash.finish(&w);

    /* setting configurations...*/
    CONFIG;

    /* starting the app... */
    qInfo() << "starting sub3suite...";
    return s3s_app.exec();
}

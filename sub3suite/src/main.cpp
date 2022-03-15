/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :main function which launches the app.
*/

#include "MainWindow.h"
#include "src/utils/Config.h"
#include "src/dialogs/StartupDialog.h"

#include <QMap>
#include <QApplication>
#include <QDesktopWidget>
#include <QSplashScreen>

#include "src/utils/s3s.h"

///
/// a custom messagehandler for logging messages to log file
///
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
    QFile logfile(QApplication::applicationDirPath()+"/logs/"+date+".log");

    if(logfile.open(QIODevice::WriteOnly | QIODevice::Append)){
        QTextStream ts(&logfile);
        ts << log << endl;
        logfile.flush();
        logfile.close();
    }

    mutex.unlock();
}

int main(int argc, char *argv[])
{
    /* high scalling */
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");
    s3s_Application::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0) && defined(Q_OS_WIN)
    if (qgetenv("QT_SCALE_FACTOR_ROUNDING_POLICY").isEmpty())
        QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif

    /* removing context help button from all on dialogs for now */
    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);

    /* installing the message handler */
    qInstallMessageHandler(s3s_MessageHandler);

    /* app info */
    QCoreApplication::setOrganizationName("Sub3 Suite");
    QCoreApplication::setApplicationName("Sub3 Suite");
    QCoreApplication::setApplicationVersion("1.0.0");
    QCoreApplication::setOrganizationDomain("github.com/3nock/sub3suite");

    /* create the sub3suite app */
    s3s_Application s3s_app(argc, argv);

    int dpi = qApp->desktop()->logicalDpiX();
    dpi = static_cast<int>(dpi/96);

    /* setting stylesheets */
    QFile stylesheet(":/themes/res/themes/default.css");
    if(stylesheet.open(QFile::ReadOnly)){
        qApp->setStyleSheet(QLatin1String(stylesheet.readAll()));
        stylesheet.close();
    }

    /* splash screen */
    QPixmap splashImage = QPixmap(":/img/res/icons/splash.png");
    splashImage.scaledToWidth(600);
    QSplashScreen splash(splashImage);
    splash.show();

    /* init configurations */
    CONFIG;

    /* project */
    ProjectStruct project;

    /* startup dialog */
    StartupDialog startupDialog(&project);
    startupDialog.setModal(true);

    /* splashscreen timing */
    splash.finish(&startupDialog);

    /* start startupDialog */
    startupDialog.exec();

    if(project.isNew || project.isExisting || project.isTemporary)
    {
        /* creating the main window */
        MainWindow w;
        w.setWindowState(Qt::WindowMaximized);
        w.show();

        /* opening the project */
        w.initProject(project);

        /* starting the app */
        qInfo() << "starting sub3suite...";
        return s3s_app.exec();
    }
    else
        s3s_app.quit();
}

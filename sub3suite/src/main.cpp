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
#include <QStandardPaths>
#include <QTranslator>

#if defined(Q_OS_WIN)
#include <Windows.h>
#endif

///
/// logging device informations...
///
void log_device_info(){
    /* display info.. */
    qDebug() << "physical dpi value X: " << qApp->desktop()->physicalDpiX();
    qDebug() << "physical dpi value Y: " << qApp->desktop()->physicalDpiY();
    qDebug() << "Logical dpi value X: " << qApp->desktop()->logicalDpiX();
    qDebug() << "Logical dpi value Y: " << qApp->desktop()->logicalDpiY();
    qDebug() << "device-pixel Ratio: " << qApp->desktop()->devicePixelRatio();
}

///
/// Check Sub3 Suite permissions/privileges
///
void check_priv(){
#if defined (Q_OS_WIN)
    LUID priv_id;
    HANDLE hToken = nullptr;
    PRIVILEGE_SET privs;
    if(LookupPrivilegeValue(nullptr, SE_CREATE_GLOBAL_NAME, &priv_id)){
        privs.PrivilegeCount = 1;
        privs.Control = PRIVILEGE_SET_ALL_NECESSARY;
        privs.Privilege[0].Luid = priv_id;
        privs.Privilege[0].Attributes = SE_PRIVILEGE_ENABLED;
        if ( ! OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hToken))
            qWarning() << "can_switch_ids: OpenProcessToken failed error: " << GetLastError();
        else {
            BOOL fEnabled = false;
            if (!PrivilegeCheck(hToken, &privs, &fEnabled))
                qWarning() << "can_switch_ids: PrivilegeCheck failed error: " << GetLastError();
            else
                s3s_global::is_priv = fEnabled;
            CloseHandle(hToken);
        }
    }
#else
    if (!geteuid())
        s3s_global::is_priv = true;
    else
        s3s_global::is_priv = false;
#endif
}

void registerMetaTypes(){
    qRegisterMetaType<u_short>("u_short");
    qRegisterMetaType<ScanLog>("ScanLog");
    qRegisterMetaType<scan::Log>("scan::Log");
    qRegisterMetaType<s3s_struct::RAW>("s3s_struct::RAW");
    qRegisterMetaType<s3s_struct::DNS>("s3s_struct::DNS");
    qRegisterMetaType<s3s_struct::IP>("s3s_struct::IP");
    qRegisterMetaType<s3s_struct::ASN>("s3s_struct::ASN");
    qRegisterMetaType<s3s_struct::CIDR>("s3s_struct::CIDR");
    qRegisterMetaType<s3s_struct::NS>("s3s_struct::NS");
    qRegisterMetaType<s3s_struct::MX>("s3s_struct::MX");
    qRegisterMetaType<s3s_struct::URL>("s3s_struct::URL");
    qRegisterMetaType<s3s_struct::HOST>("s3s_struct::HOST");
    qRegisterMetaType<s3s_struct::Email>("s3s_struct::Email");
    qRegisterMetaType<s3s_struct::Wildcard>("s3s_struct::Wildcard");
    qRegisterMetaType<QSslCertificate>("QSslCertificate");
}

void create_desktop_file(){
    if(QDir::isAbsolutePath("/usr/share/applications/")){
        QFile file("/usr/share/applications/sub3suite.desktop");
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
            file.write("[Desktop Entry]\n");
            file.write("Type=Application\n");
            file.write("Exec="+QCoreApplication::applicationFilePath().toUtf8()+"\n");
            file.write("Name=Sub3 Suite\n");
            file.write("GenericName=Advance Intelligence Gathering Tool\n");
            file.write("Icon="+QCoreApplication::applicationDirPath().toUtf8()+"/icon/s3s.png\n");
            file.write("Terminal=false\n");
            file.close();
        }
        else
            qWarning() << "Couldnt open /usr/share/applications/sub3suite.desktop for write";
    }
    else
        qWarning() << "Path /usr/share/applications/ not available";
}

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
	case QtFatalMsg:
        log = QString("[%1] ERROR: %2").arg(time).arg(msg);
        break;
    }

    QMutex mutex;
    mutex.lock();

    QString date = QDateTime::currentDateTime().toString("dd-MM-yyyy");
    QFile logfile(QApplication::applicationDirPath()+"/logs/"+date+".log");
    logfile.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner |
                           QFileDevice::ReadUser | QFileDevice::WriteUser |
                           QFileDevice::ReadGroup | QFileDevice::WriteGroup |
                           QFileDevice::ReadOther | QFileDevice::WriteOther);

    if(logfile.open(QIODevice::WriteOnly | QIODevice::Append)){
        QTextStream ts(&logfile);
        ts << log << endl;
        logfile.flush();
        logfile.close();
    }

    mutex.unlock();
}

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

namespace s3s_global {
    bool is_priv = false;
    bool is_dark_theme = false;
    bool is_light_theme = false;
    int font_size = 0;
}


int main(int argc, char *argv[])
{
    /* dpi scalling */
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

    /* splash screen */
    QPixmap splashImage = QPixmap(":/img/res/icons/splash.png");
    splashImage.scaledToWidth(600);
    QSplashScreen splash(splashImage);
    splash.show();

    qInfo() << "**************************************************************************************";
    qInfo() << "*                                             Sub3 Suite                              ";
    qInfo() << "**************************************************************************************";

    QFile::setPermissions(QGuiApplication::applicationDirPath()+"/sub3suite.ini",
                          QFileDevice::ReadOwner | QFileDevice::WriteOwner |
                          QFileDevice::ReadUser | QFileDevice::WriteUser |
                          QFileDevice::ReadGroup | QFileDevice::WriteGroup |
                          QFileDevice::ReadOther | QFileDevice::WriteOther);

    QFile::setPermissions(QGuiApplication::applicationDirPath()+"/keys.ini",
                          QFileDevice::ReadOwner | QFileDevice::WriteOwner |
                          QFileDevice::ReadUser | QFileDevice::WriteUser |
                          QFileDevice::ReadGroup | QFileDevice::WriteGroup |
                          QFileDevice::ReadOther | QFileDevice::WriteOther);

    if(CONFIG.value("is_first_run").toBool()){
        CONFIG.setValue("is_first_run", false);
        CONFIG.setValue("s3s", QCoreApplication::applicationFilePath());
#if defined (Q_OS_LINUX)
        create_desktop_file();
        qDebug() << "sub3suite.desktop file created!";
#endif
    }


    /* check s3s path */
    if(CONFIG.value("s3s").toString() != QCoreApplication::applicationFilePath()){
        CONFIG.setValue("s3s", QCoreApplication::applicationFilePath());
#if defined (Q_OS_LINUX)
        create_desktop_file();
        qDebug() << "sub3suite.desktop file changed!";
#endif
    }

    check_priv();
    log_device_info();
    registerMetaTypes();

    /* setting font */
    switch (CONFIG.value(CFG_VAL_FONT).toInt()) {
    case 11:
    {
        QFont font = qApp->font();
        font.setPixelSize(11);
        qApp->setFont(font);
        s3s_global::font_size = 11;
    }
        break;
    case 12:
    {
        QFont font = qApp->font();
        font.setPixelSize(12);
        qApp->setFont(font);
        s3s_global::font_size = 12;
    }
        break;
    default:
        s3s_global::font_size = 0;
        break;
    }

    /* setting stylesheets */
    QFile stylesheet;
    if(CONFIG.value(CFG_VAL_THEME).toString() == "dark"){
        stylesheet.setFileName(":/themes/res/themes/default.css");
        s3s_global::is_dark_theme = true;
    }
    else{
        stylesheet.setFileName(":/themes/res/themes/light.css");
        s3s_global::is_light_theme = true;
    }

    if(stylesheet.open(QFile::ReadOnly)){
        qApp->setStyleSheet(QLatin1String(stylesheet.readAll()));
        stylesheet.close();
    }

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
        splash.show();
        /* creating the main window */
        MainWindow w;
        w.setWindowState(Qt::WindowMaximized);
        splash.finish(&w);
        w.show();

        /* opening the project */
        w.initProject(project);

        return s3s_app.exec();
    }
    else
        s3s_app.quit();
}

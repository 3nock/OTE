//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "MainWindow.h"

#include <QApplication>
#include <QMutex>
#include <QMutexLocker>

#include "src/core/Config.h"
#include "src/core/Themes.h"
#include "src/core/Utility.h"
#include "src/template/Template.h"

static QMutex gMessageMutex;

void MessageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    QString log;
    QString time(QDateTime::currentDateTime().toString("hh:mm:ss"));

    QMutexLocker locker(&gMessageMutex);

    if(LogViewerDialog::logsTextEdit)
    {
        switch (type) {
        case QtInfoMsg:
            log = QString("[%1] <b><font color=\"blue\">INFO: </font></b> %2").arg(time, msg);
            break;
        case QtDebugMsg:
            log = QString("[%1] <b><font color=\"green\">DEBUG: </font></b> %2").arg(time, msg);
            break;
        case QtWarningMsg:
            log = QString("[%1] <b><font color=\"yellow\">WARNING: </font></b> %2").arg(time, msg);
            break;
        case QtCriticalMsg:
            log = QString("[%1] <b><font color=\"yellow\">CRITICAL: </font></b> %2").arg(time, msg);
            break;
        case QtFatalMsg:
            log = QString("[%1] <b><font color=\"red\">FATAL: </font></b> %2").arg(time, msg);
            break;
        }
        LogViewerDialog::logsTextEdit->appendHtml(log);
    }
    else
    {
        switch (type) {
        case QtInfoMsg:
            log = QString("[%1] INFO: %2").arg(time, msg);
            break;
        case QtDebugMsg:
            log = QString("[%1] DEBUG: %2").arg(time, msg);
            break;
        case QtWarningMsg:
            log = QString("[%1] WARNING: %2").arg(time, msg);
            break;
        case QtCriticalMsg:
            log = QString("[%1] CRITICAL: %2").arg(time, msg);
            break;
        case QtFatalMsg:
            log = QString("[%1] FATAL: %2").arg(time, msg);
            break;
        }
        fprintf(stderr, "%s", log.toLocal8Bit().data());
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
/**
 * @brief setupHighDPI, dpi scalling for high screen resolutions
 */
void setupHighDPI()
{
    bool has_env_setting = false;
    QStringList env_vars;
    env_vars << "QT_ENABLE_HIGHDPI_SCALING"
             << "QT_SCALE_FACTOR_ROUNDING_POLICY"
             << "QT_AUTO_SCREEN_SCALE_FACTOR"
             << "QT_SCALE_FACTOR"
             << "QT_SCREEN_SCALE_FACTORS"
             << "QT_DEVICE_PIXEL_RATIO";

    foreach(QString v, env_vars)
    {
        if (!OTE::Utility::getEnvironmentVar(v).isEmpty())
        {
            has_env_setting = true;
            break;
        }
    }

    //int highDPI = gConfig.highDPI;
    int highDPI = 1;
    if (highDPI == 1 || (highDPI == 0 && !has_env_setting))
    {
        // Turning on Automatic High DPI scaling
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    }
    else if (highDPI == 2)
    {
        // Turning off Automatic High DPI scaling
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, false);
        foreach(QString v, env_vars)
        {
            bool irrel = qunsetenv(v.toUtf8().constData());
            Q_UNUSED(irrel)
        }
    }
}
#endif

// the logic is tightly coupled to the widgets in this project

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifndef Q_OS_MAC
    setupHighDPI();
#endif
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    // removing context help button from all on dialogs for now
    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton, true);

    // installing the message handler
    //qInstallMessageHandler(MessageHandler);

    // app info
    QCoreApplication::setOrganizationName("SpiderSuite");
    QCoreApplication::setApplicationName("OTE");
    QCoreApplication::setApplicationVersion("1.0.0");
    QCoreApplication::setOrganizationDomain("https://SpiderSuite.github.io/OTE");

    QApplication app(argc, argv);

    // obtain configurations
    OTE::gConfig.fromConfigFile();

    // We set the window icon explicitly on Linux.
    // On Windows this is handled by the RC file,
    // and on Mac by the ICNS file.
#if !defined(Q_OS_WIN32) && !defined(Q_OS_MAC)
    app.setWindowIcon(Utility::getApplicationIcon());
#if QT_VERSION >= 0x050700
    // Wayland needs this clarified in order to propery assign the icon
    app.setDesktopFileName(QStringLiteral("OTE.desktop"));
#endif
#endif

    // set style theme
    if(OTE::gConfig.general.theme == OTE::THEME::LIGHT)
        QApplication::setStyle(new OTE::LightStyle);
    else
        QApplication::setStyle(new OTE::DarkStyle);

    // Set ui font from preferences after theming
    QFont f = QFont(QApplication::font());
    f.setFamily(OTE::gConfig.general.font_name);
    f.setPointSize(OTE::gConfig.general.font_size);
    f.setStyleName(OTE::gConfig.general.font_style);
    QApplication::setFont(f);

    OTE::Template::loadTemplates();

    MainWindow w;
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return app.exec();
}

#-------------------------------------------------
#
# Project created by QtCreator 2020-12-07T00:44:51
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = subsuite
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

include(includes/gumbo/gumbo-parser.pri)

SOURCES += \
    src/dialogs/OsintConfigDialog.cpp \
    src/engines/osint/modules/Anubis.cpp \
    src/engines/osint/modules/ArchiveIt.cpp \
    src/engines/osint/modules/ArchiveToday.cpp \
    src/engines/osint/modules/Certspotter.cpp \
    src/engines/osint/modules/Crtsh.cpp \
    src/engines/osint/modules/Dnsbufferoverun.cpp \
    src/engines/osint/modules/Dnsdumpster.cpp \
    src/engines/osint/modules/GoogleCert.cpp \
    src/engines/osint/modules/Hackertarget.cpp \
    src/engines/osint/modules/Netcraft.cpp \
    src/engines/osint/modules/Omnisint.cpp \
    src/engines/osint/modules/Otx.cpp \
    src/engines/osint/modules/Pkey.cpp \
    src/engines/osint/modules/Projectdiscovery.cpp \
    src/engines/osint/modules/Qwant.cpp \
    src/engines/osint/modules/Rapiddns.cpp \
    src/engines/osint/modules/Spyse.cpp \
    src/engines/osint/modules/Sublist3r.cpp \
    src/engines/osint/modules/Suip.cpp \
    src/engines/osint/modules/Threatcrowd.cpp \
    src/engines/osint/modules/Threatminer.cpp \
    src/engines/osint/modules/Urlscan.cpp \
    src/engines/osint/modules/VirusTotal.cpp \
    src/engines/osint/modules/Waybackmachine.cpp \
        src\engines\ContextMenu.cpp \
        src\Logger.cpp \
        src\project\general\GeneralAnalysis.cpp \
        src\project\specific\SpecificAnalysis.cpp \
        src\utils.cpp \
        src\widgets\InputWidget.cpp \
        src\widgets\ResultsWidget.cpp \
        src\engines\CommonScanners.cpp \
        src\project\ProjectDataModel.cpp \
        src\project\Project.cpp \
        src\engines\Base.cpp \
        src\engines\ip\Ip.cpp \
        src\engines\ip\IpScanner.cpp \
        src\engines\active\Active.cpp \
        src\engines\active\ActiveScanner.cpp \
        src\engines\dns\DnsRecords.cpp \
        src\engines\dns\DnsRecordsScanner.cpp \
        src\engines\brute\Brute.cpp \
        src\engines\brute\BruteScanner.cpp \
        src\engines\osint\Osint.cpp \
        src\engines\osint\OsintScanner.cpp \
        src\dialogs\ConfigDialog.cpp \
        src\dialogs\ApiKeysDialog.cpp \
        src\dialogs\WordlistDialog.cpp \
        src\SplashScreen.cpp \
        src\MainWindow.cpp \
        src\main.cpp \

HEADERS += \
    src/Config.h \
    src/dialogs/OsintConfigDialog.h \
    src/engines/osint/modules/AbstractModule.h \
    src/engines/osint/modules/Anubis.h \
    src/engines/osint/modules/ArchiveIt.h \
    src/engines/osint/modules/ArchiveToday.h \
    src/engines/osint/modules/Certspotter.h \
    src/engines/osint/modules/Crtsh.h \
    src/engines/osint/modules/Dnsbufferoverun.h \
    src/engines/osint/modules/Dnsdumpster.h \
    src/engines/osint/modules/GoogleCert.h \
    src/engines/osint/modules/Hackertarget.h \
    src/engines/osint/modules/Netcraft.h \
    src/engines/osint/modules/Omnisint.h \
    src/engines/osint/modules/Otx.h \
    src/engines/osint/modules/Pkey.h \
    src/engines/osint/modules/Projectdiscovery.h \
    src/engines/osint/modules/Qwant.h \
    src/engines/osint/modules/Rapiddns.h \
    src/engines/osint/modules/Spyse.h \
    src/engines/osint/modules/Sublist3r.h \
    src/engines/osint/modules/Suip.h \
    src/engines/osint/modules/Threatcrowd.h \
    src/engines/osint/modules/Threatminer.h \
    src/engines/osint/modules/Urlscan.h \
    src/engines/osint/modules/VirusTotal.h \
    src/engines/osint/modules/Waybackmachine.h \
        src\engines\AbstractClass.h \
        src\engines\ContextMenu.h \
        src\Logger.h \
        src\project\general\GeneralAnalysis.h \
        src\project\specific\SpecificAnalysis.h \
        src\utils.h \
        src\widgets\InputWidget.h \
        src\widgets\ResultsWidget.h \
        src\engines\CommonScanners.h \
        src\project\ProjectDataModel.h \
        src\project\Project.h \
        src\engines\Base.h \
        src\engines\ip\Ip.h \
        src\engines\ip\IpScanner.h \
        src\engines\active\Active.h \
        src\engines\active\ActiveScanner.h \
        src\engines\dns\DnsRecords.h \
        src\engines\dns\DnsRecordsScanner.h \
        src\engines\brute\Brute.h \
        src\engines\brute\BruteScanner.h \
        src\engines\osint\Osint.h \
        src\engines\osint\OsintScanner.h \
        src\dialogs\ApiKeysDialog.h \
        src\dialogs\ConfigDialog.h \
        src\dialogs\WordlistDialog.h \
        src\SplashScreen.h \
        src\MainWindow.h \

FORMS += \
        src/dialogs/OsintConfigDialog.ui \
        src\project\general\GeneralAnalysis.ui \
        src\project\specific\SpecificAnalysis.ui \
        src\widgets\InputWidget.ui \
        src\widgets\ResultsWidget.ui \
        src\project\Project.ui \
        src\engines\ip\Ip.ui \
        src\engines\dns\DnsRecords.ui \
        src\engines\brute\Brute.ui \
        src\engines\osint\Osint.ui \
        src\engines\active\Active.ui \
        src\dialogs\ConfigDialog.ui \
        src\dialogs\WordlistDialog.ui \
        src\dialogs\ApiKeysDialog.ui \
        src\MainWindow.ui

# including the python3 headers and libs for compile time...
INCLUDEPATH += -I ../include/python3/include

LIBS += $$OUT_PWD/libs/python38.lib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

# setting the icon...
RC_ICONS = res/icons/main.ico

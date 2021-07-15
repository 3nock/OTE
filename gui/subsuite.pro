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

SOURCES += \
        src\engines\save\Save.cpp \
        src\engines\ip\Ip.cpp \
        src\engines\ip\IpEnumerator.cpp \
        src\engines\active\Active.cpp \
        src\engines\active\ActiveEnumerator.cpp \
        src\engines\level\Level.cpp \
        src\engines\level\lib-level.cpp \
        src\engines\level\LevelEnumerator.cpp \
        src\engines\dns\Dns.cpp \
        src\engines\dns\lib-dns.cpp \
        src\engines\dns\DnsEnumerator.cpp \
        src\engines\brute\Brute.cpp \
        src\engines\brute\lib-brute.cpp \
        src\engines\brute\BruteEnumerator.cpp \
        src\engines\osint\Osint.cpp \
        src\engines\osint\lib-osint.cpp \
        src\engines\osint\OsintEnumerator.cpp \
        src\dialogs\ConfigDialog.cpp \
        src\dialogs\ApiKeysDialog.cpp \
        src\dialogs\WordlistDialog.cpp \
        src\dialogs\WordlistGeneratorDialog.cpp \
        src\SplashScreen.cpp \
        src\MainWindow.cpp \
        src\core.cpp \
        src\main.cpp \

HEADERS += \
        src\engines\save\Save.h \
        src\engines\ip\Ip.h \
        src\engines\ip\IpEnumerator.h \
        src\engines\active\Active.h \
        src\engines\active\lib-active.h \
        src\engines\active\ActiveEnumerator.h \
        src\engines\level\Level.h \
        src\engines\level\lib-level.h \
        src\engines\level\LevelEnumerator.h \
        src\engines\dns\Dns.h \
        src\engines\dns\lib-dns.h \
        src\engines\dns\DnsEnumerator.h \
        src\engines\brute\Brute.h \
        src\engines\brute\lib-brute.h \
        src\engines\brute\BruteEnumerator.h \
        src\engines\osint\Osint.h \
        src\engines\osint\lib-osint.h \
        src\engines\osint\OsintEnumerator.h \
        src\dialogs\ApiKeysDialog.h \
        src\dialogs\ConfigDialog.h \
        src\dialogs\WordlistDialog.h \
        src\dialogs\WordlistGeneratorDialog.h \
        src\SplashScreen.h \
        src\MainWindow.h \
        src\core.h

FORMS += \
        src\engines\ip\Ip.ui \
        src\engines\dns\Dns.ui \
        src\engines\save\Save.ui \
        src\engines\level\Level.ui \
        src\engines\brute\Brute.ui \
        src\engines\osint\Osint.ui \
        src\engines\active\Active.ui \
        src\dialogs\ConfigDialog.ui \
        src\dialogs\WordlistDialog.ui \
        src\dialogs\WordlistGeneratorDialog.ui \
        src\dialogs\ApiKeysDialog.ui \
        src\MainWindow.ui

# including the python3 headers and libs for compile time...
INCLUDEPATH += -I ../include/python3/include

LIBS += -L ../subsuite/include/python3/libs -lpython38

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

# setting the icon...
RC_ICONS = res/icons/main.ico

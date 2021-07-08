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
        src\ActiveTab\ActiveEnumerator.cpp \
        src\ActiveTab\Active.cpp \
        src\LevelTab\LevelEnumerator.cpp \
        src\LevelTab\lib-level.cpp \
        src\DnsTab\DnsEnumerator.cpp \
        src\DnsTab\lib-dns.cpp \
        src\LevelTab\Level.cpp \
        src\BruteTab\lib-brute.cpp \
        src\OsintTab\lib-osint.cpp \
        src\BruteTab\BruteEnumerator.cpp \
        src\BruteTab\Brute.cpp \
        src\OsintTab\OsintEnumerator.cpp \
        src\OsintTab\Osint.cpp \
        src\DnsTab\Dns.cpp \
        src\SaveTab\Save.cpp \
		src\Dialogs\ConfigDialog.cpp \
        src\Dialogs\WordlistDialog.cpp \
		src\Dialogs\ApiKeysDialog.cpp \
        src\Dialogs\WordlistGeneratorDialog.cpp \
        src\core.cpp \
		src\main.cpp \
        src\MainWindow.cpp

HEADERS += \
        src\ActiveTab\ActiveEnumerator.h \
        src\ActiveTab\lib-active.h \
        src\ActiveTab\Active.h \
        src\LevelTab\LevelEnumerator.h \
        src\LevelTab\lib-level.h \
        src\DnsTab\DnsEnumerator.h \
        src\DnsTab\lib-dns.h \
        src\LevelTab\Level.h \
        src\BruteTab\lib-brute.h \
        src\OsintTab\lib-osint.h \
        src\BruteTab\BruteEnumerator.h \
        src\BruteTab\Brute.h \
        src\OsintTab\OsintEnumerator.h \
        src\OsintTab\Osint.h \
        src\DnsTab\Dns.h \
        src\SaveTab\Save.h \
		src\Dialogs\ApiKeysDialog.h \
        src\Dialogs\ConfigDialog.h \
        src\Dialogs\WordlistDialog.h \
		src\Dialogs\WordlistGeneratorDialog.h \
        src\core.h \
        src\MainWindow.h

FORMS += \
        src\ActiveTab\Active.ui \
        src\LevelTab\Level.ui \
        src\BruteTab\Brute.ui \
        src\OsintTab\Osint.ui \
        src\DnsTab\Dns.ui \
        src\SaveTab\Save.ui \
		src\Dialogs\ConfigDialog.ui \
        src\Dialogs\WordlistDialog.ui \
        src\Dialogs\WordlistGeneratorDialog.ui \
        src\Dialogs\ApiKeysDialog.ui \
        src\MainWindow.ui

# including the python headers and libs...
INCLUDEPATH += -I py/include

LIBS += -L ../subsuite/Gui/py/libs -lpython38

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

# setting the icon...
RC_ICONS = res/images/icon.ico

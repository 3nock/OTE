#-------------------------------------------------
#
# Project created by QtCreator 2020-12-07T00:44:51
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sub3num
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
        src\BruteTab\lib-brute.cpp \
        src\OsintTab\lib-osint.cpp \
        src\BruteTab\BruteEnumerator.cpp \
        src\BruteTab\Brute.cpp \
        src\BruteTab\BruteConfigDialog.cpp \
        src\BruteTab\WordlistDialog.cpp \
        src\BruteTab\WordlistGeneratorDialog.cpp \
        src\OsintTab\OsintConfigDialog.cpp \
        src\OsintTab\OsintEnumerator.cpp \
        src\OsintTab\Osint.cpp \
        src\OsintTab\ApiKeysDialog.cpp \
        src\DnsTab\Dns.cpp \
        src\SaveTab\Save.cpp \
        src\main.cpp \
        src\core.cpp \
        src\MainWindow.cpp

HEADERS += \
        src\BruteTab\lib-brute.h \
        src\OsintTab\lib-osint.h \
        src\BruteTab\BruteEnumerator.h \
        src\BruteTab\Brute.h \
        src\BruteTab\BruteConfigDialog.h \
        src\BruteTab\WordlistDialog.h \
        src\BruteTab\WordlistGeneratorDialog.h \
        src\OsintTab\OsintConfigDialog.h \
        src\OsintTab\OsintEnumerator.h \
        src\OsintTab\Osint.h \
        src\OsintTab\ApiKeysDialog.h \
        src\DnsTab\Dns.h \
        src\SaveTab\Save.h \
        src\core.h \
        src\MainWindow.h

FORMS += \
        src\BruteTab\Brute.ui \
        src\BruteTab\BruteConfigDialog.ui \
        src\BruteTab\WordlistDialog.ui \
        src\BruteTab\WordlistGeneratorDialog.ui \
        src\OsintTab\OsintConfigDialog.ui \
        src\OsintTab\Osint.ui \
        src\OsintTab\ApiKeysDialog.ui \
        src\DnsTab\Dns.ui \
        src\SaveTab\Save.ui \
        src\MainWindow.ui

# including the python headers and libs...
INCLUDEPATH += -I py/include

LIBS += -L ../sub3num/Gui/py/libs -lpython38

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

# setting the icon...
RC_ICONS = res/images/icon.ico

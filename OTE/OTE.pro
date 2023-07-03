QT       += core gui
QT       += network
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OTE
TEMPLATE = app

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# include external libraries

SOURCES += \
    src/Main.cpp \
    src/MainWindow.cpp \
    src/core/Config.cpp \
    src/core/Highlighter.cpp \
    src/dialogs/MultipleTargetsDialog.cpp \
    src/dialogs/preferences/EngineConfig.cpp \
    src/dialogs/preferences/GeneralConfig.cpp \
    src/engine/ExplorerEngine.cpp \
    src/engine/ExtractorEngine.cpp \
    src/core/Database.cpp \
    src/models/ExtractorSitemapModel.cpp \
    src/template/Endpoint.cpp \
    src/template/Extractor.cpp \
    src/template/Template.cpp \
    src/core/Themes.cpp \
    src/core/UpdateChecker.cpp \
    src/core/Utility.cpp \
    src/dialogs/AboutDialog.cpp \
    src/dialogs/ApiKeysDialog.cpp \
    src/dialogs/EndpointDialog.cpp \
    src/dialogs/ExtractorDialog.cpp \
    src/dialogs/LogViewerDialog.cpp \
    src/dialogs/TemplateDialog.cpp \
    src/dialogs/PreferencesDialog.cpp \
    src/views/ExplorerScannerView.cpp \
    src/views/ExplorerSitemapView.cpp \
    src/views/ExplorerTemplatesView.cpp \
    src/views/ExplorerView.cpp \
    src/views/ExtractorScannerView.cpp \
    src/views/ExtractorSitemapView.cpp \
    src/views/ExtractorTemplatesView.cpp \
    src/views/ExtractorView.cpp \
    src/views/WelcomeView.cpp \
    src/widgets/KeyWidget.cpp \
    src/widgets/SideBarWidget.cpp \
    src/widgets/UserKeyWidget.cpp

HEADERS += \
    src/MainWindow.h \
    src/core/Config.h \
    src/core/Highlighter.h \
    src/dialogs/MultipleTargetsDialog.h \
    src/dialogs/preferences/AbstractConfig.h \
    src/dialogs/preferences/EngineConfig.h \
    src/dialogs/preferences/GeneralConfig.h \
    src/engine/ExplorerEngine.h \
    src/engine/ExtractorEngine.h \
    src/core/Database.h \
    src/models/ExtractorSitemapModel.h \
    src/template/Endpoint.h \
    src/template/Extractor.h \
    src/template/Template.h \
    src/core/Themes.h \
    src/core/UpdateChecker.h \
    src/core/Utility.h \
    src/dialogs/AboutDialog.h \
    src/dialogs/ApiKeysDialog.h \
    src/dialogs/EndpointDialog.h \
    src/dialogs/ExtractorDialog.h \
    src/dialogs/LogViewerDialog.h \
    src/dialogs/TemplateDialog.h \
    src/dialogs/PreferencesDialog.h \
    src/views/ExplorerScannerView.h \
    src/views/ExplorerSitemapView.h \
    src/views/ExplorerTemplatesView.h \
    src/views/ExplorerView.h \
    src/views/ExtractorScannerView.h \
    src/views/ExtractorSitemapView.h \
    src/views/ExtractorTemplatesView.h \
    src/views/ExtractorView.h \
    src/views/WelcomeView.h \
    src/widgets/KeyWidget.h \
    src/widgets/SideBarWidget.h \
    src/widgets/UserKeyWidget.h

FORMS += \
    src/MainWindow.ui \
    src/dialogs/AboutDialog.ui \
    src/dialogs/ApiKeysDialog.ui \
    src/dialogs/EndpointDialog.ui \
    src/dialogs/ExtractorDialog.ui \
    src/dialogs/LogViewerDialog.ui \
    src/dialogs/MultipleTargetsDialog.ui \
    src/dialogs/TemplateDialog.ui \
    src/dialogs/PreferencesDialog.ui \
    src/dialogs/preferences/EngineConfig.ui \
    src/dialogs/preferences/GeneralConfig.ui \
    src/views/ExplorerScannerView.ui \
    src/views/ExplorerSitemapView.ui \
    src/views/ExplorerTemplatesView.ui \
    src/views/ExplorerView.ui \
    src/views/ExtractorScannerView.ui \
    src/views/ExtractorSitemapView.ui \
    src/views/ExtractorTemplatesView.ui \
    src/views/ExtractorView.ui \
    src/views/WelcomeView.ui \
    src/widgets/KeyWidget.ui \
    src/widgets/UserKeyWidget.ui
	
RESOURCES += \
        res.qrc \
        res.qrc

unix {
    CONFIG += link_pkgconfig

    # SQLite3
    PKGCONFIG += sqlite3
}

win32 {
    # Sqlite3
    INCLUDEPATH += include/sqlite3/include
    LIBS += $$PWD/../OTE/include/sqlite3/lib/sqlite3.lib
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# setting the icon...
RC_ICONS = res/icons/icon.ico

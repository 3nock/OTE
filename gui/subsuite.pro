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
    src/tools/raw/RawOsint.cpp \
    src/modules/osint/api/Bgpview.cpp \
    src/modules/osint/api/BinaryEdge.cpp \
    src/modules/osint/api/C99.cpp \
    src/modules/osint/api/Circl.cpp \
    src/modules/osint/api/Github.cpp \
    src/modules/osint/api/HunterSearch.cpp \
    src/modules/osint/api/IpApi.cpp \
    src/modules/osint/api/IpInfo.cpp \
    src/modules/osint/api/Mnemonic.cpp \
    src/modules/osint/api/ReconDev.cpp \
    src/modules/osint/api/RiskIq.cpp \
    src/modules/osint/api/Robtex.cpp \
    src/modules/osint/api/SecurityTrails.cpp \
    src/modules/osint/api/Shodan.cpp \
    src/modules/osint/api/ThreatBook.cpp \
    src/modules/osint/api/ViewDns.cpp \
    src/modules/osint/api/WhoisXmlApi.cpp \
    src/modules/osint/api/ZETAlytics.cpp \
    src/modules/osint/api/ZoomEye.cpp \
    src/modules/osint/api/Anubis.cpp \
    src/modules/osint/api/Dnsbufferoverun.cpp \
    src/modules/osint/api/Hackertarget.cpp \
    src/modules/osint/api/Omnisint.cpp \
    src/modules/osint/api/Otx.cpp \
    src/modules/osint/api/Projectdiscovery.cpp \
    src/modules/osint/api/Qwant.cpp \
    src/modules/osint/api/Spyse.cpp \
    src/modules/osint/api/Sublist3r.cpp \
    src/modules/osint/api/Threatcrowd.cpp \
    src/modules/osint/api/Threatminer.cpp \
    src/modules/osint/api/Urlscan.cpp \
    src/modules/osint/api/VirusTotal.cpp \
    src/modules/osint/archive/ArchiveIt.cpp \
    src/modules/osint/archive/ArchiveToday.cpp \
    src/modules/osint/archive/CommonCrawl.cpp \
    src/modules/osint/archive/Waybackmachine.cpp \
    src/modules/osint/cert/Certspotter.cpp \
    src/modules/osint/cert/Crtsh.cpp \
    src/modules/osint/cert/GoogleCert.cpp \
    src/modules/osint/cert/CensysFree.cpp \
    src/modules/osint/scrape/Ask.cpp \
    src/modules/osint/scrape/Baidu.cpp \
    src/modules/osint/scrape/Bing.cpp \
    src/modules/osint/scrape/DogPile.cpp \
    src/modules/osint/scrape/DuckDuckGo.cpp \
    src/modules/osint/scrape/Exalead.cpp \
    src/modules/osint/scrape/Trello.cpp \
    src/modules/osint/scrape/Yahoo.cpp \
    src/modules/osint/site/Dnsdumpster.cpp \
    src/modules/osint/site/Netcraft.cpp \
    src/modules/osint/site/PagesInventory.cpp \
    src/modules/osint/site/Pkey.cpp \
    src/modules/osint/site/Rapiddns.cpp \
    src/modules/osint/site/SiteDossier.cpp \
    src/modules/osint/site/Suip.cpp \
    src/engines/ip/Ip.cpp \
    src/engines/ip/IpScanner.cpp \
    src/engines/active/Active.cpp \
    src/engines/active/ActiveScanner.cpp \
    src/engines/dns/DnsRecords.cpp \
    src/engines/dns/DnsRecordsScanner.cpp \
    src/engines/brute/Brute.cpp \
    src/engines/brute/BruteScanner.cpp \
    src/engines/osint/Osint.cpp \
    src/engines/osint/OsintScanner.cpp \
    src/engines/CommonScanners.cpp \
    src/engines/AbstractEngine.cpp \
    src/project/general/GeneralAnalysis.cpp \
    src/project/specific/SpecificAnalysis.cpp \
    src/project/ProjectDataModel.cpp \
    src/project/Project.cpp \
    src/widgets/InputWidget.cpp \
    src/widgets/OsintModulesWidget.cpp \
    src/widgets/WordlistGeneratorWidget.cpp \
    src/dialogs/AboutDialog.cpp \
    src/dialogs/ConfigDialog.cpp \
    src/dialogs/ApiKeysDialog.cpp \
    src/dialogs/WordlistDialog.cpp \
    src/dialogs/OsintConfigDialog.cpp \
    src/utils/utils.cpp \
    src/utils/Logger.cpp \
    src/SplashScreen.cpp \
    src/MainWindow.cpp \
    src/main.cpp

HEADERS += \
    src/engines/AbstractScanner.h \
    src/tools/raw/RawOsint.h \
    src/modules/osint/api/Bgpview.h \
    src/modules/osint/api/BinaryEdge.h \
    src/modules/osint/api/C99.h \
    src/modules/osint/api/Circl.h \
    src/modules/osint/api/Github.h \
    src/modules/osint/api/HunterSearch.h \
    src/modules/osint/api/IpApi.h \
    src/modules/osint/api/IpInfo.h \
    src/modules/osint/api/Mnemonic.h \
    src/modules/osint/api/ReconDev.h \
    src/modules/osint/api/RiskIq.h \
    src/modules/osint/api/Robtex.h \
    src/modules/osint/api/SecurityTrails.h \
    src/modules/osint/api/Shodan.h \
    src/modules/osint/api/ThreatBook.h \
    src/modules/osint/api/ViewDns.h \
    src/modules/osint/api/WhoisXmlApi.h \
    src/modules/osint/api/ZETAlytics.h \
    src/modules/osint/api/ZoomEye.h \
    src/modules/osint/api/Anubis.h \
    src/modules/osint/api/Dnsbufferoverun.h \
    src/modules/osint/api/Hackertarget.h \
    src/modules/osint/api/Omnisint.h \
    src/modules/osint/api/Otx.h \
    src/modules/osint/api/Projectdiscovery.h \
    src/modules/osint/api/Qwant.h \
    src/modules/osint/api/Spyse.h \
    src/modules/osint/api/Sublist3r.h \
    src/modules/osint/api/Threatcrowd.h \
    src/modules/osint/api/Threatminer.h \
    src/modules/osint/api/Urlscan.h \
    src/modules/osint/api/VirusTotal.h \
    src/modules/osint/archive/ArchiveIt.h \
    src/modules/osint/archive/ArchiveToday.h \
    src/modules/osint/archive/CommonCrawl.h \
    src/modules/osint/archive/Waybackmachine.h \
    src/modules/osint/cert/Certspotter.h \
    src/modules/osint/cert/Crtsh.h \
    src/modules/osint/cert/GoogleCert.h \
    src/modules/osint/cert/CensysFree.h \
    src/modules/osint/scrape/Ask.h \
    src/modules/osint/scrape/Baidu.h \
    src/modules/osint/scrape/Bing.h \
    src/modules/osint/scrape/DogPile.h \
    src/modules/osint/scrape/DuckDuckGo.h \
    src/modules/osint/scrape/Exalead.h \
    src/modules/osint/scrape/Trello.h \
    src/modules/osint/scrape/Yahoo.h \
    src/modules/osint/site/Dnsdumpster.h \
    src/modules/osint/site/Netcraft.h \
    src/modules/osint/site/PagesInventory.h \
    src/modules/osint/site/Pkey.h \
    src/modules/osint/site/Rapiddns.h \
    src/modules/osint/site/SiteDossier.h \
    src/modules/osint/site/Suip.h \
    src/modules/osint/AbstractOsintModule.h \
    src/engines/ip/Ip.h \
    src/engines/ip/IpScanner.h \
    src/engines/active/Active.h \
    src/engines/active/ActiveScanner.h \
    src/engines/dns/DnsRecords.h \
    src/engines/dns/DnsRecordsScanner.h \
    src/engines/brute/Brute.h \
    src/engines/brute/BruteScanner.h \
    src/engines/osint/Osint.h \
    src/engines/osint/OsintScanner.h \
    src/engines/AbstractEngine.h \
    src/engines/CommonScanners.h \
    src/project/general/GeneralAnalysis.h \
    src/project/specific/SpecificAnalysis.h \
    src/project/ProjectDataModel.h \
    src/project/Project.h \
    src/widgets/InputWidget.h \
    src/widgets/OsintModulesWidget.h \
    src/widgets/WordlistGeneratorWidget.h \
    src/dialogs/AboutDialog.h \
    src/dialogs/ApiKeysDialog.h \
    src/dialogs/ConfigDialog.h \
    src/dialogs/WordlistDialog.h \
    src/dialogs/OsintConfigDialog.h \
    src/utils/utils.h \
    src/utils/Config.h \
    src/utils/Logger.h \
    src/SplashScreen.h \
    src/MainWindow.h

FORMS += \
    src/tools/raw/RawOsint.ui \
    src/project/Project.ui \
    src/project/general/GeneralAnalysis.ui \
    src/project/specific/SpecificAnalysis.ui \
    src/engines/ip/Ip.ui \
    src/engines/dns/DnsRecords.ui \
    src/engines/brute/Brute.ui \
    src/engines/osint/Osint.ui \
    src/engines/active/Active.ui \
    src/widgets/InputWidget.ui \
    src/widgets/OsintModulesWidget.ui \
    src/widgets/WordlistGeneratorWidget.ui \
    src/dialogs/AboutDialog.ui \
    src/dialogs/ConfigDialog.ui \
    src/dialogs/WordlistDialog.ui \
    src/dialogs/ApiKeysDialog.ui \
    src/dialogs/OsintConfigDialog.ui \
    src/MainWindow.ui

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

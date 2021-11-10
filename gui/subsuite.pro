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
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables subdomainIp the APIs deprecated before Qt 6.0.0

CONFIG += c++11

include(includes/gumbo/gumbo-parser.pri)

SOURCES += \
    src/engines/ActiveContextMenu.cpp \
    src/engines/BruteContextMenu.cpp \
    src/engines/DnsRecordsContextMenu.cpp \
    src/engines/IpContextMenu.cpp \
    src/engines/OsintContextMenu.cpp \
    src/engines/OsintModules.cpp \
    src/engines/OsintProfiles.cpp \
    src/engines/RawContextMenu.cpp \
    src/engines/RawModules.cpp \
    src/modules/osint/iana/Apnic.cpp \
    src/modules/osint/iana/Arin.cpp \
    src/modules/osint/api/BuiltWith.cpp \
    src/modules/osint/api/Detectify.cpp \
    src/modules/osint/api/Dnslytics.cpp \
    src/modules/osint/api/DomainTools.cpp \
    src/modules/osint/api/HackerTargetFree.cpp \
    src/modules/osint/api/HackerTargetPaid.cpp \
    src/modules/osint/api/Maltiverse.cpp \
    src/modules/osint/api/MnemonicFree.cpp \
    src/modules/osint/api/MnemonicPaid.cpp \
    src/modules/osint/api/N45HT.cpp \
    src/modules/osint/api/Onyphe.cpp \
    src/modules/osint/api/OtxFree.cpp \
    src/modules/osint/api/OtxPaid.cpp \
    src/modules/osint/api/PassiveDns360.cpp \
    src/modules/osint/api/Quake.cpp \
    src/modules/osint/iana/Lacnic.cpp \
    src/modules/osint/iana/Ripe.cpp \
    src/modules/osint/api/RobtexFree.cpp \
    src/modules/osint/api/RobtexPaid.cpp \
    src/modules/osint/api/SpamHaus.cpp \
    src/modules/osint/api/WebResolver.cpp \
    src/modules/osint/iana/Afrinic.cpp \
    src/modules/osint/ip/Ip2Location.cpp \
    src/modules/osint/ip/IpData.cpp \
    src/modules/osint/ip/IpGeoLocation.cpp \
    src/modules/osint/ip/IpRegistry.cpp \
    src/modules/osint/ip/MaxMind.cpp \
    src/modules/osint/mal/AbuseIPDB.cpp \
    src/modules/osint/archive/Arquivo.cpp \
    src/modules/osint/archive/Haw.cpp \
    src/modules/osint/archive/UkWebArchive.cpp \
    src/modules/osint/cert/Digitorus.cpp \
    src/tools/ASNTool.cpp \
    src/tools/BannerGrabber.cpp \
    src/tools/CertTool.cpp \
    src/tools/DomainTool.cpp \
    src/tools/EmailTool.cpp \
    src/tools/IpTool.cpp \
    src/tools/MXTool.cpp \
    src/modules/scan/BruteScanner.cpp \
    src/modules/scan/OsintScanner.cpp \
    src/modules/scan/DnsRecordsScanner.cpp \
    src/modules/scan/ActiveScanner.cpp \
    src/modules/scan/IpScanner.cpp \
    src/modules/check/HostActiveCheck.cpp \
    src/modules/check/InternetCheck.cpp \
    src/modules/check/NameserverCheck.cpp \
    src/modules/check/WildcardCheck.cpp \
    src/modules/osint/api/Bgpview.cpp \
    src/modules/osint/api/BinaryEdge.cpp \
    src/modules/osint/api/C99.cpp \
    src/modules/osint/api/Circl.cpp \
    src/modules/osint/api/Github.cpp \
    src/modules/osint/api/HunterSearch.cpp \
    src/modules/osint/ip/IpApi.cpp \
    src/modules/osint/ip/IpInfo.cpp \
    src/modules/osint/api/RiskIq.cpp \
    src/modules/osint/api/SecurityTrails.cpp \
    src/modules/osint/api/Shodan.cpp \
    src/modules/osint/api/ThreatBook.cpp \
    src/modules/osint/api/ViewDns.cpp \
    src/modules/osint/api/WhoisXmlApi.cpp \
    src/modules/osint/api/ZETAlytics.cpp \
    src/modules/osint/api/ZoomEye.cpp \
    src/modules/osint/api/Anubis.cpp \
    src/modules/osint/api/Dnsbufferoverun.cpp \
    src/modules/osint/api/Omnisint.cpp \
    src/modules/osint/api/Projectdiscovery.cpp \
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
    src/engines/Raw.cpp \
    src/engines/Ip.cpp \
    src/engines/Active.cpp \
    src/engines/DnsRecords.cpp \
    src/engines/Brute.cpp \
    src/engines/Osint.cpp \
    src/engines/AbstractEngine.cpp \
    src/project/general/GeneralAnalysis.cpp \
    src/project/specific/SpecificAnalysis.cpp \
    src/project/ProjectDataModel.cpp \
    src/project/Project.cpp \
    src/tools/NSTool.cpp \
    src/utils/JsonSyntaxHighlighter.cpp \
    src/widgets/InputWidget.cpp \
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
    src/models/AsnModel.h \
    src/models/IpModel.h \
    src/modules/osint/OsintModulesHeaders.h \
    src/modules/osint/iana/Apnic.h \
    src/modules/osint/iana/Arin.h \
    src/modules/osint/api/BuiltWith.h \
    src/modules/osint/api/Detectify.h \
    src/modules/osint/api/Dnslytics.h \
    src/modules/osint/api/DomainTools.h \
    src/modules/osint/api/HackerTargetFree.h \
    src/modules/osint/api/HackerTargetPaid.h \
    src/modules/osint/api/Maltiverse.h \
    src/modules/osint/api/MnemonicFree.h \
    src/modules/osint/api/MnemonicPaid.h \
    src/modules/osint/api/N45HT.h \
    src/modules/osint/api/Onyphe.h \
    src/modules/osint/api/OtxFree.h \
    src/modules/osint/api/OtxPaid.h \
    src/modules/osint/api/PassiveDns360.h \
    src/modules/osint/api/Quake.h \
    src/modules/osint/iana/Lacnic.h \
    src/modules/osint/iana/Ripe.h \
    src/modules/osint/api/RobtexFree.h \
    src/modules/osint/api/RobtexPaid.h \
    src/modules/osint/api/SpamHaus.h \
    src/modules/osint/api/WebResolver.h \
    src/modules/osint/iana/Afrinic.h \
    src/modules/osint/ip/Ip2Location.h \
    src/modules/osint/ip/IpData.h \
    src/modules/osint/ip/IpGeoLocation.h \
    src/modules/osint/ip/IpRegistry.h \
    src/modules/osint/ip/MaxMind.h \
    src/modules/osint/mal/AbuseIPDB.h \
    src/modules/osint/archive/Arquivo.h \
    src/modules/osint/archive/Haw.h \
    src/modules/osint/archive/UkWebArchive.h \
    src/modules/osint/cert/Digitorus.h \
    src/tools/ASNTool.h \
    src/tools/BannerGrabber.h \
    src/tools/CertTool.h \
    src/tools/DomainTool.h \
    src/tools/EmailTool.h \
    src/tools/IpTool.h \
    src/modules/scan/OsintScanner.h \
    src/modules/scan/BruteScanner.h \
    src/modules/scan/DnsRecordsScanner.h \
    src/modules/scan/ActiveScanner.h \
    src/modules/scan/IpScanner.h \
    src/modules/scan/AbstractScanner.h \
    src/modules/check/HostActiveCheck.h \
    src/modules/check/InternetCheck.h \
    src/modules/check/NameserverCheck.h \
    src/modules/check/WildcardCheck.h \
    src/modules/osint/api/Bgpview.h \
    src/modules/osint/api/BinaryEdge.h \
    src/modules/osint/api/C99.h \
    src/modules/osint/api/Circl.h \
    src/modules/osint/api/Github.h \
    src/modules/osint/api/HunterSearch.h \
    src/modules/osint/ip/IpApi.h \
    src/modules/osint/ip/IpInfo.h \
    src/modules/osint/api/RiskIq.h \
    src/modules/osint/api/SecurityTrails.h \
    src/modules/osint/api/Shodan.h \
    src/modules/osint/api/ThreatBook.h \
    src/modules/osint/api/ViewDns.h \
    src/modules/osint/api/WhoisXmlApi.h \
    src/modules/osint/api/ZETAlytics.h \
    src/modules/osint/api/ZoomEye.h \
    src/modules/osint/api/Anubis.h \
    src/modules/osint/api/Dnsbufferoverun.h \
    src/modules/osint/api/Omnisint.h \
    src/modules/osint/api/Projectdiscovery.h \
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
    src/engines/Raw.h \
    src/engines/Ip.h \
    src/engines/Active.h \
    src/engines/DnsRecords.h \
    src/engines/Brute.h \
    src/engines/Osint.h \
    src/engines/AbstractEngine.h \
    src/project/general/GeneralAnalysis.h \
    src/project/specific/SpecificAnalysis.h \
    src/project/ProjectDataModel.h \
    src/project/Project.h \
    src/tools/MXTool.h \
    src/tools/NSTool.h \
    src/utils/JsonSyntaxHighlighter.h \
    src/utils/ResultsModels.h \
    src/utils/models.h \
    src/widgets/InputWidget.h \
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
    src/tools/ASNTool.ui \
    src/tools/BannerGrabber.ui \
    src/tools/CertTool.ui \
    src/tools/DomainTool.ui \
    src/tools/EmailTool.ui \
    src/tools/IpTool.ui \
    src/tools/MXTool.ui \
    src/project/Project.ui \
    src/project/general/GeneralAnalysis.ui \
    src/project/specific/SpecificAnalysis.ui \
    src/engines/Raw.ui \
    src/engines/Ip.ui \
    src/engines/DnsRecords.ui \
    src/engines/Brute.ui \
    src/engines/Osint.ui \
    src/engines/Active.ui \
    src/tools/NSTool.ui \
    src/widgets/InputWidget.ui \
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

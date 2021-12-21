#-------------------------------------------------
#
# Project created by QtCreator 2020-12-07T00:44:51
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sub3suite
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

include(../include/gumbo-parser.pri)

SOURCES += \
    src/dialogs/ActiveConfigDialog.cpp \
    src/dialogs/FailedScansDialog.cpp \
    src/dialogs/PassiveConfigDialog.cpp \
    src/engines/active/ActiveContextMenu.cpp \
    src/engines/active/ActiveScan.cpp \
    src/engines/brute/BruteContextMenu.cpp \
    src/engines/brute/BruteScan.cpp \
    src/engines/ssl/Ssl.cpp \
    src/engines/ssl/SslContextMenu.cpp \
    src/engines/ssl/SslResults.cpp \
    src/engines/ssl/SslScan.cpp \
    src/engines/ip/IpScan.cpp \
    src/engines/osint/OsintResults.cpp \
    src/engines/osint/OsintScan.cpp \
    src/engines/raw/RawResults.cpp \
    src/engines/raw/RawScan.cpp \
    src/engines/dns/Dns.cpp \
    src/engines/ip/IpContextMenu.cpp \
    src/engines/osint/OsintContextMenu.cpp \
    src/engines/osint/OsintModules.cpp \
    src/engines/osint/OsintProfiles.cpp \
    src/engines/raw/RawContextMenu.cpp \
    src/engines/raw/RawModules.cpp \
    src/engines/dns/DnsContextMenu.cpp \
    src/engines/dns/DnsScan.cpp \
    src/modules/osint/api/HaveIBeenPawned.cpp \
    src/modules/osint/api/Host.cpp \
    src/modules/osint/api/JsonWhois.cpp \
    src/modules/osint/api/LeakIX.cpp \
    src/modules/osint/api/NetworksDB.cpp \
    src/modules/osint/api/NeutrinoApi.cpp \
    src/modules/osint/api/Seon.cpp \
    src/modules/osint/api/SpyOnWeb.cpp \
    src/modules/osint/api/Whatcms.cpp \
    src/modules/osint/api/Whoxy.cpp \
    src/modules/osint/archive/UKWebArchive.cpp \
    src/modules/osint/cert/Censys.cpp \
    src/modules/osint/cert/Certspotter.cpp \
    src/modules/osint/cert/CertspotterFree.cpp \
    src/modules/osint/email/EmailCrawlr.cpp \
    src/modules/osint/email/Debounce.cpp \
    src/modules/osint/api/FullHunt.cpp \
    src/modules/osint/email/EmailFormat.cpp \
    src/modules/osint/email/EmailRep.cpp \
    src/modules/osint/email/Hunter.cpp \
    src/modules/osint/email/Snov.cpp \
    src/modules/osint/email/TruMail.cpp \
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
    src/modules/osint/iana/Ripe.cpp \
    src/modules/osint/api/RobtexFree.cpp \
    src/modules/osint/api/RobtexPaid.cpp \
    src/modules/osint/api/SpamHaus.cpp \
    src/modules/osint/api/WebResolver.cpp \
    src/modules/osint/iana/Afrinic.cpp \
    src/modules/osint/ip/IpData.cpp \
    src/modules/osint/ip/IpGeoLocation.cpp \
    src/modules/osint/ip/IpRegistry.cpp \
    src/modules/osint/ip/IpStack.cpp \
    src/modules/osint/mal/AbuseIPDB.cpp \
    src/modules/osint/archive/Arquivo.cpp \
    src/modules/osint/mal/BotScout.cpp \
    src/modules/osint/mal/FraudGuard.cpp \
    src/modules/osint/mal/HybridAnalysis.cpp \
    src/modules/osint/mal/IpQualityScore.cpp \
    src/modules/osint/mal/LeakLookup.cpp \
    src/modules/scan/BannerScanner.cpp \
    src/modules/scan/SSLScanner.cpp \
    src/tools/ASNTool.cpp \
    src/tools/BannerTool.cpp \
    src/tools/CidrTool.cpp \
    src/tools/DomainTool.cpp \
    src/tools/EmailTool.cpp \
    src/tools/IpTool.cpp \
    src/tools/MXTool.cpp \
    src/modules/scan/BruteScanner.cpp \
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
    src/modules/osint/mal/VirusTotal.cpp \
    src/modules/osint/archive/ArchiveIt.cpp \
    src/modules/osint/archive/ArchiveToday.cpp \
    src/modules/osint/archive/CommonCrawl.cpp \
    src/modules/osint/archive/Waybackmachine.cpp \
    src/modules/osint/cert/Crtsh.cpp \
    src/modules/osint/cert/GoogleCert.cpp \
    src/modules/osint/cert/CensysFree.cpp \
    src/modules/osint/scrape/Ask.cpp \
    src/modules/osint/scrape/Baidu.cpp \
    src/modules/osint/scrape/Bing.cpp \
    src/modules/osint/scrape/DogPile.cpp \
    src/modules/osint/scrape/DuckDuckGo.cpp \
    src/modules/osint/scrape/Exalead.cpp \
    src/modules/osint/scrape/Yahoo.cpp \
    src/modules/osint/site/Dnsdumpster.cpp \
    src/modules/osint/site/Netcraft.cpp \
    src/modules/osint/site/PagesInventory.cpp \
    src/modules/osint/site/Pkey.cpp \
    src/modules/osint/site/Rapiddns.cpp \
    src/modules/osint/site/SiteDossier.cpp \
    src/engines/raw/Raw.cpp \
    src/engines/ip/Ip.cpp \
    src/engines/active/Active.cpp \
    src/engines/brute/Brute.cpp \
    src/engines/osint/Osint.cpp \
    src/project/general/GeneralAnalysis.cpp \
    src/project/specific/SpecificAnalysis.cpp \
    src/project/ProjectDataModel.cpp \
    src/project/Project.cpp \
    src/tools/NSTool.cpp \
    src/tools/SSLTool.cpp \
    src/utils/JsonSyntaxHighlighter.cpp \
    src/utils/NotesSyntaxHighlighter.cpp \
    src/utils/config/GeneralConfigDialog.cpp \
    src/utils/logs/LogViewerDialog.cpp \
    src/widgets/InputWidget.cpp \
    src/widgets/Notes.cpp \
    src/dialogs/wordlist/WordlistGeneratorWidget.cpp \
    src/dialogs/AboutDialog.cpp \
    src/dialogs/ApiKeysDialog.cpp \
    src/dialogs/wordlist/WordlistDialog.cpp \
    src/utils/utils.cpp \
    src/utils/Logger.cpp \
    src/SplashScreen.cpp \
    src/MainWindow.cpp \
    src/main.cpp

HEADERS += \
    src/dialogs/ActiveConfigDialog.h \
    src/dialogs/FailedScansDialog.h \
    src/dialogs/PassiveConfigDialog.h \
    src/engines/ssl/Ssl.h \
    src/engines/dns/Dns.h \
    src/models/ASNModel.h \
    src/models/BannerModel.h \
    src/models/IPModel.h \
    src/models/SSLModel.h \
    src/models/CIDRModel.h \
    src/models/DomainModel.h \
    src/models/EmailModel.h \
    src/models/MXModel.h \
    src/models/NSmodel.h \
    src/modules/osint/OsintDefinitions.h \
    src/modules/osint/api/HaveIBeenPawned.h \
    src/modules/osint/api/Host.h \
    src/modules/osint/api/JsonWhois.h \
    src/modules/osint/api/LeakIX.h \
    src/modules/osint/api/NetworksDB.h \
    src/modules/osint/api/NeutrinoApi.h \
    src/modules/osint/api/Seon.h \
    src/modules/osint/api/SpyOnWeb.h \
    src/modules/osint/api/Whatcms.h \
    src/modules/osint/api/Whoxy.h \
    src/modules/osint/archive/UKWebArchive.h \
    src/modules/osint/cert/Censys.h \
    src/modules/osint/cert/Certspotter.h \
    src/modules/osint/cert/CertspotterFree.h \
    src/modules/osint/email/EmailCrawlr.h \
    src/modules/osint/OsintModulesHeaders.h \
    src/modules/osint/email/Debounce.h \
    src/modules/osint/api/FullHunt.h \
    src/modules/osint/email/EmailFormat.h \
    src/modules/osint/email/EmailRep.h \
    src/modules/osint/email/Hunter.h \
    src/modules/osint/email/Snov.h \
    src/modules/osint/email/TruMail.h \
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
    src/modules/osint/api/Quake.h \
    src/modules/osint/iana/Ripe.h \
    src/modules/osint/api/RobtexFree.h \
    src/modules/osint/api/RobtexPaid.h \
    src/modules/osint/api/SpamHaus.h \
    src/modules/osint/api/WebResolver.h \
    src/modules/osint/iana/Afrinic.h \
    src/modules/osint/ip/IpData.h \
    src/modules/osint/ip/IpGeoLocation.h \
    src/modules/osint/ip/IpRegistry.h \
    src/modules/osint/ip/IpStack.h \
    src/modules/osint/mal/AbuseIPDB.h \
    src/modules/osint/archive/Arquivo.h \
    src/modules/osint/mal/BotScout.h \
    src/modules/osint/mal/FraudGuard.h \
    src/modules/osint/mal/HybridAnalysis.h \
    src/modules/osint/mal/IpQualityScore.h \
    src/modules/osint/mal/LeakLookup.h \
    src/modules/scan/BannerScanner.h \
    src/modules/scan/SSLScanner.h \
    src/tools/ASNTool.h \
    src/tools/BannerTool.h \
    src/tools/CidrTool.h \
    src/tools/DomainTool.h \
    src/tools/EmailTool.h \
    src/tools/IpTool.h \
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
    src/modules/osint/mal/VirusTotal.h \
    src/modules/osint/archive/ArchiveIt.h \
    src/modules/osint/archive/ArchiveToday.h \
    src/modules/osint/archive/CommonCrawl.h \
    src/modules/osint/archive/Waybackmachine.h \
    src/modules/osint/cert/Crtsh.h \
    src/modules/osint/cert/GoogleCert.h \
    src/modules/osint/cert/CensysFree.h \
    src/modules/osint/scrape/Ask.h \
    src/modules/osint/scrape/Baidu.h \
    src/modules/osint/scrape/Bing.h \
    src/modules/osint/scrape/DogPile.h \
    src/modules/osint/scrape/DuckDuckGo.h \
    src/modules/osint/scrape/Exalead.h \
    src/modules/osint/scrape/Yahoo.h \
    src/modules/osint/site/Dnsdumpster.h \
    src/modules/osint/site/Netcraft.h \
    src/modules/osint/site/PagesInventory.h \
    src/modules/osint/site/Pkey.h \
    src/modules/osint/site/Rapiddns.h \
    src/modules/osint/site/SiteDossier.h \
    src/modules/osint/AbstractOsintModule.h \
    src/engines/raw/Raw.h \
    src/engines/ip/Ip.h \
    src/engines/active/Active.h \
    src/engines/brute/Brute.h \
    src/engines/osint/Osint.h \
    src/engines/AbstractEngine.h \
    src/project/general/GeneralAnalysis.h \
    src/project/specific/SpecificAnalysis.h \
    src/project/ProjectDataModel.h \
    src/project/Project.h \
    src/tools/MXTool.h \
    src/tools/NSTool.h \
    src/tools/SSLTool.h \
    src/utils/Definitions.h \
    src/utils/JsonSyntaxHighlighter.h \
    src/utils/NotesSyntaxHighlighter.h \
    src/utils/config/GeneralConfigDialog.h \
    src/utils/logs/LogViewerDialog.h \
    src/utils/models.h \
    src/widgets/InputWidget.h \
    src/widgets/Notes.h \
    src/dialogs/wordlist/WordlistGeneratorWidget.h \
    src/dialogs/AboutDialog.h \
    src/dialogs/ApiKeysDialog.h \
    src/dialogs/wordlist/WordlistDialog.h \
    src/utils/utils.h \
    src/utils/Config.h \
    src/utils/Logger.h \
    src/SplashScreen.h \
    src/MainWindow.h

FORMS += \
    src/dialogs/ActiveConfigDialog.ui \
    src/dialogs/FailedScansDialog.ui \
    src/dialogs/PassiveConfigDialog.ui \
    src/engines/ssl/Ssl.ui \
    src/tools/ASNTool.ui \
    src/tools/BannerTool.ui \
    src/tools/CidrTool.ui \
    src/tools/DomainTool.ui \
    src/tools/EmailTool.ui \
    src/tools/IpTool.ui \
    src/tools/MXTool.ui \
    src/project/Project.ui \
    src/project/general/GeneralAnalysis.ui \
    src/project/specific/SpecificAnalysis.ui \
    src/engines/raw/Raw.ui \
    src/engines/ip/Ip.ui \
    src/engines/dns/Dns.ui \
    src/engines/brute/Brute.ui \
    src/engines/osint/Osint.ui \
    src/engines/active/Active.ui \
    src/tools/NSTool.ui \
    src/tools/SSLTool.ui \
    src/utils/config/GeneralConfigDialog.ui \
    src/utils/logs/LogViewerDialog.ui \
    src/widgets/InputWidget.ui \
    src/widgets/Notes.ui \
    src/dialogs/wordlist/WordlistGeneratorWidget.ui \
    src/dialogs/AboutDialog.ui \
    src/dialogs/wordlist/WordlistDialog.ui \
    src/dialogs/ApiKeysDialog.ui \
    src/MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

# setting the icon...
RC_ICONS = res/icons/logo.ico

DISTFILES += \
    res/files/Ideas

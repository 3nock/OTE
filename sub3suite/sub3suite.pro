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
    src/dialogs/DocumentationDialog.cpp \
    src/dialogs/ActiveConfigDialog.cpp \
    src/dialogs/FailedScansDialog.cpp \
    src/dialogs/PassiveConfigDialog.cpp \
    src/dialogs/StartupDialog.cpp \
    src/dialogs/ToolsConfigDialog.cpp \
    src/dialogs/preference/PreferenceDialog.cpp \
    src/dialogs/wordlist/ChooseWordlist.cpp \
    src/dialogs/wordlist/GenerateWordlist.cpp \
    src/engines/active/ActiveContextMenu.cpp \
    src/engines/active/ActiveResults.cpp \
    src/engines/active/ActiveScan.cpp \
    src/engines/brute/BruteContextMenu.cpp \
    src/engines/brute/BruteResults.cpp \
    src/engines/brute/BruteScan.cpp \
    src/engines/dns/DnsResults.cpp \
    src/engines/ssl/Ssl.cpp \
    src/engines/ssl/SslContextMenu.cpp \
    src/engines/ssl/SslResults.cpp \
    src/engines/ssl/SslScan.cpp \
    src/engines/osint/OsintResults.cpp \
    src/engines/osint/OsintScan.cpp \
    src/engines/raw/RawResults.cpp \
    src/engines/raw/RawScan.cpp \
    src/engines/dns/Dns.cpp \
    src/engines/osint/OsintContextMenu.cpp \
    src/engines/osint/OsintModules.cpp \
    src/engines/raw/RawContextMenu.cpp \
    src/engines/raw/RawModules.cpp \
    src/engines/dns/DnsContextMenu.cpp \
    src/engines/dns/DnsScan.cpp \
    src/models/ProjectModel.cpp \
    src/modules/passive/api/HaveIBeenPawned.cpp \
    src/modules/passive/api/Host.cpp \
    src/modules/passive/api/JsonWhois.cpp \
    src/modules/passive/api/LeakIX.cpp \
    src/modules/passive/api/NetworksDB.cpp \
    src/modules/passive/api/NeutrinoApi.cpp \
    src/modules/passive/api/Seon.cpp \
    src/modules/passive/api/SpyOnWeb.cpp \
    src/modules/passive/api/Wappalyzer.cpp \
    src/modules/passive/api/Whatcms.cpp \
    src/modules/passive/api/Whoxy.cpp \
    src/modules/passive/archive/UKWebArchive.cpp \
    src/modules/passive/cert/Censys.cpp \
    src/modules/passive/cert/Certspotter.cpp \
    src/modules/passive/cert/CertspotterFree.cpp \
    src/modules/passive/email/EmailCrawlr.cpp \
    src/modules/passive/email/Debounce.cpp \
    src/modules/passive/api/FullHunt.cpp \
    src/modules/passive/email/EmailFormat.cpp \
    src/modules/passive/email/EmailRep.cpp \
    src/modules/passive/email/Hunter.cpp \
    src/modules/passive/email/Snov.cpp \
    src/modules/passive/email/TruMail.cpp \
    src/modules/passive/iana/Apnic.cpp \
    src/modules/passive/iana/Arin.cpp \
    src/modules/passive/api/BuiltWith.cpp \
    src/modules/passive/api/Dnslytics.cpp \
    src/modules/passive/api/DomainTools.cpp \
    src/modules/passive/api/HackerTargetFree.cpp \
    src/modules/passive/api/HackerTargetPaid.cpp \
    src/modules/passive/api/Maltiverse.cpp \
    src/modules/passive/api/MnemonicFree.cpp \
    src/modules/passive/api/MnemonicPaid.cpp \
    src/modules/passive/api/N45HT.cpp \
    src/modules/passive/api/Onyphe.cpp \
    src/modules/passive/api/OtxFree.cpp \
    src/modules/passive/api/OtxPaid.cpp \
    src/modules/passive/iana/Ripe.cpp \
    src/modules/passive/api/RobtexFree.cpp \
    src/modules/passive/api/RobtexPaid.cpp \
    src/modules/passive/api/SpamHaus.cpp \
    src/modules/passive/api/WebResolver.cpp \
    src/modules/passive/iana/Afrinic.cpp \
    src/modules/passive/ip/IpData.cpp \
    src/modules/passive/ip/IpGeoLocation.cpp \
    src/modules/passive/ip/IpRegistry.cpp \
    src/modules/passive/ip/IpStack.cpp \
    src/modules/passive/mal/AbuseIPDB.cpp \
    src/modules/passive/archive/Arquivo.cpp \
    src/modules/passive/mal/BotScout.cpp \
    src/modules/passive/mal/FraudGuard.cpp \
    src/modules/passive/mal/HybridAnalysis.cpp \
    src/modules/passive/mal/IpQualityScore.cpp \
    src/modules/passive/mal/LeakLookup.cpp \
    src/modules/active/DNSScanner.cpp \
    src/modules/active/SSLScanner.cpp \
    src/modules/active/WildcardScanner.cpp \
    src/project/Project.cpp \
    src/enums/url/UrlEnum.cpp \
    src/enums/asn/ASNEnum.cpp \
    src/enums/cidr/CidrEnum.cpp \
    src/enums/email/EmailEnum.cpp \
    src/enums/ip/IpEnum.cpp \
    src/enums/mx/MXEnum.cpp \
    src/enums/ns/NSEnum.cpp \
    src/enums/ssl/SSLEnum.cpp \
    src/modules/active/BruteScanner.cpp \
    src/modules/active/ActiveScanner.cpp \
    src/modules/active/IpScanner.cpp \
    src/modules/passive/api/Bgpview.cpp \
    src/modules/passive/api/BinaryEdge.cpp \
    src/modules/passive/api/C99.cpp \
    src/modules/passive/api/Circl.cpp \
    src/modules/passive/api/Github.cpp \
    src/modules/passive/ip/IpApi.cpp \
    src/modules/passive/ip/IpInfo.cpp \
    src/modules/passive/api/RiskIq.cpp \
    src/modules/passive/api/SecurityTrails.cpp \
    src/modules/passive/api/Shodan.cpp \
    src/modules/passive/api/ThreatBook.cpp \
    src/modules/passive/api/ViewDns.cpp \
    src/modules/passive/api/WhoisXmlApi.cpp \
    src/modules/passive/api/ZETAlytics.cpp \
    src/modules/passive/api/ZoomEye.cpp \
    src/modules/passive/api/Anubis.cpp \
    src/modules/passive/api/Dnsbufferoverun.cpp \
    src/modules/passive/api/Omnisint.cpp \
    src/modules/passive/api/Projectdiscovery.cpp \
    src/modules/passive/api/Spyse.cpp \
    src/modules/passive/api/Sublist3r.cpp \
    src/modules/passive/api/Threatcrowd.cpp \
    src/modules/passive/api/Threatminer.cpp \
    src/modules/passive/api/Urlscan.cpp \
    src/modules/passive/mal/VirusTotal.cpp \
    src/modules/passive/archive/ArchiveIt.cpp \
    src/modules/passive/archive/ArchiveToday.cpp \
    src/modules/passive/archive/CommonCrawl.cpp \
    src/modules/passive/archive/Waybackmachine.cpp \
    src/modules/passive/cert/Crtsh.cpp \
    src/modules/passive/cert/GoogleCert.cpp \
    src/modules/passive/cert/CensysFree.cpp \
    src/modules/passive/scrape/Ask.cpp \
    src/modules/passive/scrape/Baidu.cpp \
    src/modules/passive/scrape/Bing.cpp \
    src/modules/passive/scrape/DogPile.cpp \
    src/modules/passive/scrape/DuckDuckGo.cpp \
    src/modules/passive/scrape/Exalead.cpp \
    src/modules/passive/scrape/Yahoo.cpp \
    src/modules/passive/site/Dnsdumpster.cpp \
    src/modules/passive/site/Netcraft.cpp \
    src/modules/passive/site/PagesInventory.cpp \
    src/modules/passive/site/Pkey.cpp \
    src/modules/passive/site/Rapiddns.cpp \
    src/modules/passive/site/SiteDossier.cpp \
    src/engines/raw/Raw.cpp \
    src/engines/active/Active.cpp \
    src/engines/brute/Brute.cpp \
    src/engines/osint/Osint.cpp \
    src/utils/JsonSyntaxHighlighter.cpp \
    src/utils/LogsSyntaxHighlighter.cpp \
    src/utils/NotesSyntaxHighlighter.cpp \
    src/widgets/InputWidget.cpp \
    src/dialogs/LogViewerDialog.cpp \
    src/dialogs/AboutDialog.cpp \
    src/dialogs/ApiKeysDialog.cpp \
    src/dialogs/wordlist/WordlistDialog.cpp \
    src/MainWindow.cpp \
    src/main.cpp

HEADERS += \
    src/dialogs/DocumentationDialog.h \
    src/dialogs/ActiveConfigDialog.h \
    src/dialogs/FailedScansDialog.h \
    src/dialogs/PassiveConfigDialog.h \
    src/dialogs/StartupDialog.h \
    src/dialogs/ToolsConfigDialog.h \
    src/dialogs/preference/PreferenceDialog.h \
    src/engines/ssl/Ssl.h \
    src/engines/dns/Dns.h \
    src/models/ASNModel.h \
    src/models/IPModel.h \
    src/models/NSModel.h \
    src/models/ProjectModel.h \
    src/models/SSLModel.h \
    src/models/CIDRModel.h \
    src/models/DomainModel.h \
    src/models/EmailModel.h \
    src/models/MXModel.h \
    src/modules/passive/OsintDefinitions.h \
    src/modules/passive/api/HaveIBeenPawned.h \
    src/modules/passive/api/Host.h \
    src/modules/passive/api/JsonWhois.h \
    src/modules/passive/api/LeakIX.h \
    src/modules/passive/api/NetworksDB.h \
    src/modules/passive/api/NeutrinoApi.h \
    src/modules/passive/api/Seon.h \
    src/modules/passive/api/SpyOnWeb.h \
    src/modules/passive/api/Wappalyzer.h \
    src/modules/passive/api/Whatcms.h \
    src/modules/passive/api/Whoxy.h \
    src/modules/passive/archive/UKWebArchive.h \
    src/modules/passive/cert/Censys.h \
    src/modules/passive/cert/Certspotter.h \
    src/modules/passive/cert/CertspotterFree.h \
    src/modules/passive/email/EmailCrawlr.h \
    src/modules/passive/OsintModulesHeaders.h \
    src/modules/passive/email/Debounce.h \
    src/modules/passive/api/FullHunt.h \
    src/modules/passive/email/EmailFormat.h \
    src/modules/passive/email/EmailRep.h \
    src/modules/passive/email/Hunter.h \
    src/modules/passive/email/Snov.h \
    src/modules/passive/email/TruMail.h \
    src/modules/passive/iana/Apnic.h \
    src/modules/passive/iana/Arin.h \
    src/modules/passive/api/BuiltWith.h \
    src/modules/passive/api/Dnslytics.h \
    src/modules/passive/api/DomainTools.h \
    src/modules/passive/api/HackerTargetFree.h \
    src/modules/passive/api/HackerTargetPaid.h \
    src/modules/passive/api/Maltiverse.h \
    src/modules/passive/api/MnemonicFree.h \
    src/modules/passive/api/MnemonicPaid.h \
    src/modules/passive/api/N45HT.h \
    src/modules/passive/api/Onyphe.h \
    src/modules/passive/api/OtxFree.h \
    src/modules/passive/api/OtxPaid.h \
    src/modules/passive/api/Quake.h \
    src/modules/passive/iana/Ripe.h \
    src/modules/passive/api/RobtexFree.h \
    src/modules/passive/api/RobtexPaid.h \
    src/modules/passive/api/SpamHaus.h \
    src/modules/passive/api/WebResolver.h \
    src/modules/passive/iana/Afrinic.h \
    src/modules/passive/ip/IpData.h \
    src/modules/passive/ip/IpGeoLocation.h \
    src/modules/passive/ip/IpRegistry.h \
    src/modules/passive/ip/IpStack.h \
    src/modules/passive/mal/AbuseIPDB.h \
    src/modules/passive/archive/Arquivo.h \
    src/modules/passive/mal/BotScout.h \
    src/modules/passive/mal/FraudGuard.h \
    src/modules/passive/mal/HybridAnalysis.h \
    src/modules/passive/mal/IpQualityScore.h \
    src/modules/passive/mal/LeakLookup.h \
    src/modules/active/DNSScanner.h \
    src/modules/active/SSLScanner.h \
    src/modules/active/WildcardScanner.h \
    src/project/Project.h \
    src/enums/AbstractEnum.h \
    src/enums/url/UrlEnum.h \
    src/enums/asn/ASNEnum.h \
    src/enums/cidr/CidrEnum.h \
    src/enums/email/EmailEnum.h \
    src/enums/ip/IpEnum.h \
    src/enums/mx/MXEnum.h \
    src/enums/ns/NSEnum.h \
    src/enums/ssl/SSLEnum.h \
    src/modules/active/BruteScanner.h \
    src/modules/active/ActiveScanner.h \
    src/modules/active/IpScanner.h \
    src/modules/active/AbstractScanner.h \
    src/modules/passive/api/Bgpview.h \
    src/modules/passive/api/BinaryEdge.h \
    src/modules/passive/api/C99.h \
    src/modules/passive/api/Circl.h \
    src/modules/passive/api/Github.h \
    src/modules/passive/ip/IpApi.h \
    src/modules/passive/ip/IpInfo.h \
    src/modules/passive/api/RiskIq.h \
    src/modules/passive/api/SecurityTrails.h \
    src/modules/passive/api/Shodan.h \
    src/modules/passive/api/ThreatBook.h \
    src/modules/passive/api/ViewDns.h \
    src/modules/passive/api/WhoisXmlApi.h \
    src/modules/passive/api/ZETAlytics.h \
    src/modules/passive/api/ZoomEye.h \
    src/modules/passive/api/Anubis.h \
    src/modules/passive/api/Dnsbufferoverun.h \
    src/modules/passive/api/Omnisint.h \
    src/modules/passive/api/Projectdiscovery.h \
    src/modules/passive/api/Spyse.h \
    src/modules/passive/api/Sublist3r.h \
    src/modules/passive/api/Threatcrowd.h \
    src/modules/passive/api/Threatminer.h \
    src/modules/passive/api/Urlscan.h \
    src/modules/passive/mal/VirusTotal.h \
    src/modules/passive/archive/ArchiveIt.h \
    src/modules/passive/archive/ArchiveToday.h \
    src/modules/passive/archive/CommonCrawl.h \
    src/modules/passive/archive/Waybackmachine.h \
    src/modules/passive/cert/Crtsh.h \
    src/modules/passive/cert/GoogleCert.h \
    src/modules/passive/cert/CensysFree.h \
    src/modules/passive/scrape/Ask.h \
    src/modules/passive/scrape/Baidu.h \
    src/modules/passive/scrape/Bing.h \
    src/modules/passive/scrape/DogPile.h \
    src/modules/passive/scrape/DuckDuckGo.h \
    src/modules/passive/scrape/Exalead.h \
    src/modules/passive/scrape/Yahoo.h \
    src/modules/passive/site/Dnsdumpster.h \
    src/modules/passive/site/Netcraft.h \
    src/modules/passive/site/PagesInventory.h \
    src/modules/passive/site/Pkey.h \
    src/modules/passive/site/Rapiddns.h \
    src/modules/passive/site/SiteDossier.h \
    src/modules/passive/AbstractOsintModule.h \
    src/engines/raw/Raw.h \
    src/engines/active/Active.h \
    src/engines/brute/Brute.h \
    src/engines/osint/Osint.h \
    src/engines/AbstractEngine.h \
    src/s3s.h \
    src/utils/Definitions.h \
    src/utils/JsonSyntaxHighlighter.h \
    src/utils/LogsSyntaxHighlighter.h \
    src/utils/NotesSyntaxHighlighter.h \
    src/utils/models.h \
    src/widgets/InputWidget.h \
    src/dialogs/LogViewerDialog.h \
    src/dialogs/AboutDialog.h \
    src/dialogs/ApiKeysDialog.h \
    src/dialogs/wordlist/WordlistDialog.h \
    src/utils/utils.h \
    src/utils/Config.h \
    src/MainWindow.h

FORMS += \
    src/dialogs/DocumentationDialog.ui \
    src/dialogs/ActiveConfigDialog.ui \
    src/dialogs/FailedScansDialog.ui \
    src/dialogs/PassiveConfigDialog.ui \
    src/dialogs/StartupDialog.ui \
    src/dialogs/ToolsConfigDialog.ui \
    src/dialogs/preference/PreferenceDialog.ui \
    src/engines/ssl/Ssl.ui \
    src/project/Project.ui \
    src/enums/url/UrlEnum.ui \
    src/enums/asn/ASNEnum.ui \
    src/enums/cidr/CidrEnum.ui \
    src/enums/email/EmailEnum.ui \
    src/enums/ip/IpEnum.ui \
    src/enums/mx/MXEnum.ui \
    src/enums/ns/NSEnum.ui \
    src/enums/ssl/SSLEnum.ui \
    src/engines/raw/Raw.ui \
    src/engines/dns/Dns.ui \
    src/engines/brute/Brute.ui \
    src/engines/osint/Osint.ui \
    src/engines/active/Active.ui \
    src/widgets/InputWidget.ui \
    src/dialogs/LogViewerDialog.ui \
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
RC_ICONS = res/icons/s3s_logo.ico

DISTFILES +=

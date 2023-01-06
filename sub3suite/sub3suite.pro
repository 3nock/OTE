#-------------------------------------------------------------------------------------------
#
#                Project created by QtCreator 2020-11-07T00:44:51
#
# Copyright 2020-2022 Enock Nicholaus Michael <3nock@protonmail.com>. All rights reserved.
#
#-------------------------------------------------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sub3suite
TEMPLATE = app

# show deprecated functions warnings
DEFINES += QT_DEPRECATED_WARNINGS

# compiler versions
CONFIG += c99
CONFIG += c++11

win32-msvc* {
    # debug information; PDB and MAP files
    QMAKE_LFLAGS_RELEASE += /MAP
    QMAKE_CFLAGS_RELEASE += /Zi
    QMAKE_LFLAGS_RELEASE += /INCREMENTAL:NO /debug /opt:ref
}

unix {
    # debug information; .debug file
    CONFIG += separate_debug_info
    QMAKE_CXXFLAGS += -g

    # linked shared libraries path
    QMAKE_LFLAGS_RPATH =
    QMAKE_LFLAGS += "-Wl,-rpath,\'\$$ORIGIN/lib\'"
}

# included external libraries
include(include/gumbo-parser.pri)

SOURCES += \
    src/dialogs/DocumentationDialog.cpp \
    src/dialogs/FailedScansDialog.cpp \
    src/dialogs/SaveProjectDialog.cpp \
    src/dialogs/StartupDialog.cpp \
    src/dialogs/WordlistDialog_choose.cpp \
    src/dialogs/WordlistDialog_generate.cpp \
    src/dialogs/config/BruteConfigDialog.cpp \
    src/dialogs/config/DNSConfigDialog.cpp \
    src/dialogs/config/EnumConfigDialog.cpp \
    src/dialogs/config/HostConfigDialog.cpp \
    src/dialogs/config/IPConfigDialog.cpp \
    src/dialogs/config/OsintConfigDialog.cpp \
    src/dialogs/config/ProjectConfigDialog.cpp \
    src/dialogs/config/RawConfigDialog.cpp \
    src/dialogs/config/SSLConfigDialog.cpp \
    src/dialogs/config/URLConfigDialog.cpp \
    src/items/IPToolItem.cpp \
    src/modules/active/ReverseIPScanner.cpp \
    src/tools/ip/IPTool_actions.cpp \
    src/tools/ip/IPTool_contextmenu.cpp \
    src/tools/ip/IPTool_results.cpp \
    src/tools/ip/IPTool_scan.cpp \
    src/tools/url/URLTool_actions.cpp \
    src/tools/url/URLTool_contextmenu.cpp \
    src/tools/url/URLTool_results.cpp \
    src/tools/url/URLTool_scan.cpp \
    src/tools/host/HostTool_actions.cpp \
    src/tools/host/HostTool_contextmenu.cpp \
    src/tools/host/HostTool_results.cpp \
    src/tools/host/HostTool_scan.cpp \
    src/tools/brute/BruteTool_actions.cpp \
    src/tools/brute/BruteTool_contextmenu.cpp \
    src/tools/brute/BruteTool_results.cpp \
    src/tools/brute/BruteTool_scan.cpp \
    src/tools/dns/DNSTool_actions.cpp \
    src/tools/dns/DNSTool_contextmenu.cpp \
    src/tools/dns/DNSTool_results.cpp \
    src/tools/dns/DNSTool_scan.cpp \
    src/tools/osint/OsintTool_actions.cpp \
    src/tools/osint/OsintTool_contextmenu.cpp \
    src/tools/osint/OsintTool_modules.cpp \
    src/tools/osint/OsintTool_results.cpp \
    src/tools/osint/OsintTool_scan.cpp \
    src/tools/raw/RawTool_actions.cpp \
    src/tools/raw/RawTool_contextmenu.cpp \
    src/tools/raw/RawTool_modules.cpp \
    src/tools/raw/RawTool_results.cpp \
    src/tools/raw/RawTool_scan.cpp \
    src/tools/ssl/SSLTool_actions.cpp \
    src/tools/ssl/SSLTool_contextmenu.cpp \
    src/tools/ssl/SSLTool_results.cpp \
    src/tools/ssl/SSLTool_scan.cpp \
    src/tools/ip/IPTool.cpp \
    src/tools/url/URLTool.cpp \
    src/tools/ssl/SSLTool.cpp \
    src/tools/dns/DNSTool.cpp \
    src/tools/raw/RawTool.cpp \
    src/tools/host/HostTool.cpp \
    src/tools/brute/BruteTool.cpp \
    src/tools/osint/OsintTool.cpp \
    src/enums/asn/ASNEnum_actions.cpp \
    src/enums/asn/ASNEnum_contextmenu.cpp \
    src/enums/asn/ASNEnum_results.cpp \
    src/enums/asn/ASNEnum_scan.cpp \
    src/enums/cidr/CIDREnum.cpp \
    src/enums/cidr/CIDREnum_actions.cpp \
    src/enums/cidr/CIDREnum_contextmenu.cpp \
    src/enums/cidr/CIDREnum_results.cpp \
    src/enums/cidr/CIDREnum_scan.cpp \
    src/enums/email/EmailEnum_actions.cpp \
    src/enums/email/EmailEnum_contextmenu.cpp \
    src/enums/email/EmailEnum_results.cpp \
    src/enums/email/EmailEnum_scan.cpp \
    src/enums/ip/IPEnum_actions.cpp \
    src/enums/ip/IPEnum_contextmenu.cpp \
    src/enums/ip/IPEnum_results.cpp \
    src/enums/ip/IPEnum_scan.cpp \
    src/enums/mx/MXEnum_actions.cpp \
    src/enums/mx/MXEnum_contextmenu.cpp \
    src/enums/mx/MXEnum_results.cpp \
    src/enums/mx/MXEnum_scan.cpp \
    src/enums/ns/NSEnum_actions.cpp \
    src/enums/ns/NSEnum_contextmenu.cpp \
    src/enums/ns/NSEnum_results.cpp \
    src/enums/ns/NSEnum_scan.cpp \
    src/enums/ssl/SSLEnum_actions.cpp \
    src/enums/ssl/SSLEnum_contextmenu.cpp \
    src/enums/ssl/SSLEnum_results.cpp \
    src/enums/ssl/SSLEnum_scan.cpp \
    src/enums/asn/ASNEnum.cpp \
    src/enums/email/EmailEnum.cpp \
    src/enums/ip/IPEnum.cpp \
    src/enums/mx/MXEnum.cpp \
    src/enums/ns/NSEnum.cpp \
    src/enums/ssl/SSLEnum.cpp \
    src/items/ASNItem.cpp \
    src/items/CIDRItem.cpp \
    src/items/DNSItem.cpp \
    src/items/EmailItem.cpp \
    src/items/HostItem.cpp \
    src/items/IPItem.cpp \
    src/items/MXItem.cpp \
    src/items/NSItem.cpp \
    src/models/ExplorerModel.cpp \
    src/modules/active/PingScanner.cpp \
    src/modules/active/PingScanner_unix.cpp \
    src/modules/active/PingScanner_win.cpp \
    src/modules/active/PortScanner_unix.cpp \
    src/modules/active/PortScanner_win.cpp \
    src/modules/active/URLScanner.cpp \
    src/modules/passive/api/ASRank.cpp \
    src/modules/passive/api/CirclPublic.cpp \
    src/modules/passive/api/HackerTarget.cpp \
    src/modules/passive/api/Mnemonic.cpp \
    src/modules/passive/api/Otx.cpp \
    src/modules/passive/api/PassiveTotal.cpp \
    src/modules/passive/api/Robtex.cpp \
    src/modules/passive/ip/Ipfy.cpp \
    src/modules/passive/scrape/BingSearch.cpp \
    src/modules/passive/scrape/GoogleSearch.cpp \
    src/modules/passive/scrape/YahooSearch.cpp \
    src/project/Project_contextmenu.cpp \
    src/models/ProjectModel.cpp \
    src/models/ProjectModel_serialization.cpp \
    src/models/ProjectModel_slots.cpp \
    src/items/RawItem.cpp \
    src/items/SSLItem.cpp \
    src/items/URLItem.cpp \
    src/items/WildcardItem.cpp \
    src/modules/active/PortScanner.cpp \
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
    src/modules/passive/api/Maltiverse.cpp \
    src/modules/passive/api/MnemonicFree.cpp \
    src/modules/passive/api/N45HT.cpp \
    src/modules/passive/api/Onyphe.cpp \
    src/modules/passive/api/OtxFree.cpp \
    src/modules/passive/iana/Ripe.cpp \
    src/modules/passive/api/RobtexFree.cpp \
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
    src/project/Project.cpp \
    src/modules/active/BruteScanner.cpp \
    src/modules/active/HostScanner.cpp \
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
    src/modules/passive/scrape/DogPile.cpp \
    src/modules/passive/scrape/DuckDuckGo.cpp \
    src/modules/passive/scrape/Exalead.cpp \
    src/modules/passive/site/Dnsdumpster.cpp \
    src/modules/passive/site/Netcraft.cpp \
    src/modules/passive/site/PagesInventory.cpp \
    src/modules/passive/site/Pkey.cpp \
    src/modules/passive/site/Rapiddns.cpp \
    src/modules/passive/site/SiteDossier.cpp \
    src/project/Project_actions.cpp \
    src/project/Project_slots.cpp \
    src/utils/UpdateChecker.cpp \
    src/widgets/InputWidget.cpp \
    src/utils/SyntaxHighlighter.cpp \
    src/dialogs/LogViewerDialog.cpp \
    src/dialogs/AboutDialog.cpp \
    src/dialogs/ApiKeysDialog.cpp \
    src/dialogs/WordlistDialog.cpp \
    src/utils/Config.cpp \
    src/MainWindow.cpp \
    src/main.cpp

HEADERS += \
    src/dialogs/DocumentationDialog.h \
    src/dialogs/FailedScansDialog.h \
    src/dialogs/SaveProjectDialog.h \
    src/dialogs/StartupDialog.h \
    src/dialogs/config/BruteConfigDialog.h \
    src/dialogs/config/DNSConfigDialog.h \
    src/dialogs/config/EnumConfigDialog.h \
    src/dialogs/config/HostConfigDialog.h \
    src/dialogs/config/IPConfigDialog.h \
    src/dialogs/config/OsintConfigDialog.h \
    src/dialogs/config/ProjectConfigDialog.h \
    src/dialogs/config/RawConfigDialog.h \
    src/dialogs/config/SSLConfigDialog.h \
    src/dialogs/config/URLConfigDialog.h \
    src/items/IPToolItem.h \
    src/modules/active/ReverseIPScanner.h \
    src/tools/ip/IPTool.h \
    src/tools/url/URLTool.h \
    src/tools/ssl/SSLTool.h \
    src/tools/dns/DNSTool.h \
    src/tools/raw/RawTool.h \
    src/tools/host/HostTool.h \
    src/tools/brute/BruteTool.h \
    src/tools/osint/OsintTool.h \
    src/tools/AbstractTool.h \
    src/items/ASNItem.h \
    src/items/CIDRItem.h \
    src/items/DNSItem.h \
    src/items/EmailItem.h \
    src/items/HostItem.h \
    src/items/IPItem.h \
    src/items/MXItem.h \
    src/items/NSItem.h \
    src/models/ExplorerModel.h \
    src/modules/active/PingScanner.h \
    src/modules/active/URLScanner.h \
    src/modules/active/utils/iphdr.h \
    src/modules/passive/OsintHeaders.h \
    src/modules/passive/api/ASRank.h \
    src/modules/passive/api/CirclPublic.h \
    src/modules/passive/api/HackerTarget.h \
    src/modules/passive/api/Mnemonic.h \
    src/modules/passive/api/Otx.h \
    src/modules/passive/api/PassiveTotal.h \
    src/models/ProjectModel.h \
    src/items/RawItem.h \
    src/items/SSLItem.h \
    src/items/URLItem.h \
    src/items/WildcardItem.h \
    src/modules/active/PortScanner.h \
    src/modules/passive/OsintDefinitions.h \
    src/modules/passive/api/HaveIBeenPawned.h \
    src/modules/passive/api/Host.h \
    src/modules/passive/api/JsonWhois.h \
    src/modules/passive/api/LeakIX.h \
    src/modules/passive/api/NetworksDB.h \
    src/modules/passive/api/NeutrinoApi.h \
    src/modules/passive/api/Robtex.h \
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
    src/modules/passive/api/Maltiverse.h \
    src/modules/passive/api/MnemonicFree.h \
    src/modules/passive/api/N45HT.h \
    src/modules/passive/api/Onyphe.h \
    src/modules/passive/api/OtxFree.h \
    src/modules/passive/iana/Ripe.h \
    src/modules/passive/api/RobtexFree.h \
    src/modules/passive/api/SpamHaus.h \
    src/modules/passive/api/WebResolver.h \
    src/modules/passive/iana/Afrinic.h \
    src/modules/passive/ip/IpData.h \
    src/modules/passive/ip/IpGeoLocation.h \
    src/modules/passive/ip/IpRegistry.h \
    src/modules/passive/ip/IpStack.h \
    src/modules/passive/ip/Ipfy.h \
    src/modules/passive/mal/AbuseIPDB.h \
    src/modules/passive/archive/Arquivo.h \
    src/modules/passive/mal/BotScout.h \
    src/modules/passive/mal/FraudGuard.h \
    src/modules/passive/mal/HybridAnalysis.h \
    src/modules/passive/mal/IpQualityScore.h \
    src/modules/passive/mal/LeakLookup.h \
    src/modules/active/DNSScanner.h \
    src/modules/active/SSLScanner.h \
    src/modules/passive/scrape/BingSearch.h \
    src/modules/passive/scrape/GoogleSearch.h \
    src/modules/passive/scrape/YahooSearch.h \
    src/project/Project.h \
    src/enums/AbstractEnum.h \
    src/enums/asn/ASNEnum.h \
    src/enums/cidr/CIDREnum.h \
    src/enums/email/EmailEnum.h \
    src/enums/ip/IPEnum.h \
    src/enums/mx/MXEnum.h \
    src/enums/ns/NSEnum.h \
    src/enums/ssl/SSLEnum.h \
    src/modules/active/BruteScanner.h \
    src/modules/active/HostScanner.h \
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
    src/modules/passive/scrape/DogPile.h \
    src/modules/passive/scrape/DuckDuckGo.h \
    src/modules/passive/scrape/Exalead.h \
    src/modules/passive/site/Dnsdumpster.h \
    src/modules/passive/site/Netcraft.h \
    src/modules/passive/site/PagesInventory.h \
    src/modules/passive/site/Pkey.h \
    src/modules/passive/site/Rapiddns.h \
    src/modules/passive/site/SiteDossier.h \
    src/modules/passive/AbstractOsintModule.h \
    src/utils/s3s.h \
    src/utils/SyntaxHighlighter.h \
    src/utils/UpdateChecker.h \
    src/widgets/InputWidget.h \
    src/dialogs/LogViewerDialog.h \
    src/dialogs/AboutDialog.h \
    src/dialogs/ApiKeysDialog.h \
    src/dialogs/WordlistDialog.h \
    src/utils/utils.h \
    src/utils/Config.h \
    src/MainWindow.h

FORMS += \
    src/dialogs/DocumentationDialog.ui \
    src/dialogs/FailedScansDialog.ui \
    src/dialogs/SaveProjectDialog.ui \
    src/dialogs/StartupDialog.ui \
    src/dialogs/config/BruteConfigDialog.ui \
    src/dialogs/config/DNSConfigDialog.ui \
    src/dialogs/config/EnumConfigDialog.ui \
    src/dialogs/config/HostConfigDialog.ui \
    src/dialogs/config/IPConfigDialog.ui \
    src/dialogs/config/OsintConfigDialog.ui \
    src/dialogs/config/ProjectConfigDialog.ui \
    src/dialogs/config/RawConfigDialog.ui \
    src/dialogs/config/SSLConfigDialog.ui \
    src/dialogs/config/URLConfigDialog.ui \
    src/tools/ip/IPTool.ui \
    src/tools/url/URLTool.ui \
    src/tools/ssl/SSLTool.ui \
    src/tools/raw/RawTool.ui \
    src/tools/dns/DNSTool.ui \
    src/tools/brute/BruteTool.ui \
    src/tools/osint/OsintTool.ui \
    src/tools/host/HostTool.ui \
    src/project/Project.ui \
    src/enums/asn/ASNEnum.ui \
    src/enums/cidr/CIDREnum.ui \
    src/enums/email/EmailEnum.ui \
    src/enums/ip/IPEnum.ui \
    src/enums/mx/MXEnum.ui \
    src/enums/ns/NSEnum.ui \
    src/enums/ssl/SSLEnum.ui \
    src/widgets/InputWidget.ui \
    src/dialogs/LogViewerDialog.ui \
    src/dialogs/AboutDialog.ui \
    src/dialogs/WordlistDialog.ui \
    src/dialogs/ApiKeysDialog.ui \
    src/MainWindow.ui

RESOURCES += \
    res.qrc

win32 {
    LIBS += -lAdvapi32
}

#
# FOR WINDOWS SYN SCAN
#win32 {
#    INCLUDEPATH += ../vendor/npcap/include
#    LIBS += -L"lib" -lAdvapi32
#
#    contains(QT_ARCH, i386) {
#        LIBS += -L"$$PWD/../vendor/npcap/lib" -lPacket -lwpcap
#    } else {
#        LIBS += -L"$$PWD/../vendor/npcap/lib/x64" -lPacket -lwpcap
#    }
#}

# setting the icon...
RC_ICONS = res/icons/main_logo.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

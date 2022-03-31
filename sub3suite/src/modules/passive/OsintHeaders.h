#ifndef OSINTMODULESHEADERS_H
#define OSINTMODULESHEADERS_H

///
/// api...
///
#include "src/modules/passive/api/Wappalyzer.h"
#include "src/modules/passive/api/ViewDns.h"
#include "src/modules/passive/api/ZoomEye.h"
#include "src/modules/passive/api/ZETAlytics.h"
#include "src/modules/passive/api/WhoisXmlApi.h"
#include "src/modules/passive/api/ThreatBook.h"
#include "src/modules/passive/api/Shodan.h"
#include "src/modules/passive/api/SecurityTrails.h"
#include "src/modules/passive/api/RobtexFree.h"
#include "src/modules/passive/api/Robtex.h"
#include "src/modules/passive/api/RiskIq.h"
#include "src/modules/passive/api/MnemonicFree.h"
#include "src/modules/passive/api/Mnemonic.h"
#include "src/modules/passive/api/Github.h"
#include "src/modules/passive/api/C99.h"
#include "src/modules/passive/api/BinaryEdge.h"
#include "src/modules/passive/api/Bgpview.h"
#include "src/modules/passive/api/Anubis.h"
#include "src/modules/passive/api/OtxFree.h"
#include "src/modules/passive/api/Otx.h"
#include "src/modules/passive/api/Sublist3r.h"
#include "src/modules/passive/api/Threatminer.h"
#include "src/modules/passive/api/Threatcrowd.h"
#include "src/modules/passive/api/HackerTargetFree.h"
#include "src/modules/passive/api/HackerTarget.h"
#include "src/modules/passive/api/Dnsbufferoverun.h"
#include "src/modules/passive/api/Projectdiscovery.h"
#include "src/modules/passive/api/Spyse.h"
#include "src/modules/passive/api/Omnisint.h"
#include "src/modules/passive/api/Urlscan.h"
#include "src/modules/passive/api/Circl.h"
#include "src/modules/passive/api/CirclPublic.h"
#include "src/modules/passive/api/WebResolver.h"
#include "src/modules/passive/api/BuiltWith.h"
#include "src/modules/passive/api/Dnslytics.h"
#include "src/modules/passive/api/DomainTools.h"
#include "src/modules/passive/api/Maltiverse.h"
#include "src/modules/passive/api/N45HT.h"
#include "src/modules/passive/api/Onyphe.h"
#include "src/modules/passive/api/FullHunt.h"
#include "src/modules/passive/api/SpamHaus.h"
#include "src/modules/passive/api/HaveIBeenPawned.h"
#include "src/modules/passive/api/Host.h"
#include "src/modules/passive/api/JsonWhois.h"
#include "src/modules/passive/api/LeakIX.h"
#include "src/modules/passive/api/NetworksDB.h"
#include "src/modules/passive/api/NeutrinoApi.h"
#include "src/modules/passive/api/Seon.h"
#include "src/modules/passive/api/SpyOnWeb.h"
#include "src/modules/passive/api/Whatcms.h"
#include "src/modules/passive/api/Whoxy.h"
#include "src/modules/passive/api/PassiveTotal.h"
#include "src/modules/passive/api/ASRank.h"
///
/// iana...
///
#include "src/modules/passive/iana/Arin.h"
#include "src/modules/passive/iana/Afrinic.h"
#include "src/modules/passive/iana/Ripe.h"
#include "src/modules/passive/iana/Apnic.h"
///
/// cert...
///
#include "src/modules/passive/cert/Crtsh.h"
#include "src/modules/passive/cert/GoogleCert.h"
#include "src/modules/passive/cert/CertspotterFree.h"
#include "src/modules/passive/cert/Certspotter.h"
#include "src/modules/passive/cert/CensysFree.h"
#include "src/modules/passive/cert/Censys.h"
///
/// site...
///
#include "src/modules/passive/site/PagesInventory.h"
#include "src/modules/passive/site/SiteDossier.h"
#include "src/modules/passive/site/Dnsdumpster.h"
#include "src/modules/passive/site/Netcraft.h"
#include "src/modules/passive/site/Pkey.h"
#include "src/modules/passive/site/Rapiddns.h"
///
/// ip...
///
#include "src/modules/passive/ip/IpInfo.h"
#include "src/modules/passive/ip/IpApi.h"
#include "src/modules/passive/ip/IpData.h"
#include "src/modules/passive/ip/IpGeoLocation.h"
#include "src/modules/passive/ip/IpRegistry.h"
#include "src/modules/passive/ip/IpStack.h"
#include "src/modules/passive/ip/Ipfy.h"
///
/// email...
///
#include "src/modules/passive/email/Hunter.h"
#include "src/modules/passive/email/Debounce.h"
#include "src/modules/passive/email/EmailCrawlr.h"
#include "src/modules/passive/email/EmailFormat.h"
#include "src/modules/passive/email/EmailRep.h"
#include "src/modules/passive/email/Snov.h"
#include "src/modules/passive/email/TruMail.h"
///
/// mal...
///
#include "src/modules/passive/mal/VirusTotal.h"
#include "src/modules/passive/mal/AbuseIPDB.h"
#include "src/modules/passive/mal/BotScout.h"
#include "src/modules/passive/mal/FraudGuard.h"
#include "src/modules/passive/mal/HybridAnalysis.h"
#include "src/modules/passive/mal/IpQualityScore.h"
#include "src/modules/passive/mal/LeakLookup.h"
///
/// archive...
///
#include "src/modules/passive/archive/Waybackmachine.h"
#include "src/modules/passive/archive/ArchiveToday.h"
#include "src/modules/passive/archive/CommonCrawl.h"
#include "src/modules/passive/archive/ArchiveIt.h"
#include "src/modules/passive/archive/Arquivo.h"
#include "src/modules/passive/archive/UKWebArchive.h"
///
/// scrape
///
#include "src/modules/passive/scrape/BingSearch.h"
#include "src/modules/passive/scrape/YahooSearch.h"
#include "src/modules/passive/scrape/Exalead.h"
#include "src/modules/passive/scrape/DuckDuckGo.h"
#include "src/modules/passive/scrape/DogPile.h"
#include "src/modules/passive/scrape/Baidu.h"
#include "src/modules/passive/scrape/Ask.h"
#include "src/modules/passive/scrape/GoogleSearch.h"

#endif // OSINTMODULESHEADERS_H

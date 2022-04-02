/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : utility file
*/


#ifndef UTILS_H
#define UTILS_H

#include <QtGlobal>
#include <QDnsLookup>
#include <QListWidget>
#include <QHostAddress>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>


#define NEWLINE "\n"
#define TRUE "1"
#define FALSE "0"

///
/// osint engines...
///
#define OSINT_VIRUSTOTALAPI "virustotalapi"
#define OSINT_OMNISINT "omnisint"
#define OSINT_QWANT "qwant"
#define OSINT_URLSCAN "urlscan"
#define OSINT_RAPIDDNS "rapiddns"
#define OSINT_PENTESTTOOLS "pentesttools"
#define OSINT_PROJECTDISCOVERY "projectdiscovery"
#define OSINT_SPYSE "spyse"
#define OSINT_THREATMINER "threatminer"
#define OSINT_SHODAN "shodan"
#define OSINT_GOOGLE "google"
#define OSINT_BING "bing"
#define OSINT_GITHUB "github"
#define OSINT_CENSYS "censys"
#define OSINT_SECURITYTRAILS "securitytrails"
#define OSINT_CLOUDFLARE "cloudflare"
#define OSINT_INTELX "intelx"
#define OSINT_VIRUSTOTAL "virustotal"
#define OSINT_CERTSPOTTER "certspotter"
#define OSINT_CRTSH "crtsh"
#define OSINT_DOGPILE "dogpile"
#define OSINT_DUCKDUCKGO "duckduckgo"
#define OSINT_EXALEAD "exalead"
#define OSINT_HUNTERSEARCH "huntersearch"
#define OSINT_NETCRAFT "netcraft"
#define OSINT_OTX "otx"
#define OSINT_SUIP "suip"
#define OSINT_TRELLO "trello"
#define OSINT_THREATCROWD "threatcrowd"
#define OSINT_DNSBUFFEROVERRUN "dnsbufferoverrun"
#define OSINT_HACKERTARGET "hackertarget"
#define OSINT_PKEY "pkey"
#define OSINT_WAYBACKMACHINE "waybackmachine"
#define OSINT_ASK "ask"
#define OSINT_BAIDU "baidu"
#define OSINT_DNSDUMPSTER "dnsdumpster"
#define OSINT_PASSIVEDNS "passivedns"
#define OSINT_YAHOO "yahoo"

///
/// placeholder texts...
///
#define PLACEHOLDERTEXT_DOMAIN "domain e.g. example.com"
#define PLACEHOLDERTEXT_IP "ip-address e.g. 8.8.8.8"
#define PLACEHOLDERTEXT_IP4 "IpV4-address e.g. 8.8.8.8"
#define PLACEHOLDERTEXT_IP_OR_CIDR "ip-address e.g. 1.1.1.1 or ip-address/cidr e.g. 1.1.1.0/24"
#define PLACEHOLDERTEXT_DOMAIN_OR_IP "domain name e.g. example.com or ip-address e.g. 1.1.1.1"
#define PLACEHOLDERTEXT_IP6 "IpV6-address e.g. 2001:4860:4860::8888"
#define PLACEHOLDERTEXT_ASN "asn number e.g. 12345"
#define PLACEHOLDERTEXT_EMAIL "email-address e.g. something@email.com"
#define PLACEHOLDERTEXT_CIDR "ip-address/cidr e.g. 1.1.1.0/24"
#define PLACEHOLDERTEXT_QUERY "query term e.g. GOOGLE"
#define PLACEHOLDERTEXT_SSLCERT "cert fingerprint(sha1 or sha256)"
#define PLACEHOLDERTEXT_PHONE "phone number e.g. +2557298984323"
#define PLACEHOLDERTEXT_USERNAME "username e.g. John Doe"
#define PLACEHOLDERTEXT_URL "url e.g. https://example.com/something"
#define PLACEHOLDERTEXT_NONE ""
#define PLACEHOLDERTEXT_NS "dns server name e.g. dns.google.com"
#define PLACEHOLDERTEXT_HOSTNAME "hostname e.g. test.example.com"
#define PLACEHOLDERTEXT_ORG "organization name e.g. GOOGLE LLC"
#define PLACEHOLDERTEXT_MX "mailserver name e.g. aspmx.l.google.com"
#define PLACEHOLDERTEXT_MAC_ADDRESS "mac address e.g. 00-05-02-34-56-78"
#define PLACEHOLDERTEXT_POC "point of contact e.g. KOSTE-ARIN"
#define PLACEHOLDERTEXT_NET "network eg"
#define PLACEHOLDERTEXT_CHECKSUM "sha256 checksum eg"
#define PLACEHOLDERTEXT_MD5 "md5 eg"
#define PLACEHOLDERTEXT_IP_OR_ASN_OR_CIDR "ip or asn or cidr"
#define PLACEHOLDERTEXT_IP_OR_ASN "ip or asn"
#define PLACEHOLDERTEXT_IP_OR_DOMAIN "ip or domain"
#define PLACEHOLDERTEXT_IP_RANGE "ip range"
#define PLACEHOLDERTEXT_IP_RANGE_OR_CIDR "ip range or cidr"
#define PLACEHOLDERTEXT_AS "as e.g. AS15169"
#define PLACEHOLDERTEXT_COUNTRY_CODE "2-digit ISO-3166 country code e.g. tz"
#define PLACEHOLDERTEXT_ANY "asn, ip, cidr, organization"
#define PLACEHOLDERTEXT_CIDR_OR_ASN "cidr or asn"
#define PLACEHOLDERTEXT_ADSENSE "google adsense id e.g. pub-5953444431482912"
#define PLACEHOLDERTEXT_NAME "domain owner's name"
#define PLACEHOLDERTEXT_UA "User-Agent"
#define PLACEHOLDERTEXT_BULK_IP "ip-address,ip-address"

struct ScanStatus {
    bool isNotActive = true;
    bool isRunning = false;
    bool isStopped = false;
    bool isPaused = false;
    int activeScanThreads = 0;
};

enum class RESULT_TYPE {
    SUBDOMAINIP,
    SUBDOMAIN,
    TLD,
    IP,
    DNS,
    SRV,
    CNAME,
    TXT,
    NS,
    MX,
    A,
    AAAA,
    ASN,
    CIDR,
    CERT_ID,
    CERT_INFO,
    URL,
    EMAIL,
    JSON,
    CSV,
    ITEM,
    IPV4,
    IPV6
};

enum class ENGINE {
    BRUTE,
    TLDBRUTE,
    SUBBRUTE,
    DNS,
    ACTIVE,
    OSINT,
    RAW,
    SSL,
    URL
};

enum class ENUMERATOR {
    MX,
    NS,
    CIDR,
    ASN,
    IP,
    EMAIL,
    SSL
};

#endif // UTILS_H

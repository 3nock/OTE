#ifndef UTILS_H
#define UTILS_H

#include <QtGlobal>
#include <QDnsLookup>
#include <QListWidget>
#include <QHostAddress>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>


/* common macros.... */
#define NEWLINE "\n"
#define TRUE "1"
#define FALSE "0"

/* osint engines... */
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


enum class RESULT_TYPE{
    SUBDOMAINIP,
    SUBDOMAIN,
    TLD,
    IP,
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
    EMAIL
};

enum class ENGINE{
    BRUTE,
    TLDBRUTE,
    SUBBRUTE,
    DNS,
    ACTIVE,
    OSINT,
    IP,
    RAW,
    CERT
};

enum class TOOL{
    DOMAINTOOL,
    MX,
    NS,
    CIDR,
    ASN,
    IP,
    EMAIL,
    CERT,
    BANNER
};

#endif // UTILS_H

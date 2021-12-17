#ifndef UTILS_H
#define UTILS_H

#include <QDnsLookup>
#include <QListWidget>
#include <QHostAddress>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

/***************************************************
                    MACROS
****************************************************/

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

/****************************************
                ENUMS
*****************************************/

enum class TARGET_TYPE{
    HOSTNAME = 0,
    IP,
    ASN,
    CIDR,
    CERT,
    URL,
    EMAIL,
};

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

enum class RESULT_MODEL_TYPE{
    SUBDOMAINIP,
    SUBDOMAIN,
    TLD,
    IP,
    DNS,
    ASN,
    CIDR,
    CERT_ID,
    CERT_INFO,
    URL,
    EMAIL,
    RAW
};

enum class ENGINE{
    BRUTE,
    SUBBRUTE,
    TLDBRUTE,
    RECORDS,
    DNS,
    ACTIVE,
    OSINT,
    IP,
    RAW,
    CERT
};

/***************************************************
                    STRUCTURES
****************************************************/


/***************************************************
                    FUNCTIONS
****************************************************/
QHostAddress RandomNameserver(bool useCustomNameservers);
//...
QString TargetNameFilter(QString domainName, ENGINE engineName);
//...
QString wordlistFilter(QString word);
//...
QString filter(QString word);

#endif // UTILS_H

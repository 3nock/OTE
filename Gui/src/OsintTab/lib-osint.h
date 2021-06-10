#ifndef LIBOSINT_H
#define LIBOSINT_H

// includes...
#include "src/core.h"


#define OSINT_TRUE "1"
#define OSINT_FALSE "0"
// osint engines...
#define ENGINE_VIRUSTOTALAPI "virustotalapi"
#define ENGINE_OMNISINT "omnisint"
#define ENGINE_QWANT "qwant"
#define ENGINE_URLSCAN "urlscan"
#define ENGINE_RAPIDDNS "rapiddns"
#define ENGINE_PENTESTTOOLS "pentesttools"
#define ENGINE_PROJECTDISCOVERY "projectdiscovery"
#define ENGINE_SPYSE "spyse"
#define ENGINE_THREATMINER "threatminer"
#define ENGINE_SHODAN "shodan"
#define ENGINE_GOOGLE "google"
#define ENGINE_BING "bing"
#define ENGINE_GITHUB "github"
#define ENGINE_CENSYS "censys"
#define ENGINE_SECURITYTRAILS "securitytrails"
#define ENGINE_CLOUDFLARE "cloudflare"
#define ENGINE_INTELX "intelx"
#define ENGINE_VIRUSTOTAL "virustotal"
#define ENGINE_CERTSPOTTER "certspotter"
#define ENGINE_CRTSH "crtsh"
#define ENGINE_DOGPILE "dogpile"
#define ENGINE_DUCKDUCKGO "duckduckgo"
#define ENGINE_EXALEAD "exalead"
#define ENGINE_HUNTERSEARCH "huntersearch"
#define ENGINE_NETCRAFT "netcraft"
#define ENGINE_OTX "otx"
#define ENGINE_SUIP "suip"
#define ENGINE_TRELLO "trello"
#define ENGINE_SAN "san"
#define ENGINE_THREATCROWD "threatcrowd"
#define ENGINE_DNSBUFFEROVERRUN "dnsbufferoverrun"
#define ENGINE_HACKERTARGET "hackertarget"
#define ENGINE_PKEY "pkey"
#define ENGINE_WAYBACKMACHINE "waybackmachine"
#define ENGINE_ASK "ask"
#define ENGINE_BAIDU "baidu"
#define ENGINE_DNSDUMPSTER "dnsdumpster"
#define ENGINE_PASSIVEDNS "passivedns"
#define ENGINE_YAHOO "yahoo"

// file's paths...
#define FILE_CONFIG "/config/osint-config.json"
#define FILE_APIKEYS "/config/osint-apiKeys.json"
#define FILE_PROFILES "/config/osint-profiles.json"

// configurations...
#define CONFIG_THREADS "threads"
#define CONFIG_TIMEOUT "timeout"
#define CONFIG_MAX_PAGES "max_pages"

// functions...
QString GetConfig(QString configType, int enumName);
void SetConfig(QString configType, int enumName, QString configValue);

// structures...
struct ScanArguments_Osint{
    QStringList choosenOptions;
    char *targetDomain;
};
typedef struct  ScanArguments_osint ScanArguments_osint;

struct ScanResults_Osint{
    QLabel *label_subdomainsCount;
    QStandardItemModel *results_model;
    int *resultsCount;
};
typedef struct ScanResults_Osint ScanResults_Osint;

#endif // LIBOSINT_H

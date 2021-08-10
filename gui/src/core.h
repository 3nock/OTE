#ifndef DNS_MAIN_H
#define DNS_MAIN_H

/***************************************************
                    HEADERS
****************************************************/
#include <QLineEdit>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QListWidgetItem>
#include <QListWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>
#include <QMenu>
#include <QFont>
#include <QtGui>
//...
#include <QThread>
#include <QString>
#include <QtCore>
//...
#include <QDir>
#include <QFile>
#include <QTemporaryFile>
#include <QTextStream>
//...
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
//...
#include <QHostAddress>
#include <QDnsLookup>
#include <QHostInfo>
#include <QTcpSocket>
#include <QUrl>
//...
#include <QProgressBar>
#include <QMainWindow>
#include <QObject>
#include <QWidget>
//...
#undef slots
#include "Python.h"
#define slots
// self defined...
#include "src/project/ProjectDataModel.h"

/***************************************************
                    MACROS
****************************************************/
#define SINGLE_TARGET 0
#define MULTIPLE_TARGETS 1
//...
#define NEWLINE "\n"
#define EMPTY ""
#define TITLE_ERROR "Error!"
//...
#define CURRENT_PATH "./"
#define INFO_LOADFILE "Load File..."
#define INFO_SAVETOFILE "Save To File..."
//...
#define WORDLIST_SPECIAL_SUBBRUTE "/wordlists/special_subBrute/names.txt"
#define WORDLIST_SPECIAL_TLDBRUTE "/wordlists/special_tldBrute/names.txt"
#define WORDLIST_NAMESERVERS "/wordlists/nameservers.txt"
#define WORDLIST_CUSTOM_NAMESERVERS "/wordlists/custom_nameservers.txt"
#define WORDLIST_SHORT_SUBBRUTE "/wordlists/subBrute_Short.txt"
#define WORDLIST_SHORT_TLDBRUTE "/wordlists/tldBrute_Short.txt"
#define WORDLIST_MEDIUM_SUBBRUTE "/wordlists/subBrute_Medium.txt"
#define WORDLIST_MEDIUM_TLDBRUTE "/wordlists/tldBrute_Medium.txt"
#define WORDLIST_LONG_SUBBRUTE "/wordlists/subBrute_Long.txt"
#define WORDLIST_LONG_TLDBRUTE "/wordlists/tldBrute_Long.txt"

/**********************************************
                 Macros
***********************************************/
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

/**********************************************
                   enums
***********************************************/
#define OSINT_TRUE "1"
#define OSINT_FALSE "0"

/***************************************************
                    ENUMERATORS
****************************************************/
enum ENGINE{
    // brute...
    BRUTE = 0,
    SUBBRUTE = 1,
    TLDBRUTE = 2,
    // records...
    RECORDS = 3,
    DNSRECORDS = 4,
    SRVRECORDS = 5,
    // ...
    ACTIVE = 6,
    OSINT = 7,
    IP = 8,
    LEVEL = 9
};

enum CHOICE{
    susbdomains = 0,
    ipaddress = 1,
    all = 2,
    //...
    srvName = 3,
    srvTarget = 4,
    //...
    CNAME = 5,
    TXT = 6,
    NS = 7,
    MX = 8,
    A = 9,
    AAAA = 10
};

/***************************************************
                    STRUCTURES
****************************************************/
struct RecordsModel{
    QStandardItemModel *model_srv = nullptr;
    QStandardItemModel *model_records = nullptr;
    QStandardItem *rootItem = nullptr;
    int resultsCount = NULL;
};
typedef struct RecordsModel RecordsModel;

struct ResultsModel{
    QStandardItemModel *ip = nullptr;
    QStandardItemModel *osint = nullptr;
    QStandardItemModel *brute = nullptr;
    QStandardItemModel *active = nullptr;
    QStandardItemModel *level = nullptr;
    QStandardItemModel *projectModel = nullptr;
    //...
    RecordsModel *records = nullptr;
    ProjectDataModel *project = nullptr;
};
typedef struct ResultsModel ResultsModel;

struct ScanStatus{
    bool isRunning = false;
    bool isStopped = false;
    bool isPaused = false;
};
typedef struct ScanStatus ScanStatus;

struct ScanConfig{
    QDnsLookup::Type dnsRecordType = QDnsLookup::A;
    bool useCustomNameServers = false;
    int threadsCount = 50;
    int timeout = 3000;
    //...
    bool checkWildcard = false;
    bool hasWildcard = false;
    QString wildcardIp = nullptr;
};
typedef struct ScanConfig ScanConfig;

/***************************************************
                    FUNCTIONS
****************************************************/
void SetConfig(QString configType, ENGINE engineName, QString configValue);
//...
QString GetConfig(QString configType, ENGINE engineName);
//...
QHostAddress RandomNameserver(bool useCustomNameservers);
//...
QString TargetNameFilter(QString domainName, ENGINE engineName);
//...
QString EnumName(ENGINE engineName);
//...
QString wordlistFilter(QString word);
//...
QString filter(QString word);
//...
void loadFile(QListWidget& wordlist, int& count);

#endif // _DNS_MAIN_H

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
struct ProjectResult{
    QStandardItemModel *model = nullptr;
    QStandardItem *rootItem = nullptr;
    QStandardItem *subdomains = nullptr;
    QStandardItem *tld = nullptr;
    //...
    QStandardItem *records = nullptr;
    QStandardItem *srv = nullptr;
    QStandardItem *a = nullptr;
    QStandardItem *aaaa = nullptr;
    QStandardItem *mx = nullptr;
    QStandardItem *txt = nullptr;
    QStandardItem *cname = nullptr;
    QStandardItem *ns = nullptr;
};
typedef struct ProjectResult ProjectResult;

struct RecordsResults{
    QStandardItemModel *model_srv = nullptr;
    QStandardItemModel *model_records = nullptr;
    QStandardItem *rootItem = nullptr;
    int resultsCount = NULL;
};
typedef struct RecordsResults RecordsResults;

struct ResultsModel{
    QStandardItemModel *ip = nullptr;
    QStandardItemModel *osint = nullptr;
    QStandardItemModel *brute = nullptr;
    QStandardItemModel *active = nullptr;
    QStandardItemModel *level = nullptr;
    QStandardItemModel *records = nullptr;
    //...
    RecordsResults *record = nullptr;
    ProjectResult *project = nullptr;
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

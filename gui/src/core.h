#ifndef DNS_MAIN_H
#define DNS_MAIN_H

/***************************************************
                    HEADERS
****************************************************/
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
    OSINT = 1,
    SUBBRUTE = 2,
    TLDBRUTE = 3,
    ACTIVE = 4,
    RECORDS = 5,
    BRUTE = 6,
    IP = 7,
    LEVEL = 8
};

/***************************************************
                    STRUCTURES
****************************************************/
struct ResultsModel{
    QStandardItemModel *ip = nullptr;
    QStandardItemModel *osint = nullptr;
    QStandardItemModel *brute = nullptr;
    QStandardItemModel *active = nullptr;
    QStandardItemModel *record = nullptr;
    QStandardItemModel *level = nullptr;
    QStandardItemModel *save = nullptr;
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

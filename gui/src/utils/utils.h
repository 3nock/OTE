#ifndef UTILS_H
#define UTILS_H

#include <QDnsLookup>
#include <QListWidget>
#include <QHostAddress>
#include <QStandardItemModel>
#include "src/project/ProjectDataModel.h"

/***************************************************
                    MACROS
****************************************************/
#define NEWLINE "\n"

/****************************************
                ENUMS
*****************************************/
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
    IP = 8
};

/***************************************************
                    STRUCTURES
****************************************************/
struct ScanConfig{
    QString name = nullptr;
    //...
    QDnsLookup::Type dnsRecordType = QDnsLookup::A;
    bool useCustomNameServers = false;
    QStringList customNameServers;
    int threadsCount = 50;
    int timeout = 3000;
    //...
    bool checkWildcard = false;
    bool hasWildcard = false;
    QString wildcardIp = nullptr;
};

struct ResultsModel{
    QStandardItemModel *ip = nullptr;
    QStandardItemModel *osint = nullptr;
    QStandardItemModel *brute = nullptr;
    QStandardItemModel *active = nullptr;
    QStandardItemModel *dnsrecords = nullptr;
    QStandardItemModel *srvrecords = nullptr;
    //...
    ProjectDataModel *project = nullptr;
};


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

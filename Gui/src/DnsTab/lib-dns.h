#ifndef LIBDNS_H
#define LIBDNS_H

#include "src/core.h"

// Structures...
struct scanArguments_dnsRecords{
    QListWidget *targetWordlist = nullptr;
    int currentTargetToEnumerate = 0;
    int choiceCount = 0;
    bool RecordType_a = false;
    bool RecordType_aaaa = false;
    bool RecordType_mx = false;
    bool RecordType_ns = false;
    bool RecordType_txt = false;
    bool RecordType_cname = false;
    bool RecordType_srv = false;
}; typedef struct scanArguments_dnsRecords scanArguments_dnsRecords;

struct scanArguments_lookup{
    QListWidget* targetWordlist = nullptr;
    int currentItemToEnumerate = 0;
}; typedef struct scanArguments_lookup scanArguments_lookup;

struct scanResults_dnsRecords{
    QStandardItem* rootItem = nullptr;
    QLabel *resultsCountLabel = nullptr;
    int resultsCount = 0;
}; typedef struct scanResults_dnsRecords scanResults_dnsRecords;

struct scanResults_lookup{
    int hostnameCount = 0;
    int ipAddressCount = 0;
    QStandardItemModel* model = nullptr;
    QLabel* resultsCountLabel = nullptr;
}; typedef struct scanResults_lookup scanResults_lookup;

#endif // LIBDNS_H

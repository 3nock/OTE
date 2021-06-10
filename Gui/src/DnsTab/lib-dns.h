#ifndef LIBDNS_H
#define LIBDNS_H

#include "src/core.h"

// Structures...
struct scanArguments_dnsRecords{
    QListWidget *targetWordlist = nullptr;
    int currentTargetToEnumerate = 0;
    int choiceCount = 0;
    bool RecordType_mx = false;
    bool RecordType_ns = false;
    bool RecordType_txt = false;
    bool RecordType_cname = false;
    bool RecordType_srv = false;
};
typedef struct scanArguments_dnsRecords scanArguments_dnsRecords;

struct scanResults_dnsRecords{
    QStandardItem *root_item = nullptr;
    QLabel *resultsCountLabel = nullptr;
    int resultsCount = 0;
};
typedef struct scanResults_dnsRecords scanResults_dnsRecords;

#endif // LIBDNS_H

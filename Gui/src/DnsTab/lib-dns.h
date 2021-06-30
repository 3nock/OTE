#ifndef LIBDNS_H
#define LIBDNS_H

#include "src/core.h"

// Structures...
struct ScanArguments_Records{
    QListWidget *srvWordlist = nullptr;
    QListWidget *targetList = nullptr;
    //...
    int currentTargetToEnumerate = 0;
    int currentSrvToEnumerate = 0;
    //...
    int choiceCount = 0;
    int progress = 0;
    //...
    bool RecordType_srv = false;
    bool RecordType_a = false;
    bool RecordType_aaaa = false;
    bool RecordType_mx = false;
    bool RecordType_ns = false;
    bool RecordType_txt = false;
    bool RecordType_cname = false;
};
typedef struct ScanArguments_Records ScanArguments_Records;

struct ScanResults_Records{
    QStandardItem* rootItem = nullptr;
    QLabel *resultsCountLabel = nullptr;
    int resultsCount = 0;
};
typedef struct ScanResults_Records ScanResults_Records;


#endif // LIBDNS_H

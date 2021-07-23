#ifndef LIBDNS_H
#define LIBDNS_H

#include "src/core.h"

/**************************************************
                   enums
***************************************************/
enum OPTION{
    DNSRECORDS = 0,
    SRV = 1
};

/**************************************************
                   Structures
***************************************************/
namespace record{
    struct ScanArguments{
        QListWidget *srvWordlist;
        QListWidget *targetList;
        //...
        int currentTargetToEnumerate;
        int currentSrvToEnumerate;
        int progress;
        //...
        bool RecordType_srv;
        bool RecordType_a;
        bool RecordType_aaaa;
        bool RecordType_mx;
        bool RecordType_ns;
        bool RecordType_txt;
        bool RecordType_cname;
    };
    typedef struct ScanArguments ScanArguments;

    struct ScanResults{
        QStandardItemModel* m_model_srv;
        QStandardItem* rootItem;
        //..
        QLabel *resultsCountLabel;
        QLabel *srvResultsLabel;
    };
    typedef struct ScanResults ScanResults;
}


#endif // LIBDNS_H

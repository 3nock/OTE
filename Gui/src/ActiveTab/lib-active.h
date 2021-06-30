#ifndef LIBACTIVE_H
#define LIBACTIVE_H

#include "src/core.h"

// macros...
#define ACTIVE_DNS 0
#define ACTIVE_HTTP 1
#define ACTIVE_HTTPS 2
#define ACTIVE_FTP 3
#define ACTIVE_SMTP 4

// structures....
struct ScanArguments_Active{
    QListWidget* targetList = nullptr;
    //...
    int progress = 0;
    int currentTargetToEnumerate = 0;
    //...
    QDnsLookup::Type dnsRecordType = QDnsLookup::A;
    bool useCustomNameServers = false;
    int maxThreads = 50;
    // socket...
    bool checkForService = false;
    quint16 service = 80;
    int timeout = 3000;
};
typedef struct ScanArguments_Active ScanArguments_Active;


#endif // LIBACTIVE_H

#ifndef LIBACTIVE_H
#define LIBACTIVE_H

/******************************************
                  includes
*******************************************/
#include "src/core.h"
#include "src/Dialogs/ConfigDialog.h"

/*******************************************
                   enums
********************************************/
enum ACTIVE{
    DNS = 0,
    HTTP = 1,
    HTTPS = 2,
    FTP = 3,
    SMTP = 4
};

/*******************************************
                 structures
********************************************/
struct ScanArguments_Active{
    QListWidget *targetList;
    //...
    int progress;
    int currentTargetToEnumerate;
    //...
    quint16 service;
    bool checkActiveService;
};
typedef struct ScanArguments_Active ScanArguments_Active;


#endif // LIBACTIVE_H

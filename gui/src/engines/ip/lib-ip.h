#ifndef LIBIP_H
#define LIBIP_H

#include "src/core.h"

/********************************************************
                        structures
*********************************************************/
namespace ip{
    struct ScanArguments{
        QListWidget *targetList;
        QLabel *label_resultsCount;
        QStandardItemModel *model_results;
        //...
        int progress;
        int currentTargetToEnumerate;
    };
    typedef struct ScanArguments ScanArguments;
}

#endif // LIBIP_H

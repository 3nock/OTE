#ifndef ABSTRACTCLASS_H
#define ABSTRACTCLASS_H

#include "src/utils/utils.h"


class AbstractEngine : public QWidget{
        Q_OBJECT

    public:
        explicit AbstractEngine(ENGINE engineName, ResultsModel *resultsModel, QWidget *parent = nullptr)
            : QWidget(parent),
              engineName(engineName),
              resultsModel(resultsModel)
        {
        }

    public:
        const ENGINE engineName;
        const ResultsModel *resultsModel;
        QItemSelectionModel *selectionModel = nullptr;

    signals:
        void changeTabToOsint();
        void changeTabToBrute();
        void changeTabToActive();
        void changeTabToIp();
        void changeTabToRecords();
        void changeTabToProject();
};

#endif // ABSTRACTCLASS_H

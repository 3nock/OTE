#ifndef BASECLASS_H
#define BASECLASS_H

#include "src/utils.h"
#include "ContextMenu.h"
#include "src/widgets/InputWidget.h"


class BaseClass :public  ContextMenu{
        Q_OBJECT

    public:
        BaseClass(ENGINE engineName, ResultsModel *resultsModel, QWidget *parent = nullptr);
        ~BaseClass();
        void initBaseClass(InputWidget* targets){m_targets = targets;}

    public:
        ScanStatus *scanStatus;
        ScanConfig *scanConfig;
        int activeThreads = 0;

    signals:
        void stopScan();
        void pauseScan();
        //...
        void sendLog(QString log);
        void sendStatus(QString status);

    public slots:
        void logs(QString log);
        void onReceiveTargets(ENGINE, CHOICE);
        void onReceiveTargets(QItemSelectionModel*);

    private:
        InputWidget* m_targets = nullptr;
};

#endif // BASECLASS_H

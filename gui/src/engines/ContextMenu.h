#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include <QMenu>
#include <QAction>
#include "src/utils.h"
#include "AbstractClass.h"

enum CHOICE{
    susbdomains = 0,
    ipaddress = 1,
    all = 2,
    //...
    srvName = 3,
    srvTarget = 4,
    //...
    CNAME = 5,
    TXT = 6,
    NS = 7,
    MX = 8,
    A = 9,
    AAAA = 10
};

class ContextMenu : public AbstractClass{
        Q_OBJECT

    public:
        ContextMenu(ENGINE engineName, ResultsModel *resultsModel, QWidget *parent = nullptr);
        ~ContextMenu();

    public:
        QMenu *a_Menu = nullptr;
        QMenu *c_Menu = nullptr;
        QMenu *saveMenu = nullptr;
        QMenu *copyMenu = nullptr;

    signals:
        void sendResultsToOsint(ENGINE, CHOICE);
        void sendResultsToActive(ENGINE, CHOICE);
        void sendResultsToBrute(ENGINE, CHOICE);
        void sendResultsToRecord(ENGINE, CHOICE);
        void sendResultsToIp(ENGINE, CHOICE);
        void sendResultsToProject(ENGINE, CHOICE);
        //...
        void sendResultsToOsint(QItemSelectionModel*);
        void sendResultsToActive(QItemSelectionModel*);
        void sendResultsToBrute(QItemSelectionModel*);
        void sendResultsToRecord(QItemSelectionModel*);
        void sendResultsToIp(QItemSelectionModel*);
        void sendResultsToProject(QItemSelectionModel*);

    private slots:
        void saveResults(CHOICE);
        void saveResults(QItemSelectionModel*);
        void copyResults(CHOICE);
        void copyResults(QItemSelectionModel*);
        //...
        void openInBrowser(QItemSelectionModel*);

    private:
        void initMenuA();
        void initMenuC();
        //...
        QAction actionSaveAll{"subdomain | ip"};
        QAction actionSaveSubdomains{"subdomains"};
        QAction actionSaveIpAddresses{"ip-addresses"};
        // for dns-records...
        QAction actionSaveA{"A Records"};
        QAction actionSaveAAAA{"AAAA Records"};
        QAction actionSaveMX{"MX Records"};
        QAction actionSaveNS{"NS Records"};
        QAction actionSaveCNAME{"CNAME Records"};
        QAction actionSaveTXT{"TXT Records"};
        QAction actionSaveSRVTarget{"SRV Targets"};
        QAction actionSaveSRVName{"SRV Names"};
        //...
        QAction actionCopyAll{"subdomain | ip"};
        QAction actionCopySubdomains{"subdomains"};
        QAction actionCopyIpAddresses{"ip-addresses"};
        // for dns-records...
        QAction actionCopyA{"A Records"};
        QAction actionCopyAAAA{"AAAA Records"};
        QAction actionCopyMX{"MX Records"};
        QAction actionCopyNS{"NS Records"};
        QAction actionCopyCNAME{"CNAME Records"};
        QAction actionCopyTXT{"TXT Records"};
        QAction actionCopySRVTarget{"SRV Targets"};
        QAction actionCopySRVName{"SRV Names"};
        //...
        QAction a_actionSendToIp{"Send To Ip"};
        QAction a_actionSendToOsint{"Send To Osint"};
        QAction a_actionSendToActive{"Send To Active"};
        QAction a_actionSendToBrute{"Send To Brute"};
        QAction a_actionSendToRecords{"Send To Records"};
        ///
        /// for selection right-click...
        ///
        QAction actionSave{"Save"};
        QAction actionCopy{"Copy"};
        QAction c_actionSendToIp{"Send To Ip"};
        QAction c_actionSendToOsint{"Send To Osint"};
        QAction c_actionSendToActive{"Send To Active"};
        QAction c_actionSendToBrute{"Send To Brute"};
        QAction c_actionSendToRecords{"Send To Records"};
        //...
        QAction actionOpenInBrowser{"Open in Browser"};
};

#endif // CONTEXTMENU_H

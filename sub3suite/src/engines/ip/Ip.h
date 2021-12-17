#ifndef IP_H
#define IP_H

#include "../AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/modules/scan/IpScanner.h"
#include "src/dialogs/ActiveConfigDialog.h"
#include "src/utils/NotesSyntaxHighlighter.h"


namespace Ui {
    class Ip;
}

class Ip : public AbstractEngine{
        Q_OBJECT

    public:
        Ip(QWidget *parent = nullptr,
           ResultsModel *resultsModel = nullptr,
           ProjectDataModel *project = nullptr,
           Status *status = nullptr);
        ~Ip();

    public slots:
        void onScanThreadEnded();
        void onInfoLog(QString log);
        void onErrorLog(QString log);
        void onScanResult(QString subdomain, QString ipAddress);
        /* receiving targets from other engines */
        void onReceiveTargets(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void onReceiveTargets(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);

    private slots:
        void onOpenInBrowser(QItemSelectionModel*);
        void onClearResults();
        void onRemoveResults(QItemSelectionModel*);
        void onSaveResults(RESULT_TYPE);
        void onSaveResults(QItemSelectionModel*);
        void onCopyResults(RESULT_TYPE);
        void onCopyResults(QItemSelectionModel*);
        /* ... */
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonAction_clicked();
        void on_buttonConfig_clicked();
        void on_comboBoxOption_currentIndexChanged(int index);
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);

    private:
        Ui::Ip *ui;
        ip::ScanConfig *m_scanConfig;
        ip::ScanArgs *m_scanArgs;
        QStringListModel *m_targetListModel;
        NotesSyntaxHighlighter *m_notesSyntaxHighlighter;
        /* ... */
        void m_stopScan();
        void m_startScan();
        void m_pauseScan();
        void m_resumeScan();
        void m_initActions();

    protected:
        /* general actions */
        QAction a_RemoveResults{"Remove"};
        QAction a_ClearResults{"Clear Results"};
        QAction a_OpenInBrowser{"Open in Browser"};

        /* for action button... */
        QAction a_SendIpToIp{"Send Addresses To Ip"};
        QAction a_SendIpToOsint{"Send Addresses To Osint"};
        QAction a_SendIpToRaw{"Send Address To Raw"};
        QAction a_SendHostToOsint{"Send Hostnames To Osint"};
        QAction a_SendHostToRaw{"Send Hostnames To Raw"};
        QAction a_SendHostToBrute{"Send Hostnames To Brute"};
        QAction a_SendHostToActive{"Send Hostnames To Active"};
        QAction a_SendHostToDns{"Send Hostnames To Records"};
        QAction a_SendHostToCert{"Send Hostnames To Cert"};
        QAction a_SendIpToIpTool{"Send Addresses To IpTool"};
        QAction a_SendHostToCertTool{"Send Hostnames To CertTool"};
        QAction a_SendHostToDomainTool{"Send Hostnames To DomainTool"};
        /* for right-click... */
        QAction a_SendSelectedIpToIp{"Send Addresses To Ip"};
        QAction a_SendSelectedIpToOsint{"Send Addresses To Osint"};
        QAction a_SendSelectedIpToRaw{"Send Address To Raw"};
        QAction a_SendSelectedHostToOsint{"Send Hostnames To Osint"};
        QAction a_SendSelectedHostToRaw{"Send Hostnames To Raw"};
        QAction a_SendSelectedHostToBrute{"Send Hostnames To Brute"};
        QAction a_SendSelectedHostToActive{"Send Hostnames To Active"};
        QAction a_SendSelectedHostToDns{"Send Hostnames To Records"};
        QAction a_SendSelectedHostToCert{"Send Hostnames To Cert"};
        QAction a_SendSelectedIpToIpTool{"Send Addresses To IpTool"};
        QAction a_SendSelectedHostToCertTool{"Send Hostnames To CertTool"};
        QAction a_SendSelectedHostToDomainTool{"Send Hostnames To DomainTool"};

        /* save */
        QAction a_Save{"Save"};
        QAction a_SaveSubdomainIp{"subdomain | ip"};
        QAction a_SaveSubdomain{"subdomains"};
        QAction a_SaveIp{"ip-addresses"};
        /* copy */
        QAction a_Copy{"Copy"};
        QAction a_CopySubdomainIp{"subdomain | ip"};
        QAction a_CopySubdomain{"subdomains"};
        QAction a_CopyIp{"ip-addresses"};
};

#endif // IP_H

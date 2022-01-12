/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef OSINT_H
#define OSINT_H

#include "../AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/ActiveConfigDialog.h"
#include "src/modules/passive/AbstractOsintModule.h"
#include "src/utils/NotesSyntaxHighlighter.h"


enum INPUT{
    HOSTNAME = 0,
    IP = 1,
    EMAIL = 2,
    URL = 3,
    ASN = 4,
    CERT = 5,
    CIDR = 6
};

namespace osint{

enum OUTPUT{
    SUBDOMAIN = 0,
    SUBDOMAINIP = 1,
    IP = 2,
    EMAIL = 3,
    URL = 4,
    ASN = 5,
    CERT = 6,
    CIDR = 7
};

}

namespace Ui {
    class Osint;
}

class Osint : public AbstractEngine{
    Q_OBJECT

    public:
        Osint(QWidget *parent = nullptr, ProjectDataModel *project = nullptr);
        ~Osint();

    public slots:
        void onScanThreadEnded();
        void onInfoLog(ScanLog log);
        void onErrorLog(ScanLog log);
        void onRateLimitLog(ScanLog log);
        /* ... */
        void onResultSubdomainIp(QString subdomain, QString ip);
        void onResultSubdomain(QString subdomain);
        void onResultMightContainWildcards(QString subdomain);
        void onResultIp(QString ip);
        void onResultEmail(QString email);
        void onResultUrl(QString url);
        void onResultAsn(QString asnValue, QString asnName);
        void onResultCidr(QString cidr);
        void onResultA(QString A);
        void onResultAAAA(QString AAAA);
        void onResultCNAME(QString CNAME);
        void onResultMX(QString MX);
        void onResultNS(QString NS);
        void onResultTXT(QString TXT);
        void onResultSSLCert(QString sha1_or_sha256);

        /* receiving targets from other engines */
        void onReceiveTargets(QString, RESULT_TYPE);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonAction_clicked();
        void on_buttonStop_clicked();
        void on_buttonConfig_clicked();
        void on_buttonKeys_clicked();
        void on_lineEditTarget_returnPressed();
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);
        void on_checkBoxMultipleTargets_stateChanged(int arg1);
        void on_comboBoxOutput_currentIndexChanged(int index);
        void on_comboBoxInput_currentIndexChanged(int index);
        void on_lineEditFilter_textChanged(const QString &arg1);

    private:
        Ui::Osint *ui;
        QStringListModel *m_targetListModelHostname;
        QStringListModel *m_targetListModelIp;
        QStringListModel *m_targetListModelAsn;
        QStringListModel *m_targetListModelCidr;
        QStringListModel *m_targetListModelCert;
        QStringListModel *m_targetListModelEmail;
        /* ... */
        QStandardItemModel *m_resultModelSubdomainIp;
        QStandardItemModel *m_resultModelSubdomain;
        QStandardItemModel *m_resultModelIp;
        QStandardItemModel *m_resultModelEmail;
        QStandardItemModel *m_resultModelUrl;
        QStandardItemModel *m_resultModelAsn;
        QStandardItemModel *m_resultModelCert;
        QStandardItemModel *m_resultModelCidr;
        QSortFilterProxyModel *m_resultProxyModel;
        NotesSyntaxHighlighter *m_notesSyntaxHighlighter;
        /* ... */
        QString m_currentPath;
        QSet<QString> m_subdomainIpSet;
        QSet<QString> m_subdomainSet;
        QSet<QString> m_ipSet;
        QSet<QString> m_emailSet;
        QSet<QString> m_urlSet;
        QSet<QString> m_asnSet;
        QSet<QString> m_sslCertSet;
        QSet<QString> m_cidrSet;
        /* for scan */
        void m_stopScan();
        void m_startScan();
        void m_pauseScan();
        void m_resumeScan();
        void m_startScanThread(AbstractOsintModule *);
        /* ... */
        void m_initProfiles();
        void m_initModules();
        void m_uncheckAllModules();
        /* ... */
        void m_infoLog(QString log);
        void m_errorLog(QString log);

    /* for context menu */
    private:
        void m_initActions();
        /* ... */
        void m_openInBrowser(QItemSelectionModel*);
        void m_clearResults();
        void m_removeResults(QItemSelectionModel*);
        void m_saveResults(RESULT_TYPE);
        void m_saveResults(QItemSelectionModel*);
        void m_copyResults(RESULT_TYPE);
        void m_copyResults(QItemSelectionModel*);
        /* sending results to other parts */
        void m_sendToEngine(ENGINE); // for asn, cidr, email, cert
        void m_sendSubdomainToEngine(ENGINE);
        void m_sendIpToEngine(ENGINE);
        void m_sendSubdomainToTool(TOOL);
        void m_sendIpToTool(TOOL);
        void m_sendToTool(TOOL);

        void m_sendToEngine(ENGINE, QItemSelectionModel*);
        void m_sendSubdomainToEngine(ENGINE, QItemSelectionModel*);
        void m_sendIpToEngine(ENGINE, QItemSelectionModel*);
        void m_sendSubdomainToTool(TOOL, QItemSelectionModel*);
        void m_sendIpToTool(TOOL, QItemSelectionModel*);
        void m_sendToTool(TOOL, QItemSelectionModel*);

    protected:
        /* general actions */
        QAction a_RemoveResults{"Remove"};
        QAction a_ClearResults{"Clear Results"};
        QAction a_OpenInBrowser{"Open in Browser"};

        /* for all */
        QAction a_SendAllIpToIp{"Send Addresses To Ip"};
        QAction a_SendAllIpToOsint{"Send Addresses To Osint"};
        QAction a_SendAllIpToRaw{"Send Address To Raw"};
        QAction a_SendAllHostToOsint{"Send Hostnames To Osint"};
        QAction a_SendAllHostToRaw{"Send Hostnames To Raw"};
        QAction a_SendAllHostToBrute{"Send Hostnames To Brute"};
        QAction a_SendAllHostToActive{"Send Hostnames To Active"};
        QAction a_SendAllHostToDns{"Send Hostnames To Records"};
        QAction a_SendAllHostToCert{"Send Hostnames To Cert"};
        QAction a_SendAllEmailToRaw{"Send Emails To Raw"};
        QAction a_SendAllUrlToRaw{"Send Urls To Raw"};
        QAction a_SendAllAsnToRaw{"Send ASNs To Raw"};
        QAction a_SendAllCertToRaw{"Send Cert To Raw"};
        QAction a_SendAllCidrToRaw{"Send Cidr To Raw"};
        QAction a_SendAllEmailToOsint{"Send Emails To Osint"};
        QAction a_SendAllAsnToOsint{"Send ASNs To Osint"};
        QAction a_SendAllCertToOsint{"Send Cert To Osint"};
        QAction a_SendAllCidrToOsint{"Send Cidr To Osint"};
        QAction a_SendAllIpToIpTool{"Send Addresses To IpTool"};
        QAction a_SendAllHostToCertTool{"Send Hostnames To CertTool"};
        QAction a_SendAllHostToDomainTool{"Send Hostnames To DomainTool"};
        QAction a_SendAllAsnToAsnTool{"Send ASN to ASNTool"};
        QAction a_SendAllCertToCertTool{"Send Cert To CertTool"};
        QAction a_SendAllCidrToCidrTool{"Send Cidr To CidrTool"};
        QAction a_SendAllEmailToEmailTool{"Send Emails To EmailTool"};
        /* for selected */
        QAction a_SendSelectedIpToIp{"Send Addresses To Ip"};
        QAction a_SendSelectedIpToOsint{"Send Addresses To Osint"};
        QAction a_SendSelectedIpToRaw{"Send Address To Raw"};
        QAction a_SendSelectedHostToOsint{"Send Hostnames To Osint"};
        QAction a_SendSelectedHostToRaw{"Send Hostnames To Raw"};
        QAction a_SendSelectedHostToBrute{"Send Hostnames To Brute"};
        QAction a_SendSelectedHostToActive{"Send Hostnames To Active"};
        QAction a_SendSelectedHostToDns{"Send Hostnames To Records"};
        QAction a_SendSelectedHostToCert{"Send Hostnames To Cert"};
        QAction a_SendSelectedEmailToRaw{"Send Emails To Raw"};
        QAction a_SendSelectedUrlToRaw{"Send Urls To Raw"};
        QAction a_SendSelectedAsnToRaw{"Send ASNs To Raw"};
        QAction a_SendSelectedCertToRaw{"Send Cert To Raw"};
        QAction a_SendSelectedCidrToRaw{"Send Cidr To Raw"};
        QAction a_SendSelectedEmailToOsint{"Send Emails To Osint"};
        QAction a_SendSelectedAsnToOsint{"Send ASNs To Osint"};
        QAction a_SendSelectedCertToOsint{"Send Cert To Osint"};
        QAction a_SendSelectedCidrToOsint{"Send Cidr To Osint"};
        QAction a_SendSelectedIpToIpTool{"Send Addresses To IpTool"};
        QAction a_SendSelectedHostToCertTool{"Send Hostnames To CertTool"};
        QAction a_SendSelectedHostToDomainTool{"Send Hostnames To DomainTool"};
        QAction a_SendSelectedAsnToAsnTool{"Send ASN to ASNTool"};
        QAction a_SendSelectedCertToCertTool{"Send Cert To CertTool"};
        QAction a_SendSelectedCidrToCidrTool{"Send Cidr To CidrTool"};
        QAction a_SendSelectedEmailToEmailTool{"Send Emails To EmailTool"};

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

#endif // OSINT_H

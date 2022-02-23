/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : OSINT Engine, for obtaining different osint info from different input targets.
*/

#ifndef OSINT_H
#define OSINT_H

#include "../AbstractEngine.h"
#include "src/modules/passive/AbstractOsintModule.h"

#include <QAction>


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
        Osint(QWidget *parent = nullptr, ProjectModel *project = nullptr);
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
        ScanConfig *m_scanConfig;
        ScanArgs *m_scanArgs;
        /* target listmodels */
        QStringListModel *m_targetListModel_host;
        QStringListModel *m_targetListModel_ip;
        QStringListModel *m_targetListModel_asn;
        QStringListModel *m_targetListModel_cidr;
        QStringListModel *m_targetListModel_ssl;
        QStringListModel *m_targetListModel_email;
        /* results models */
        QStandardItemModel *m_model_subdomainIp;
        QStandardItemModel *m_model_subdomain;
        QStandardItemModel *m_model_ip;
        QStandardItemModel *m_model_email;
        QStandardItemModel *m_model_url;
        QStandardItemModel *m_model_asn;
        QStandardItemModel *m_model_ssl;
        QStandardItemModel *m_model_cidr;
        /* results sets */
        QSet<QString> set_subdomainIP;
        QSet<QString> set_subdomain;
        QSet<QString> set_ip;
        QSet<QString> set_email;
        QSet<QString> set_url;
        QSet<QString> set_asn;
        QSet<QString> set_ssl;
        QSet<QString> set_cidr;
        int total_modules;
        void log(QString log);
        /* for scan */
        void startScan();
        void startScanThread(AbstractOsintModule*);
        /* ... */
        void initUI();
        void initModules();
        void initProfiles();
        void initConfigValues();
        void uncheckAllModules();


    /* for context menu */
    private:
        void clearResults();
        void openInBrowser();
        void removeResults();
        void extract(bool subdomain, bool tld);
        void extractSelected(bool subdomain, bool tld);
        void saveResults(RESULT_TYPE);
        void saveSelectedResults();
        void copyResults(RESULT_TYPE);
        void copySelectedResults();
        /* sending results */
        void sendToProject();
        void sendToEngine(ENGINE, RESULT_TYPE);
        void sendToEnum(TOOL, RESULT_TYPE);
        void sendSelectedToProject();
        void sendSelectedToEngine(ENGINE, RESULT_TYPE);
        void sendSelectedToEnum(TOOL, RESULT_TYPE);
};

#endif // OSINT_H

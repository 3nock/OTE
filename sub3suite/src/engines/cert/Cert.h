#ifndef CERT_H
#define CERT_H

#include <QWidget>
#include "../AbstractEngine.h"
#include "src/modules/scan/CertScanner.h"
#include "src/utils/NotesSyntaxHighlighter.h"


#define OUTPUT_SUBDOMAIN 0
#define OUTPUT_SSLCERT 1
#define OUTPUT_CERTINFO 2


namespace Ui {
    class Cert;
}

class Cert : public AbstractEngine{
        Q_OBJECT

    public:
        Cert(QWidget *parent = nullptr,
               ResultsModel *resultsModel = nullptr,
               ProjectDataModel *project = nullptr,
               Status *status = nullptr);
        ~Cert();

    public slots:
        void onScanThreadEnded();
        void onInfoLog(QString log);
        void onErrorLog(QString log);
        //...
        void onScanResultSHA1(QString sha1);
        void onScanResultSHA256(QString sha256);
        void onScanResultCertInfo(QByteArray cert);
        void onScanResultSubdomain(QString subdomain);
        /* receiving targets from other engines */
        void onReceiveTargets(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void onReceiveTargets(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);

    private slots:
        void onOpenInBrowser(QItemSelectionModel*);
        void onClearResults();
        void onExpandResults();
        void onCollapseResults();
        void onRemoveResults(QItemSelectionModel*);
        void onSaveResults(RESULT_TYPE);
        void onSaveResults(QItemSelectionModel*);
        void onCopyResults(RESULT_TYPE);
        void onCopyResults(QItemSelectionModel*);
        /* ... */
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonAction_clicked();
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);
        void on_comboBoxOutput_currentIndexChanged(int index);
        void on_lineEditFilter_textChanged(const QString &arg1);

    private:
        Ui::Cert *ui;
        certificate::ScanConfig *m_scanConfig;
        certificate::ScanArgs *m_scanArgs;
        QStringListModel *m_targetListModel;
        NotesSyntaxHighlighter *m_notesSyntaxHighlighter;
        /* ... */
        void m_stopScan();
        void m_startScan();
        void m_pauseScan();
        void m_ResumeScan();
        /* ... */
        void m_initActions();

    protected:
        /* general actions */
        QAction a_RemoveResults{"Remove"};
        QAction a_ClearResults{"Clear Results"};
        QAction a_ExpandResults{"Expand Results"};
        QAction a_CollapseResults{"Collapse Results"};
        QAction a_OpenInBrowser{"Open in Browser"};

        /* action button */
        QAction a_SendHostToOsint{"Send Hostnames To Osint"};
        QAction a_SendHostToRaw{"Send Hostnames To Raw"};
        QAction a_SendHostToBrute{"Send Hostnames To Brute"};
        QAction a_SendHostToActive{"Send Hostnames To Active"};
        QAction a_SendHostToDns{"Send Hostnames To Records"};
        QAction a_SendHostToCert{"Send Hostnames To Cert"};
        QAction a_SendCertToOsint{"Send Cert To Osint"};
        QAction a_SendCertToRaw{"Send Cert To Raw"};
        QAction a_SendHostToCertTool{"Send Hostnames To CertTool"};
        QAction a_SendCertToCertTool{"Send Cert To CertTool"};
        QAction a_SendHostToDomainTool{"Send Hostnames To DomainTool"};
        /* right-click */
        QAction a_SendSelectedHostToOsint{"Send Hostnames To Osint"};
        QAction a_SendSelectedHostToRaw{"Send Hostnames To Raw"};
        QAction a_SendSelectedHostToBrute{"Send Hostnames To Brute"};
        QAction a_SendSelectedHostToActive{"Send Hostnames To Active"};
        QAction a_SendSelectedHostToDns{"Send Hostnames To Records"};
        QAction a_SendSelectedHostToCert{"Send Hostnames To Cert"};
        QAction a_SendSelectedCertToOsint{"Send Cert To Osint"};
        QAction a_SendSelectedCertToRaw{"Send Cert To Raw"};
        QAction a_SendSelectedHostToCertTool{"Send Hostnames To CertTool"};
        QAction a_SendSelectedCertToCertTool{"Send Cert To CertTool"};
        QAction a_SendSelectedHostToDomainTool{"Send Hostnames To DomainTool"};

        /* save */
        QAction a_Save{"Save"};
        QAction a_SaveSubdomain{"subdomains"};
        QAction a_SaveCertId{"Cert Id"};
        QAction a_SaveCertInfo{"Cert Info"};
        /* copy */
        QAction a_Copy{"Copy"};
        QAction a_CopySubdomain{"subdomains"};
        QAction a_CopyCertId{"Cert Id"};
        QAction a_CopyCertInfo{"Cert Info"};
};

#endif // CERT_H

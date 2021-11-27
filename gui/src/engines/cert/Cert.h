#ifndef CERT_H
#define CERT_H

#include <QWidget>
#include "../AbstractEngine.h"
#include "src/modules/scan/CertScanner.h"

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
        void onScanResultSHA1(QString sha1);
        void onScanResultSHA256(QString sha256);
        void onScanResultCertInfo(QByteArray cert);
        void onScanResultSubdomain(QString subdomain);

    private slots:
        void onClearResults();
        void onSaveResults(CHOICE);
        void onSaveResults(QItemSelectionModel*);
        void onCopyResults(CHOICE);
        void onCopyResults(QItemSelectionModel*);
        ///
        /// ...
        ///
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonAction_clicked();
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);
        void on_comboBoxOutput_currentIndexChanged(int index);
        void on_lineEditFilter_textChanged(const QString &arg1);

    private:
        Ui::Cert *ui;
        certificate::ScanArguments *m_args;
        void stopScan();
        void startScan(certificate::ScanArguments*);
        void pauseScan();
        void ResumeScan();
        //...
        void connectActions();
};

#endif // CERT_H

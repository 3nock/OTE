#ifndef CERT_H
#define CERT_H

#include <QWidget>
#include "../AbstractEngine.h"
#include "src/modules/scan/CertScanner.h"

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
        void onScanResult(QByteArray cert);

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

    private:
        Ui::Cert *ui;
        certificate::ScanArguments *m_args;
        void stopScan();
        void startScan();
        void pauseScan();
        void ResumeScan();
        //...
        void connectActions();
};

#endif // CERT_H

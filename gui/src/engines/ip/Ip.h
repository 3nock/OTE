#ifndef IP_H
#define IP_H

#include "src/utils/utils.h"
#include "src/engines/AbstractEngine.h"
#include "IpScanner.h"
#include "src/dialogs/ConfigDialog.h"

namespace Ui {
    class Ip;
}

class Ip : public AbstractEngine{
        Q_OBJECT

    public:
        Ip(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr, Status *status = nullptr);
        ~Ip();

    public slots:
        void onScanThreadEnded();
        void onInfoLog(QString log);
        void onErrorLog(QString log);
        void onScanResult(QString subdomain, QString ipAddress);

    private slots:
        void onClearResults();
        void onShowFilter(bool);
        void onSaveResults(CHOICE);
        void onSaveResults(QItemSelectionModel*);
        void onCopyResults(CHOICE);
        void onCopyResults(QItemSelectionModel*);
        ///
        /// ....
        ///
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonAction_clicked();
        void on_buttonConfig_clicked();
        void on_comboBoxOption_currentIndexChanged(int index);
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);

        void on_buttonFilter_clicked();

private:
        Ui::Ip *ui;
        ip::ScanArguments *m_scanArguments;
        QStandardItemModel *m_model;
        QSortFilterProxyModel *m_proxyModel;
        void stopScan();
        void startScan();
        void pauseScan();
        void ResumeScan();
        void connectActions();
};

#endif // IP_H

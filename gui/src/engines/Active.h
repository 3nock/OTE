#ifndef ACTIVE_H
#define ACTIVE_H

#include "AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/modules/scan/ActiveScanner.h"

namespace Ui {
    class Active;
}

class Active : public AbstractEngine{
        Q_OBJECT

    public:
        Active(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr, Status *status = nullptr);
        ~Active();

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
        void on_checkBoxCustomActive_clicked(bool checked);

        void on_buttonFilter_clicked();

    private:
        Ui::Active *ui;
        active::ScanArguments *m_scanArguments;
        QStandardItemModel *m_model;
        QSortFilterProxyModel *m_proxyModel;
        void stopScan();
        void startScan();
        void pauseScan();
        void ResumeScan();
        //...
        void connectActions();
};

#endif // ACTIVE_H

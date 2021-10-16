#ifndef BRUTE_H
#define BRUTE_H

#include "AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/modules/scan/BruteScanner.h"

namespace Ui {
    class Brute;
}

class Brute : public AbstractEngine{
    Q_OBJECT

    public:
        Brute(QWidget *parent = nullptr,
              ResultsModel *resultsModel = nullptr,
              ProjectDataModel *project = nullptr,
              Status *status = nullptr);
        ~Brute();

    public slots:
        void onScanThreadEnded();
        void onInfoLog(QString log);
        void onErrorLog(QString log);
        void onScanResult(QString subdomain, QString ipAddress, QString target);
        //...
        void onChoosenWordlist(QString);

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
        void on_buttonWordlist_clicked();
        void on_lineEditTarget_returnPressed();
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);
        void on_checkBoxMultipleTargets_clicked(bool checked);

        void on_buttonFilter_clicked();

private:
        Ui::Brute *ui;
        QSet<QString> m_subdomainsSet;
        brute::ScanArguments *m_scanArguments;
        void stopScan();
        void startScan();
        void pauseScan();
        void resumeScan();
        //...
        void connectActions();
};

#endif // BRUTE_H

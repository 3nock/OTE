#ifndef OSINT_H
#define OSINT_H

#include "src/utils/utils.h"
#include "src/engines/AbstractEngine.h"
#include "src/widgets/OsintModulesWidget.h"
#include "OsintScanner.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/ConfigDialog.h"

namespace Ui {
    class Osint;
}

class Osint : public AbstractEngine{
    Q_OBJECT

    public:
        Osint(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr, Status *status = nullptr);
        ~Osint();

    public slots:
        void onInfoLog(QString log);
        void onErrorLog(QString log);
        void scanResults(QString subdomain);

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
        void on_buttonAction_clicked();
        void on_buttonStop_clicked();
        void on_buttonConfig_clicked();
        void on_buttonKeys_clicked();
        void on_lineEditTarget_returnPressed();
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);
        //...
        void onEnumerationComplete();
        void on_checkBoxMultipleTargets_clicked(bool checked);

        void on_buttonFilter_clicked();

private:
        Ui::Osint *ui;
        //...
        osint::ScanArguments *m_scanArguments;
        osint::ScanResults *m_scanResults;
        QStandardItemModel *m_model;
        QSortFilterProxyModel *m_proxyModel;
        QString currentPath;
        QString m_targetDomain;
        QSet<QString> m_results;
        int m_subdomainsCount = 0;
        void stopScan();
        void startScan();
        void pauseScan();
        void ResumeScan();
        void connectActions();
};

#endif // OSINT_H

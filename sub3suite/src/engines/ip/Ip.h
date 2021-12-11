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

    private slots:
        void onClearResults();
        void onShowFilter(bool);
        void onSaveResults(CHOICE, PROXYMODEL_TYPE);
        void onSaveResults(QItemSelectionModel*);
        void onCopyResults(CHOICE, PROXYMODEL_TYPE);
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
        ipEngine::ScanArguments *m_scanArguments;
        //...
        NotesSyntaxHighlighter *m_notesSyntaxHighlighter;
        //...
        void stopScan();
        void startScan();
        void pauseScan();
        void ResumeScan();
        void connectActions();
};

#endif // IP_H

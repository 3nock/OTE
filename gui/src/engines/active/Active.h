#ifndef ACTIVE_H
#define ACTIVE_H

#include "src/engines/Base.h"
#include "lib-active.h"
#include "ActiveEnumerator.h"

namespace Ui {
    class Active;
}

class Active : public BaseClass{
        Q_OBJECT

    public:
        explicit Active(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr);
        ~Active();

    private:
        Ui::Active *ui;

    private:
        void startScan();
        //...
        ResultsModel *m_resultsModel;
        //...
        ScanStatus *m_scanStatus;
        ScanConfig *m_scanConfig;
        ScanArguments_Active *m_scanArguments;
        //...
        int m_activeThreads = 0;

    signals:
        void stop();

    public slots:
        void a_receiveTargets(ENGINE engineName);
        void c_receiveTargets(QItemSelectionModel *selectionModel);

    public slots:
        void scanResult(QString subdomain, QString ipAddress);
        void scanThreadEnded();
        //...
        void logs(QString log);

    private slots:
        void on_pushButton_start_clicked();
        void on_pushButton_pause_clicked();
        void on_pushButton_stop_clicked();
        void on_pushButton_get_clicked();
        void on_pushButton_action_clicked();
        void on_toolButton_config_clicked();
        void on_pushButton_clearResults_clicked();
        //...
        void on_pushButton_removeTargets_clicked();
        void on_pushButton_clearTargets_clicked();
        void on_pushButton_loadTargets_clicked();
        void on_pushButton_addTargets_clicked();
        void on_lineEdit_targets_returnPressed();
        //...
        void on_comboBox_option_currentIndexChanged(int index);
        void on_tableView_results_customContextMenuRequested(const QPoint &pos);
};

#endif // ACTIVE_H

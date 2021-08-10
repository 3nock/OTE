#ifndef IP_H
#define IP_H

#include "src/engines/Base.h"
#include "IpScanner.h"
#include "src/dialogs/ConfigDialog.h"

namespace Ui {
    class Ip;
}

class Ip : public BaseClass{
        Q_OBJECT

    private:
        Ui::Ip *ui;

    private:
        void startScan();
        //...
        ip::ScanArguments *m_scanArguments;

    public:
        explicit Ip(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr);
        ~Ip();

    public slots:
        void scanResult(QString subdomain, QString ipAddress);
        void scanThreadEnded();

    private slots:
        void on_pushButton_start_clicked();
        void on_pushButton_stop_clicked();
        void on_pushButton_pause_clicked();
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

#endif // IP_H

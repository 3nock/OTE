#ifndef BRUTE_H
#define BRUTE_H

#include "src/engines/Base.h"
#include "src/engines/CommonScanners.h"
#include "BruteScanner.h"

namespace Ui {
    class Brute;
}

class Brute : public BaseClass{
    Q_OBJECT

    private:
        Ui::Brute *ui;

    private:
        void startScan();
        //...
        QSet<QString> m_subdomainsSet;
        brute::ScanArguments *m_scanArguments;

    public:
        explicit Brute(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr);
        ~Brute();

    public slots:
        void scanResult(QString subdomain, QString ipAddress, QString target);
        void scanThreadEnded();
        //...
        void choosenWordlist(QString);

    private slots:
        void on_pushButton_start_clicked();
        void on_pushButton_pause_clicked();
        void on_pushButton_stop_clicked();
        void on_pushButton_action_clicked();
        void on_toolButton_config_clicked();
        void on_pushButton_clearResults_clicked();
        void on_pushButton_addWordlist_clicked();
        void on_pushButton_clearWordlist_clicked();
        void on_pushButton_removeWordlist_clicked();
        void on_pushButton_loadWordlist_clicked();
        void on_pushButton_generateWordlist_clicked();
        void on_pushButton_removeTargets_clicked();
        void on_pushButton_clearTargets_clicked();
        void on_pushButton_loadTargets_clicked();
        void on_pushButton_addTargets_clicked();
        void on_toolButton_wordlist_clicked();
        void on_radioButton_tldBrute_clicked();
        void on_radioButton_subBrute_clicked();
        void on_lineEdit_wordlist_returnPressed();
        void on_lineEdit_target_returnPressed();
        void on_lineEdit_multipleTargets_returnPressed();
        void on_tableView_results_customContextMenuRequested(const QPoint &pos);
        void on_comboBox_target_currentIndexChanged(int index);
};

#endif // BRUTE_H

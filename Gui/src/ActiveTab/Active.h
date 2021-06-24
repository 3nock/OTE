#ifndef ACTIVE_H
#define ACTIVE_H

#include <QDialog>
#include "ActiveEnumerator.h"
#include "src/BruteTab/lib-brute.h"
#include "src/BruteTab/BruteConfigDialog.h"


namespace Ui {
    class Active;
}

class Active : public QDialog{
        Q_OBJECT

    public:
        explicit Active(QWidget *parent = nullptr);
        ~Active();

    private:
        Ui::Active *ui;

    private:
        void showContextMenu_actionButton(const QPoint& position);
        void showContextMenu_rightClick();
        //...
        QStandardItemModel *model;
        QStandardItem *results;
        ScanArguments_Brute *scanArguments;
        int subdomainCount = 0;
        int wordlistCount = 0;
        int activeThreads = 0;
        int endedThreads = 0;
        QString currentPath;


    private slots:
        void on_pushButton_get_clicked();
        void on_pushButton_remove_clicked();
        void on_pushButton_clearWordlist_clicked();
        void on_pushButton_load_clicked();
        void on_pushButton_add_clicked();
        void on_pushButton_clearResults_clicked();
        void on_pushButton_start_clicked();
        void on_pushButton_stop_clicked();
        void on_pushButton_action_clicked();
        void on_toolButton_config_clicked();
        void on_lineEdit_returnPressed();
        void on_tableView_results_customContextMenuRequested(const QPoint &pos);
        // context...
        void actionSendToMultiLevel();
        void actionSendToSave();
        void actionSendToDnsRecords();
        // cursor...
        void cursorSendToSave();
        void cursorSendToDnsRecords();
        void cursorOpenInBrowser();
        //...
        void startEnumeration();
        void resolvedSubdomain(QString subdomain, QString ipAddress);
        void onThreadEnd();
        void logs(QString log);

    public slots:

    signals:
        void stop();
        void sendStatus(QString status);
        void sendLog(QString log);
};

#endif // ACTIVE_H

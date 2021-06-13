#ifndef OSINT_H
#define OSINT_H

// headers...
#include "lib-osint.h"
#include "OsintEnumerator.h"
#include "OsintConfigDialog.h"
#include "ApiKeysDialog.h"


namespace Ui {
    class Osint;
}

class Osint : public QWidget{
    Q_OBJECT

    private:
        Ui::Osint *ui;

    private:
        void getUserOptions(QStringList *choosenOptions);
        void setupOsintProfiles();
        //...
        QStandardItemModel *results_model;
        //...
        ScanArguments_Osint *scanArguments;
        ScanResults_Osint *scanResults;
        //...
        int subdomainsCount = 0;
        //...
        QString currentPath;
        QString m_targetDomain;

    public:
        explicit Osint(QWidget *parent = nullptr);
        ~Osint();

    private slots:
        void on_pushButton_start_clicked();
        void on_pushButton_clear_clicked();
        void on_pushButton_action_clicked();
        void on_pushButton_stop_clicked();
        void on_toolButton_config_clicked();
        void on_toolButton_keys_clicked();
        void on_lineEdit_domain_returnPressed();
        void on_tableView_results_customContextMenuRequested(const QPoint &pos);
        // Engine options...
        void on_checkBox_useProfiles_clicked(bool checked);
        void on_pushButton_loadProfile_clicked();
        void on_pushButton_deleteProfile_clicked();
        void on_pushButton_newProfile_clicked();
        // slots for context menu for the action button...
        void actionSendToSave();
        void actionSendToActive();
        void actionSendToDnsRecords();
        void actionRemoveDuplicates();
        // slots for context menu for the right click button...
        void cursorSendToSave();
        void cursorSendToActive();
        void cursorSendToDnsRecords();
        void cursorOpenInBrowser();
        //...
        void onEnumerationComplete();

    public slots:
        void logs(QString log);
        void onSendResultsToSave();
        void onSendResultsToActive();

    signals:
        void stopEnumeration();
        //...
        void sendLog(QString log);
        void sendStatus(QString status);
        //...
        void sendResultsToActive(QString);
        void sendResultsToSave(QString);
        //...
        void changeTabToActive();
        void changeTabToSave();
};

#endif // OSINT_H

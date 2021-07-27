#ifndef OSINT_H
#define OSINT_H

// headers...
#include "src/engines/Base.h"
#include "lib-osint.h"
#include "OsintEnumerator.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/ConfigDialog.h"


namespace Ui {
    class Osint;
}

class Osint : public BaseClass{
    Q_OBJECT

    private:
        Ui::Osint *ui;

    private:
        void getUserOptions(QStringList *choosenOptions);
        void setupOsintProfiles();
        //...
        ResultsModel *m_resultsModel;
        //...
        osint::ScanArguments *scanArguments;
        osint::ScanResults *scanResults;
        //...
        int subdomainsCount = 0;
        //...
        QString currentPath;
        QString m_targetDomain;

    public:
        explicit Osint(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr);
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
        //...
        void on_pushButton_removeTargets_clicked();
        void on_pushButton_clearTargets_clicked();
        void on_pushButton_loadTargets_clicked();
        void on_pushButton_addTargets_clicked();
        void on_lineEdit_multipleTargets_returnPressed();
        //...
        void on_checkBox_useProfiles_clicked(bool checked);
        void on_pushButton_loadProfile_clicked();
        void on_pushButton_deleteProfile_clicked();
        void on_pushButton_newProfile_clicked();
        //...
        void onEnumerationComplete();
        void on_comboBox_target_currentIndexChanged(int index);
};

#endif // OSINT_H

#ifndef OSINT_H
#define OSINT_H

#include "src/utils.h"
#include "src/engines/Base.h"
#include "OsintScanner.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/ConfigDialog.h"


namespace Ui {
    class Osint;
}

class Osint : public BaseClass{
    Q_OBJECT

    public:
        Osint(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr);
        ~Osint();

    private slots:
        void on_buttonStart_clicked();
        void on_buttonClear_clicked();
        void on_buttonAction_clicked();
        void on_buttonStop_clicked();
        void on_buttonConfig_clicked();
        void on_buttonKeys_clicked();
        void on_lineEditTarget_returnPressed();
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);
        //...
        void on_checkBoxUseProfiles_clicked(bool checked);
        void on_buttonLoadProfile_clicked();
        void on_buttonDeleteProfile_clicked();
        void on_buttonNewProfile_clicked();
        //...
        void onEnumerationComplete();
        void on_checkBoxMultipleTargets_clicked(bool checked);

    private:
        Ui::Osint *ui;
        //...
        void getUserOptions(QStringList *choosenOptions);
        void setupOsintProfiles();
        //...
        osint::ScanArguments *scanArguments;
        osint::ScanResults *scanResults;
        //...
        int subdomainsCount = 0;
        //...
        QString currentPath;
        QString m_targetDomain;
};

#endif // OSINT_H

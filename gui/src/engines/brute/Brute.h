#ifndef BRUTE_H
#define BRUTE_H

#include "src/utils.h"
#include "src/engines/Base.h"
#include "src/engines/CommonScanners.h"
#include "BruteScanner.h"

namespace Ui {
    class Brute;
}

class Brute : public BaseClass{
    Q_OBJECT

    public:
        Brute(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr);
        ~Brute();

    public slots:
        void scanResult(QString subdomain, QString ipAddress, QString target);
        void scanThreadEnded();
        //...
        void choosenWordlist(QString);

    private slots:
        void on_buttonStart_clicked();
        void on_buttonPause_clicked();
        void on_buttonStop_clicked();
        void on_buttonAction_clicked();
        void on_buttonConfig_clicked();
        void on_buttonClearResults_clicked();
        void on_buttonWordlist_clicked();
        void on_lineEditTarget_returnPressed();
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);
        void on_checkBoxMultipleTargets_clicked(bool checked);

    private:
        Ui::Brute *ui;
        //...
        void startScan();
        QSet<QString> m_subdomainsSet;
        brute::ScanArguments *m_scanArguments;
};

#endif // BRUTE_H

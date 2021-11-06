#ifndef IPCHECKER_H
#define IPCHECKER_H

#include <QDialog>
#include "src/modules/osint/OsintModulesHeaders.h"

namespace Ui {
    class IpChecker;
}

class IpChecker : public QDialog{
    Q_OBJECT

    public:
        explicit IpChecker(QWidget *parent = nullptr);
        ~IpChecker();

    public slots:
        void onEnumerationComplete();
        void onErrorLog(QString log);
        void onInfoLog(QString log);

    private slots:
        void on_checkBoxExpand_clicked(bool checked);
        void on_buttonAnalyze_clicked();

    private:
        Ui::IpChecker *ui;
        IpModel *ipModel;
        ScanArgs *m_scanArgs = nullptr;
};

#endif // IPCHECKER_H

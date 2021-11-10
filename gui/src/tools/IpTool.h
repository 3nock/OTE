#ifndef IPTOOL_H
#define IPTOOL_H

#include <QDialog>
#include "src/modules/osint/OsintModulesHeaders.h"

namespace Ui {
    class IpTool;
}

class IpTool : public QDialog{
    Q_OBJECT

    public:
        explicit IpTool(QWidget *parent = nullptr);
        ~IpTool();

    public slots:
        void onEnumerationComplete();
        void onErrorLog(QString log);
        void onInfoLog(QString log);

    private slots:
        void on_checkBoxExpand_clicked(bool checked);
        void on_buttonAnalyze_clicked();

    private:
        Ui::IpTool *ui;
        IpModel *ipModel;
        ScanArgs *m_scanArgs = nullptr;
};

#endif // IPTOOL_H

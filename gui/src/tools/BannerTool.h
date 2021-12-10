#ifndef BANNERTOOL_H
#define BANNERTOOL_H

#include <QDialog>
#include "src/modules/scan/BannerScanner.h"

namespace Ui {
class BannerTool;
}

class BannerTool : public QDialog{
        Q_OBJECT

    public:
        explicit BannerTool(QWidget *parent = nullptr);
        ~BannerTool();

    private slots:
        void on_buttonStart_clicked();
        void on_comboBoxTargetType_currentIndexChanged(int index);

public slots:
        void onScanResultsBanner(QString);
        void onEnumerationComplete();
        //...
        void onErrorLog(QString log);
        void onInfoLog(QString log);

    private:
        Ui::BannerTool *ui;
        ScanConfig *scanConfig;
        //...
        void m_startScan();

};

#endif // BANNERTOOL_H

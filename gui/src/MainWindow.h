#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
//...
#include "src/utils/utils.h"
#include "src/project/Project.h"
#include "src/engines/ip/Ip.h"
#include "src/engines/dns/DnsRecords.h"
#include "src/engines/brute/Brute.h"
#include "src/engines/osint/Osint.h"
#include "src/engines/active/Active.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        //...
        ScanStatus *statusOsint;
        ScanStatus *statusBrute;
        ScanStatus *statusActive;
        ScanStatus *statusIp;
        ScanStatus *statusRecords;
        Status *status;
        //...
        ProxyModel *resultsProxyModel;
        ResultsModel *resultsModel;
        QSettings *config;

    public slots:
        void onReceiveLog(QString log);
        void onReceiveStatus(QString status);
        //...
        void onChangeTabToOsint();
        void onChangeTabToActive();
        void onChangeTabToBrute();
        void onChangeTabToIp();
        void onChangeTabToRecords();

    private slots:
        void on_actionAbout_triggered();
        void on_actionAboutQt_triggered();
        void on_actionExit_triggered();
        void on_actionRawOsint_triggered();

    private:
        Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

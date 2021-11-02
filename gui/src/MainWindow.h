#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
//...
#include "src/utils/utils.h"
#include "src/project/Project.h"
#include "src/engines/Ip.h"
#include "src/engines/DnsRecords.h"
#include "src/engines/Brute.h"
#include "src/engines/Osint.h"
#include "src/engines/Active.h"
#include "src/engines/Raw.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
        Q_OBJECT

    private:
        Ui::MainWindow *ui;

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        //...
        Status *status;
        ResultsModel *resultsModel;
        ProjectDataModel *projectDataModel;

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
        void on_actionBannerGrab_triggered();

    private:
        Ip *ip = nullptr;
        Osint *osint = nullptr;
        Brute *brute = nullptr;
        Active *active = nullptr;
        DnsRecords *records = nullptr;
        Project *project = nullptr;
        Raw *raw = nullptr;
};

#endif // MAINWINDOW_H

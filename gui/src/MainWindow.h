#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "src/core.h"
#include "src/project/Project.h"
#include "src/engines/ip/Ip.h"
#include "src/engines/dns/Dns.h"
#include "src/engines/save/Save.h"
#include "src/engines/level/Level.h"
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
        void checkForMissingFiles();
        ~MainWindow();
        //variables...
        QFile *logfile;

    public slots:
        void onReceiveLog(QString log);
        void onReceiveStatus(QString status);
        void changeTabToActive();
        void changeTabToSave();

    private:
        Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "core.h"
#include "src/LevelTab/Level.h"
#include "src/BruteTab/Brute.h"
#include "src/OsintTab/Osint.h"
#include "src/DnsTab/Dns.h"
#include "src/SaveTab/Save.h"

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

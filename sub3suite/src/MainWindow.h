#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "src/utils/utils.h"
#include "src/project/Project.h"
#include "src/engines/ip/Ip.h"
#include "src/engines/dns/Dns.h"
#include "src/engines/brute/Brute.h"
#include "src/engines/osint/Osint.h"
#include "src/engines/active/Active.h"
#include "src/engines/raw/Raw.h"
#include "src/engines/ssl/Ssl.h"


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
        /* ... */
        ProjectDataModel *projectDataModel;

    public slots:
        void onReceiveLog(QString log);
        void onReceiveStatus(QString status);
        /* ... */
        void onChangeTabToOsint();
        void onChangeTabToActive();
        void onChangeTabToBrute();
        void onChangeTabToIp();
        void onChangeTabToDns();
        void onChangeTabToRaw();
        void onChangeTabToCert();

    private slots:
        /* file menu */
        void on_actionExit_triggered();
        /* options menu */
        void on_actionApiKeys_triggered();
        void on_actionPreferences_triggered();
        /* help menu */
        void on_actionBlog_triggered();
        void on_actionAbout_triggered();
        void on_actionAboutQt_triggered();
        void on_actionlogViewer_triggered();
        /* tools menu */
        void on_actionNSTool_triggered();
        void on_actionMXTool_triggered();
        void on_actionIpTool_triggered();
        void on_actionASNTool_triggered();
        void on_actionCertTool_triggered();
        void on_actionEmailTool_triggered();
        void on_actionCIDRTool_triggered();
        void on_actionBannerTool_triggered();
        void on_actionDomainTool_triggered();

        void on_actionTwitter_triggered();

private:
        Ip *ip = nullptr;
        Osint *osint = nullptr;
        Brute *brute = nullptr;
        Active *active = nullptr;
        Dns *dns = nullptr;
        Project *project = nullptr;
        Raw *raw = nullptr;
        Ssl *ssl = nullptr;
        //...
        void m_connectSignals(AbstractEngine *engine);
};

#endif // MAINWINDOW_H

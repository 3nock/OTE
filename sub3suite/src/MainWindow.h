/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QLabel>

#include "src/utils/s3s.h"
#include "src/utils/utils.h"
#include "src/project/Project.h"
#include "src/utils/UpdateChecker.h"

/* tools */
#include "src/tools/dns/DNSTool.h"
#include "src/tools/brute/BruteTool.h"
#include "src/tools/osint/OsintTool.h"
#include "src/tools/host/HostTool.h"
#include "src/tools/raw/RawTool.h"
#include "src/tools/ssl/SSLTool.h"
#include "src/tools/url/URLTool.h"
#include "src/tools/ip/IPTool.h"

/* Enumerators */
#include "src/enums/ip/IPEnum.h"
#include "src/enums/asn/ASNEnum.h"
#include "src/enums/ssl/SSLEnum.h"
#include "src/enums/email/EmailEnum.h"
#include "src/enums/cidr/CIDREnum.h"
#include "src/enums/ns/NSEnum.h"
#include "src/enums/mx/MXEnum.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow{
        Q_OBJECT

    private:
        Ui::MainWindow *ui;

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override;

        void initProject(ProjectStruct project);

    public slots:
        void onReceiveStatus(QString status);

        /* tab change */
        void onChangeTabToOsint();
        void onChangeTabToHost();
        void onChangeTabToBrute();
        void onChangeTabToDns();
        void onChangeTabToRaw();
        void onChangeTabToSSL();
        void onChangeTabToURL();
        void onChangeTabToIP();
        void onChangeTabToIpEnum();
        void onChangeTabToAsnEnum();
        void onChangeTabToCidrEnum();
        void onChangeTabToNSEnum();
        void onChangeTabToMXEnum();
        void onChangeTabToSSLEnum();
        void onChangeTabToEmailEnum();

    private slots:
        /* documentation clickable label */
        void onGetDocumentation();

        /* file menu */
        void on_actionExit_triggered();

        /* options menu */
        void on_actionApiKeys_triggered();

        /* help menu */
        void on_actionBlog_triggered();
        void on_actionAbout_triggered();
        void on_actionAboutQt_triggered();
        void on_actionlogViewer_triggered();
        void on_actionTwitter_triggered();
        void on_actionDocumentation_triggered();
        void on_actionConfig_triggered();
        void on_actionOpenProject_triggered();
        void on_actionSourceCode_triggered();
        void on_actionDonate_triggered();
        void on_actionCheckUpdates_triggered();
        void on_actionSaveCopy_triggered();

    protected:
        void closeEvent(QCloseEvent *event) override;

    private:
        /* project */
        Project *project = nullptr;
        ProjectModel *projectModel = nullptr;

        /* tools */
        OsintTool *osintTool = nullptr;
        BruteTool *bruteTool = nullptr;
        HostTool *hostTool = nullptr;
        DNSTool *dnsTool = nullptr;
        RawTool *rawTool = nullptr;
        SSLTool *sslTool = nullptr;
        URLTool *urlTool = nullptr;
        IPTool *ipTool = nullptr;

        /* enumerators */
        IPEnum *ipEnum = nullptr;
        ASNEnum *asnEnum = nullptr;
        CIDREnum *cidrEnum = nullptr;
        NSEnum *nsEnum = nullptr;
        MXEnum *mxEnum = nullptr;
        SSLEnum *sslEnum = nullptr;
        EmailEnum *emailEnum = nullptr;

        UpdateChecker *m_updateChecker = nullptr;
        QMenu *m_menuRecents = nullptr;

        void initUI();
        void initActions();
        void initEngines();
        void connectSignals(AbstractTool *engine); // from Engines
        void connectSignals(AbstractEnum *enumerator); // from Enumerators
        void connectSignals(); // from project
        void setRecentProjects();
};

#endif // MAINWINDOW_H

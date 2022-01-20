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

#include "src/utils/utils.h"
#include "src/project/Project.h"

/* engines */
#include "src/engines/ip/Ip.h"
#include "src/engines/dns/Dns.h"
#include "src/engines/brute/Brute.h"
#include "src/engines/osint/Osint.h"
#include "src/engines/active/Active.h"
#include "src/engines/raw/Raw.h"
#include "src/engines/ssl/Ssl.h"

/* tools */
#include "src/tools/ip/IpTool.h"
#include "src/tools/asn/ASNTool.h"
#include "src/tools/ssl/SSLTool.h"
#include "src/tools/email/EmailTool.h"
#include "src/tools/cidr/CidrTool.h"
#include "src/tools/domain/DomainTool.h"
#include "src/tools/ns/NSTool.h"
#include "src/tools/mx/MXTool.h"
#include "src/tools/url/UrlTool.h"


class s3s_ClickableLabel : public QLabel{
       Q_OBJECT
    public:
       s3s_ClickableLabel(QString text, QWidget *parent = nullptr) : QLabel(text, parent) {}
       ~s3s_ClickableLabel(){}

    signals:
       void clicked();

    protected:
       void mousePressEvent(QMouseEvent *event) { Q_UNUSED(event); emit clicked(); }
};


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

    public slots:
        void onReceiveStatus(QString status);

        /* tab change */
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
        void on_actionTwitter_triggered();
        void on_actionDocumentation_triggered();

        /* documentations */
        void onDocumentation_active();
        void onDocumentation_passive();
        void onDocumentation_tools();

    private:
        /* project */
        Project *project = nullptr;
        ProjectModel *projectModel = nullptr;

        /* engines */
        Ip *ip = nullptr;
        Osint *osint = nullptr;
        Brute *brute = nullptr;
        Active *active = nullptr;
        Dns *dns = nullptr;
        Raw *raw = nullptr;
        Ssl *ssl = nullptr;

        /* tools */
        DomainTool *domainTool = nullptr;
        IpTool *ipTool = nullptr;
        ASNTool *asnTool = nullptr;
        CidrTool *cidrTool = nullptr;
        NSTool *nsTool = nullptr;
        MXTool *mxTool = nullptr;
        SSLTool *sslTool = nullptr;
        EmailTool *emailTool = nullptr;
        UrlTool *urlTool = nullptr;

        void m_initEngines();
        void m_documentation();
        void m_registerMetaTypes();
        void m_connectSignals(AbstractEngine *engine);
};

#endif // MAINWINDOW_H

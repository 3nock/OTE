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
#include "src/engines/dns/Dns.h"
#include "src/engines/brute/Brute.h"
#include "src/engines/osint/Osint.h"
#include "src/engines/active/Active.h"
#include "src/engines/raw/Raw.h"
#include "src/engines/ssl/Ssl.h"

/* tools */
#include "src/enums/ip/IpEnum.h"
#include "src/enums/asn/ASNEnum.h"
#include "src/enums/ssl/SSLEnum.h"
#include "src/enums/email/EmailEnum.h"
#include "src/enums/cidr/CidrEnum.h"
#include "src/enums/ns/NSEnum.h"
#include "src/enums/mx/MXEnum.h"
#include "src/enums/url/UrlEnum.h"


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

        void projectFile(QString projectFile);

    public slots:
        void onReceiveStatus(QString status);

        /* change to Engine */
        void onChangeTabToOsint();
        void onChangeTabToActive();
        void onChangeTabToBrute();
        void onChangeTabToDns();
        void onChangeTabToRaw();
        void onChangeTabToSSL();
        /* change tab To Enum */
        void onChangeTabToIpEnum();
        void onChangeTabToAsnEnum();
        void onChangeTabToCidrEnum();
        void onChangeTabToNSEnum();
        void onChangeTabToMXEnum();
        void onChangeTabToSSLEnum();
        void onChangeTabToEmailEnum();
        void onChangeTabToUrlEnum();

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
        QString m_projectFile;

        /* engines */
        Osint *osint = nullptr;
        Brute *brute = nullptr;
        Active *active = nullptr;
        Dns *dns = nullptr;
        Raw *raw = nullptr;
        Ssl *ssl = nullptr;

        /* tools */
        IpEnum *ipEnum = nullptr;
        ASNEnum *asnEnum = nullptr;
        CidrEnum *cidrEnum = nullptr;
        NSEnum *nsEnum = nullptr;
        MXEnum *mxEnum = nullptr;
        SSLEnum *sslEnum = nullptr;
        EmailEnum *emailEnum = nullptr;
        UrlEnum *urlEnum = nullptr;

        void m_initEngines();
        void m_documentation();
        void m_registerMetaTypes();
        void m_connectSignals(AbstractEngine *engine);
};

#endif // MAINWINDOW_H

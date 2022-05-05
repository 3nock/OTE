/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QScreen>
#include <QPushButton>
#include <QDesktopServices>
#include <QActionGroup>

#include "src/dialogs/AboutDialog.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/LogViewerDialog.h"
#include "src/dialogs/DocumentationDialog.h"
#include "src/dialogs/ProjectConfigDialog.h"
#include "src/dialogs/SaveProjectDialog.h"

#include "src/utils/Config.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->initUI();
    this->initEngines();
    this->initActions();
    this->setRecentProjects();

    /* check for updates */
    m_updateChecker = new UpdateChecker(this);
    m_updateChecker->checkForUpdates_onStart();
    ui->statusbar->showMessage("Checking For Updates...", 3000);
    this->setWindowIcon(QIcon(":/img/res/icons/main_logo.ico"));

#if defined (Q_OS_LINUX)
    this->setWindowIcon(QIcon(":/img/res/icons/main_logo.ico"));
#endif

#if defined(Q_OS_WIN)
    // Load Winsock
    WSADATA wsd;
    int rc;
    if ((rc = WSAStartup(MAKEWORD(2,2), &wsd)) != 0)
        qWarning() << "WSAStartup() failed: " << rc;
#endif // WINDOWS
}
MainWindow::~MainWindow(){
#if defined (Q_OS_WIN)
    WSACleanup();
#endif
    delete ui;
}

void MainWindow::initUI(){
    ui->setupUi(this);

    /* build info */
    QAction* buildInfo = new QAction(tr("(%1)").arg("BETA"), this);
    buildInfo->setEnabled(false);
    ui->menubar->addAction(buildInfo);

    /* donate */
    QWidget *cornerWidget_donate = new QWidget(this);
    QHBoxLayout *hbox_donate = new QHBoxLayout(cornerWidget_donate);
    s3s_ClickableLabel *donate = new s3s_ClickableLabel("", this);
    donate->setPixmap(QPixmap(":/img/res/icons/donate.png").scaled(15, 15, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    hbox_donate->addWidget(donate);
    hbox_donate->setContentsMargins(0, 0, 15, 2);
    ui->tabWidgetMain->setCornerWidget(cornerWidget_donate);
    connect(donate, &s3s_ClickableLabel::clicked, this, [=](){
        QDesktopServices::openUrl(QUrl("https://github.com/3nock/sub3suite/blob/main/SPONSOR.md", QUrl::TolerantMode));
    });

    /* documentation */
    QWidget *cornerWidget_active = new QWidget(this);
    QWidget *cornerWidget_passive = new QWidget(this);
    QWidget *cornerWidget_enums = new QWidget(this);

    QHBoxLayout *hbox_active = new QHBoxLayout(cornerWidget_active);
    QHBoxLayout *hbox_passive = new QHBoxLayout(cornerWidget_passive);
    QHBoxLayout *hbox_enums = new QHBoxLayout(cornerWidget_enums);

    s3s_ClickableLabel *documentation_active = new s3s_ClickableLabel("", this);
    s3s_ClickableLabel *documentation_passive = new s3s_ClickableLabel("", this);
    s3s_ClickableLabel *documentation_enums = new s3s_ClickableLabel("", this);

    documentation_active->setPixmap(QPixmap(":/img/res/icons/help.png"));
    documentation_passive->setPixmap(QPixmap(":/img/res/icons/help.png"));
    documentation_enums->setPixmap(QPixmap(":/img/res/icons/help.png"));

    hbox_active->addWidget(documentation_active);
    hbox_passive->addWidget(documentation_passive);
    hbox_enums->addWidget(documentation_enums);

    hbox_active->setContentsMargins(0, 0, 10, 2);
    hbox_passive->setContentsMargins(0, 0, 10, 2);
    hbox_enums->setContentsMargins(0, 0, 10, 2);

    ui->tabWidgetPassive->setCornerWidget(cornerWidget_passive);
    ui->tabWidgetActive->setCornerWidget(cornerWidget_active);
    ui->tabWidgetEnums->setCornerWidget(cornerWidget_enums);

    connect(documentation_passive, &s3s_ClickableLabel::clicked, this, &MainWindow::onGetDocumentation);
    connect(documentation_active, &s3s_ClickableLabel::clicked, this, &MainWindow::onGetDocumentation);
    connect(documentation_enums, &s3s_ClickableLabel::clicked, this, &MainWindow::onGetDocumentation);
}

void MainWindow::closeEvent(QCloseEvent *event){
    int retVal = QMessageBox::warning(nullptr, "Exit",
                               "Do you want to close Sub3 Suite?",
                                   QMessageBox::Close |
                                   QMessageBox::Cancel,
                                   QMessageBox::Close);
    if(retVal == QMessageBox::Cancel){
        event->ignore();
        return;
    }

    /* check if project is configured, if not prompt to save project */
    if(projectModel->info.isNew){
        if(!projectModel->info.isConfigured)
        {
            /* configure project */
            ProjectConfigDialog projectConfigDialog(projectModel, this);
            projectConfigDialog.setModal(true);
            projectConfigDialog.exec();
        }
        /* then save the project */
        projectModel->saveProject();
    }
    if(projectModel->info.isExisting){
        /* save project */
        projectModel->closeProject();
    }
}

void MainWindow::initActions(){
    ui->actionBlog->setIcon(QIcon(":/img/res/icons/website.png"));
    ui->actionExit->setIcon(QIcon(":/img/res/icons/exit.png"));
    ui->actionAbout->setIcon(QIcon(":/img/res/icons/about.png"));
    ui->actionConfig->setIcon(QIcon(":/img/res/icons/gear.png"));
    ui->actionDonate->setIcon(QIcon(":/img/res/icons/donate.png"));
    ui->actionAboutQt->setIcon(QIcon(":/img/res/icons/qt-logo.png"));
    ui->actionApiKeys->setIcon(QIcon(":/img/res/icons/key.png"));
    ui->actionTwitter->setIcon(QIcon(":/img/res/icons/twitter.png"));
    ui->actionlogViewer->setIcon(QIcon(":/img/res/icons/log.png"));
    ui->actionSourceCode->setIcon(QIcon(":/img/res/icons/source.png"));
    ui->actionOpenProject->setIcon(QIcon(":/img/res/icons/open.png"));
    ui->actionDocumentation->setIcon(QIcon(":/img/res/icons/doc.png"));
    ui->actionRecentProjects->setIcon(QIcon(":/img/res/icons/recent.png"));
    ui->actionCheckUpdates->setIcon(QIcon(":/img/res/icons/updates.png"));
    ui->actionSaveCopy->setIcon(QIcon(":/img/res/icons/save.png"));

    /* for tools */
    ui->menuTools->addAction("OSINT Tool", this, [=](){this->onChangeTabToOsint();})->setIcon(QIcon(":/img/res/icons/osint.png"));
    ui->menuTools->addAction("RAW Tool", this, [=](){this->onChangeTabToRaw();})->setIcon(QIcon(":/img/res/icons/raw.png"));
    ui->menuTools->addSeparator();
    ui->menuTools->addAction("BRUTE Tool", this, [=](){this->onChangeTabToBrute();})->setIcon(QIcon(":/img/res/icons/lock.png"));
    ui->menuTools->addAction("HOST Tool", this, [=](){this->onChangeTabToHost();})->setIcon(QIcon(":/img/res/icons/active.png"));
    ui->menuTools->addAction("IP Tool", this, [=](){this->onChangeTabToIP();})->setIcon(QIcon(":/img/res/icons/ip.png"));
    ui->menuTools->addAction("DNS Tool", this, [=](){this->onChangeTabToDns();})->setIcon(QIcon(":/img/res/icons/dns.png"));
    ui->menuTools->addAction("SSL Tool", this, [=](){this->onChangeTabToSSL();})->setIcon(QIcon(":/img/res/icons/ssl.png"));
    ui->menuTools->addAction("URL Tool", this, [=](){this->onChangeTabToURL();})->setIcon(QIcon(":/img/res/icons/url.png"));
    ui->menuTools->addSeparator();
    ui->menuTools->addAction("IP Enumerator", this, [=](){this->onChangeTabToIpEnum();})->setIcon(QIcon(":/img/res/icons/ip.png"));
    ui->menuTools->addAction("ASN Enumerator", this, [=](){this->onChangeTabToAsnEnum();})->setIcon(QIcon(":/img/res/icons/asn.png"));
    ui->menuTools->addAction("CIDR Enumerator", this, [=](){this->onChangeTabToCidrEnum();})->setIcon(QIcon(":/img/res/icons/cidr.png"));
    ui->menuTools->addAction("NS Enumerator", this, [=](){this->onChangeTabToNSEnum();})->setIcon(QIcon(":/img/res/icons/ns.png"));
    ui->menuTools->addAction("MX Enumerator", this, [=](){this->onChangeTabToMXEnum();})->setIcon(QIcon(":/img/res/icons/mx.png"));
    ui->menuTools->addAction("SSL Enumerator", this, [=](){this->onChangeTabToSSLEnum();})->setIcon(QIcon(":/img/res/icons/ssl.png"));
    ui->menuTools->addAction("Email Enumerator", this, [=](){this->onChangeTabToEmailEnum();})->setIcon(QIcon(":/img/res/icons/email.png"));

    ///
    /// theme actions...
    ///
    QAction *light_theme = new QAction(this);
    QAction *dark_theme = new QAction(this);
    light_theme->setText(tr("Light"));
    dark_theme->setText(tr("Dark"));
    light_theme->setCheckable(true);
    dark_theme->setCheckable(true);

    connect(light_theme, &QAction::triggered, this, [=](){
        CONFIG.setValue(CFG_VAL_THEME, "light");
        QMessageBox::information(this, "Info", "The theme will be applied after re-start");
    });
    connect(dark_theme, &QAction::triggered, this, [=](){
        CONFIG.setValue(CFG_VAL_THEME, "dark");
        QMessageBox::information(this, "Info", "The theme will be applied after re-start");
    });

    QActionGroup *theme_group = new QActionGroup(this);
    theme_group->addAction(light_theme);
    theme_group->addAction(dark_theme);
    theme_group->setExclusive(true);

    QMenu *themes = new QMenu(this);
    themes->addAction(light_theme);
    themes->addAction(dark_theme);

    if(s3s_global::is_dark_theme)
        dark_theme->setChecked(true);
    if(s3s_global::is_light_theme)
        light_theme->setChecked(true);

    ui->actionTheme->setMenu(themes);

    ///
    /// theme actions...
    ///
    QAction *font_11 = new QAction(this);
    QAction *font_12 = new QAction(this);
    QAction *font_none = new QAction(this);
    font_11->setText(tr("11"));
    font_12->setText(tr("12"));
    font_none->setText(tr("none"));
    font_11->setCheckable(true);
    font_12->setCheckable(true);
    font_none->setCheckable(true);

    connect(font_11, &QAction::triggered, this, [=](){
        CONFIG.setValue(CFG_VAL_FONT, 11);
        QMessageBox::information(this, "Info", "The font size will be applied after re-start");
    });
    connect(font_12, &QAction::triggered, this, [=](){
        CONFIG.setValue(CFG_VAL_FONT, 12);
        QMessageBox::information(this, "Info", "The font size will be applied after re-start");
    });
    connect(font_none, &QAction::triggered, this, [=](){
        CONFIG.setValue(CFG_VAL_FONT, 0);
        QMessageBox::information(this, "Info", "The font size will be applied after re-start");
    });

    QActionGroup *font_group = new QActionGroup(this);
    font_group->addAction(font_11);
    font_group->addAction(font_12);
    font_group->addAction(font_none);
    font_group->setExclusive(true);

    QMenu *fonts = new QMenu(this);
    fonts->addAction(font_11);
    fonts->addAction(font_12);
    fonts->addAction(font_none);

    if(s3s_global::font_size == 11)
        font_11->setChecked(true);
    if(s3s_global::font_size == 12)
        font_12->setChecked(true);
    if(s3s_global::font_size == 0)
        font_none->setChecked(true);

    ui->actionFont->setMenu(fonts);
}

void MainWindow::setRecentProjects(){
    m_menuRecents = new QMenu(this);

    CONFIG.beginGroup(CFG_GRP_RECENT);
    QStringList recentList =  CONFIG.allKeys();

    foreach(const QString &project_name, CONFIG.allKeys()){
        QString project_path = CONFIG.value(project_name).toString();
        m_menuRecents->addAction(project_name, this, [=](){projectModel->openExistingProject(project_name, project_path);});
    }

    /* set the menu */
    ui->actionRecentProjects->setMenu(m_menuRecents);
    CONFIG.endGroup();
}

void MainWindow::initProject(ProjectStruct project_info){
    ui->statusbar->showMessage("Opening Project \""+project_info.name+"\"", 3000);
    projectModel->openProject(project_info);
}

void MainWindow::initEngines(){
    /* Project */
    projectModel = new ProjectModel(this);
    project = new Project(this, projectModel);

    connect(projectModel, &ProjectModel::projectLoaded, project, &Project::onProjectLoaded);

    /* Engines */
    osintTool = new OsintTool(this, projectModel);
    bruteTool = new BruteTool(this, projectModel);
    hostTool = new HostTool(this, projectModel);
    dnsTool = new DNSTool(this, projectModel);
    sslTool = new SSLTool(this, projectModel);
    rawTool = new RawTool(this, projectModel);
    urlTool = new URLTool(this, projectModel);
    ipTool = new IPTool(this, projectModel);

    /* Enumerators */
    ipEnum = new IPEnum(this, projectModel);
    asnEnum = new ASNEnum(this, projectModel);
    cidrEnum = new CIDREnum(this, projectModel);
    nsEnum = new NSEnum(this, projectModel);
    mxEnum = new MXEnum(this, projectModel);
    sslEnum = new SSLEnum(this, projectModel);
    emailEnum = new EmailEnum(this, projectModel);

    /* connecting signals from engines */
    this->connectSignals(osintTool);
    this->connectSignals(bruteTool);
    this->connectSignals(hostTool);
    this->connectSignals(dnsTool);
    this->connectSignals(sslTool);
    this->connectSignals(rawTool);
    this->connectSignals(urlTool);
    this->connectSignals(ipTool);

    /* connecting signals from enumerators */
    this->connectSignals(ipEnum);
    this->connectSignals(asnEnum);
    this->connectSignals(cidrEnum);
    this->connectSignals(nsEnum);
    this->connectSignals(mxEnum);
    this->connectSignals(sslEnum);
    this->connectSignals(emailEnum);

    /* connecting signals from project */
    this->connectSignals();

    /* passive tabwidget */
    ui->tabWidgetPassive->insertTab(0, osintTool, "OSINT");
    ui->tabWidgetPassive->insertTab(1, rawTool, "RAW");
    ui->tabWidgetPassive->setCurrentIndex(0);
    /* active tabwidget */
    ui->tabWidgetActive->insertTab(0, bruteTool, "BRUTE");
    ui->tabWidgetActive->insertTab(1, hostTool, "HOST");
    ui->tabWidgetActive->insertTab(2, ipTool, "IP");
    ui->tabWidgetActive->insertTab(3, dnsTool, "DNS");
    ui->tabWidgetActive->insertTab(4, sslTool, "SSL");
    ui->tabWidgetActive->insertTab(5, urlTool, "URL");
    ui->tabWidgetActive->setCurrentIndex(0);
    /* tools tabwidget */
    ui->tabWidgetEnums->insertTab(0, ipEnum, "IP");
    ui->tabWidgetEnums->insertTab(1, asnEnum, "ASN");
    ui->tabWidgetEnums->insertTab(2, cidrEnum, "CIDR");
    ui->tabWidgetEnums->insertTab(3, nsEnum, "NS");
    ui->tabWidgetEnums->insertTab(4, mxEnum, "MX");
    ui->tabWidgetEnums->insertTab(5, sslEnum, "SSL");
    ui->tabWidgetEnums->insertTab(6, emailEnum, "EMAIL");
    /* main tabwidget */
    ui->tabWidgetMain->insertTab(3, project, "Project");
    ui->tabWidgetMain->setCurrentIndex(0);
}

void MainWindow::connectSignals(AbstractTool *engine){
    /* sending results */
    connect(engine, SIGNAL(sendToOsint(QSet<QString>, RESULT_TYPE)), osintTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(engine, SIGNAL(sendToHost(QSet<QString>, RESULT_TYPE)), hostTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(engine, SIGNAL(sendToBrute(QSet<QString>, RESULT_TYPE)), bruteTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(engine, SIGNAL(sendToDns(QSet<QString>, RESULT_TYPE)), dnsTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(engine, SIGNAL(sendToSsl(QSet<QString>, RESULT_TYPE)), sslTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(engine, SIGNAL(sendToRaw(QSet<QString>, RESULT_TYPE)), rawTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(engine, SIGNAL(sendToUrl(QSet<QString>, RESULT_TYPE)), urlTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(engine, SIGNAL(sendToIP(QSet<QString>, RESULT_TYPE)), ipTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    /* sending results enum */
    connect(engine, SIGNAL(sendToAsnEnum(QSet<QString>, RESULT_TYPE)), asnEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(engine, SIGNAL(sendToCidrEnum(QSet<QString>, RESULT_TYPE)), cidrEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(engine, SIGNAL(sendToIpEnum(QSet<QString>, RESULT_TYPE)), ipEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(engine, SIGNAL(sendToNSEnum(QSet<QString>, RESULT_TYPE)), nsEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(engine, SIGNAL(sendToMXEnum(QSet<QString>, RESULT_TYPE)), mxEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(engine, SIGNAL(sendToEmailEnum(QSet<QString>, RESULT_TYPE)), emailEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(engine, SIGNAL(sendToSSLEnum(QSet<QString>, RESULT_TYPE)), sslEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));

    /* change tab to Engine */
    connect(engine, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(engine, SIGNAL(changeTabToHost()), this, SLOT(onChangeTabToHost()));
    connect(engine, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(engine, SIGNAL(changeTabToDns()), this, SLOT(onChangeTabToDns()));
    connect(engine, SIGNAL(changeTabToRaw()), this, SLOT(onChangeTabToRaw()));
    connect(engine, SIGNAL(changeTabToSSL()), this, SLOT(onChangeTabToSSL()));
    connect(engine, SIGNAL(changeTabToURL()), this, SLOT(onChangeTabToURL()));
    connect(engine, SIGNAL(changeTabToIP()), this, SLOT(onChangeTabToIP()));
    /* change tab to Enumerator */
    connect(engine, SIGNAL(changeTabToIpEnum()), this, SLOT(onChangeTabToIpEnum()));
    connect(engine, SIGNAL(changeTabToAsnEnum()), this, SLOT(onChangeTabToAsnEnum()));
    connect(engine, SIGNAL(changeTabToCidrEnum()), this, SLOT(onChangeTabToCidrEnum()));
    connect(engine, SIGNAL(changeTabToNSEnum()), this, SLOT(onChangeTabToNSEnum()));
    connect(engine, SIGNAL(changeTabToMXEnum()), this, SLOT(onChangeTabToMXEnum()));
    connect(engine, SIGNAL(changeTabToSSLEnum()), this, SLOT(onChangeTabToSSLEnum()));
    connect(engine, SIGNAL(changeTabToEmailEnum()), this, SLOT(onChangeTabToEmailEnum()));
}

void MainWindow::connectSignals(AbstractEnum *enumerator){
    /* sending results to engine */
    connect(enumerator, SIGNAL(sendToOsint(QSet<QString>, RESULT_TYPE)), osintTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(enumerator, SIGNAL(sendToHost(QSet<QString>, RESULT_TYPE)), hostTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(enumerator, SIGNAL(sendToBrute(QSet<QString>, RESULT_TYPE)), bruteTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(enumerator, SIGNAL(sendToDns(QSet<QString>, RESULT_TYPE)), dnsTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(enumerator, SIGNAL(sendToSsl(QSet<QString>, RESULT_TYPE)), sslTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(enumerator, SIGNAL(sendToRaw(QSet<QString>, RESULT_TYPE)), rawTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(enumerator, SIGNAL(sendToUrl(QSet<QString>, RESULT_TYPE)), urlTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(enumerator, SIGNAL(sendToIP(QSet<QString>, RESULT_TYPE)), ipTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    /* sending results enum */
    connect(enumerator, SIGNAL(sendToAsnEnum(QSet<QString>, RESULT_TYPE)), asnEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(enumerator, SIGNAL(sendToCidrEnum(QSet<QString>, RESULT_TYPE)), cidrEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(enumerator, SIGNAL(sendToIpEnum(QSet<QString>, RESULT_TYPE)), ipEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(enumerator, SIGNAL(sendToNSEnum(QSet<QString>, RESULT_TYPE)), nsEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(enumerator, SIGNAL(sendToMXEnum(QSet<QString>, RESULT_TYPE)), mxEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(enumerator, SIGNAL(sendToEmailEnum(QSet<QString>, RESULT_TYPE)), emailEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(enumerator, SIGNAL(sendToSSLEnum(QSet<QString>, RESULT_TYPE)), sslEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));

    /* change tab to Engine */
    connect(enumerator, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(enumerator, SIGNAL(changeTabToHost()), this, SLOT(onChangeTabToHost()));
    connect(enumerator, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(enumerator, SIGNAL(changeTabToDns()), this, SLOT(onChangeTabToDns()));
    connect(enumerator, SIGNAL(changeTabToRaw()), this, SLOT(onChangeTabToRaw()));
    connect(enumerator, SIGNAL(changeTabToSSL()), this, SLOT(onChangeTabToSSL()));
    connect(enumerator, SIGNAL(changeTabToURL()), this, SLOT(onChangeTabToURL()));
    connect(enumerator, SIGNAL(changeTabToIP()), this, SLOT(onChangeTabToIP()));
    /* change tab to Enumerator */
    connect(enumerator, SIGNAL(changeTabToIpEnum()), this, SLOT(onChangeTabToIpEnum()));
    connect(enumerator, SIGNAL(changeTabToAsnEnum()), this, SLOT(onChangeTabToAsnEnum()));
    connect(enumerator, SIGNAL(changeTabToCidrEnum()), this, SLOT(onChangeTabToCidrEnum()));
    connect(enumerator, SIGNAL(changeTabToNSEnum()), this, SLOT(onChangeTabToNSEnum()));
    connect(enumerator, SIGNAL(changeTabToMXEnum()), this, SLOT(onChangeTabToMXEnum()));
    connect(enumerator, SIGNAL(changeTabToSSLEnum()), this, SLOT(onChangeTabToSSLEnum()));
    connect(enumerator, SIGNAL(changeTabToEmailEnum()), this, SLOT(onChangeTabToEmailEnum()));
}

void MainWindow::connectSignals(){
    /* sending to engine */
    connect(project, SIGNAL(sendToOsint(QSet<QString>, RESULT_TYPE)), osintTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(project, SIGNAL(sendToHost(QSet<QString>, RESULT_TYPE)), hostTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(project, SIGNAL(sendToBrute(QSet<QString>, RESULT_TYPE)), bruteTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(project, SIGNAL(sendToDns(QSet<QString>, RESULT_TYPE)), dnsTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(project, SIGNAL(sendToSsl(QSet<QString>, RESULT_TYPE)), sslTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(project, SIGNAL(sendToRaw(QSet<QString>, RESULT_TYPE)), rawTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(project, SIGNAL(sendToUrl(QSet<QString>, RESULT_TYPE)), urlTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(project, SIGNAL(sendToIP(QSet<QString>, RESULT_TYPE)), ipTool, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    /* sending enum */
    connect(project, SIGNAL(sendToAsnEnum(QSet<QString>, RESULT_TYPE)), asnEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(project, SIGNAL(sendToCidrEnum(QSet<QString>, RESULT_TYPE)), cidrEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(project, SIGNAL(sendToIpEnum(QSet<QString>, RESULT_TYPE)), ipEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(project, SIGNAL(sendToNSEnum(QSet<QString>, RESULT_TYPE)), nsEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(project, SIGNAL(sendToMXEnum(QSet<QString>, RESULT_TYPE)), mxEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(project, SIGNAL(sendToEmailEnum(QSet<QString>, RESULT_TYPE)), emailEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));
    connect(project, SIGNAL(sendToSSLEnum(QSet<QString>, RESULT_TYPE)), sslEnum, SLOT(onReceiveTargets(QSet<QString>, RESULT_TYPE)));

    /* change tab to Engine */
    connect(project, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(project, SIGNAL(changeTabToHost()), this, SLOT(onChangeTabToHost()));
    connect(project, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(project, SIGNAL(changeTabToDns()), this, SLOT(onChangeTabToDns()));
    connect(project, SIGNAL(changeTabToRaw()), this, SLOT(onChangeTabToRaw()));
    connect(project, SIGNAL(changeTabToSSL()), this, SLOT(onChangeTabToSSL()));
    connect(project, SIGNAL(changeTabToURL()), this, SLOT(onChangeTabToURL()));
    connect(project, SIGNAL(changeTabToIP()), this, SLOT(onChangeTabToIP()));
    /* change tab to Enumerator */
    connect(project, SIGNAL(changeTabToIpEnum()), this, SLOT(onChangeTabToIpEnum()));
    connect(project, SIGNAL(changeTabToAsnEnum()), this, SLOT(onChangeTabToAsnEnum()));
    connect(project, SIGNAL(changeTabToCidrEnum()), this, SLOT(onChangeTabToCidrEnum()));
    connect(project, SIGNAL(changeTabToNSEnum()), this, SLOT(onChangeTabToNSEnum()));
    connect(project, SIGNAL(changeTabToMXEnum()), this, SLOT(onChangeTabToMXEnum()));
    connect(project, SIGNAL(changeTabToSSLEnum()), this, SLOT(onChangeTabToSSLEnum()));
    connect(project, SIGNAL(changeTabToEmailEnum()), this, SLOT(onChangeTabToEmailEnum()));
}

void MainWindow::onGetDocumentation(){
    DocumentationDialog *documentationDialog = nullptr;

    switch (ui->tabWidgetMain->currentIndex())
    {
    case 0: // Passive
        switch (ui->tabWidgetPassive->currentIndex()) {
        case 0:
            documentationDialog = new DocumentationDialog(TOOL::OSINT, this);
            break;
        case 1:
            documentationDialog = new DocumentationDialog(TOOL::RAW, this);
            break;
        }
        break;

    case 1: // Active
        switch (ui->tabWidgetActive->currentIndex()) {
        case 0:
            documentationDialog = new DocumentationDialog(TOOL::BRUTE, this);
            break;
        case 1:
            documentationDialog = new DocumentationDialog(TOOL::HOST, this);
            break;
        case 2:
            documentationDialog = new DocumentationDialog(TOOL::IP, this);
            break;
        case 3:
            documentationDialog = new DocumentationDialog(TOOL::DNS, this);
            break;
        case 4:
            documentationDialog = new DocumentationDialog(TOOL::SSL, this);
            break;
        case 5:
            documentationDialog = new DocumentationDialog(TOOL::URL, this);
            break;
        }
        break;

    case 2: // Enum
        switch (ui->tabWidgetEnums->currentIndex()) {
        case 0:
            documentationDialog = new DocumentationDialog(ENUMERATOR::IP, this);
            break;
        case 1:
            documentationDialog = new DocumentationDialog(ENUMERATOR::ASN, this);
            break;
        case 2:
            documentationDialog = new DocumentationDialog(ENUMERATOR::CIDR, this);
            break;
        case 3:
            documentationDialog = new DocumentationDialog(ENUMERATOR::NS, this);
            break;
        case 4:
            documentationDialog = new DocumentationDialog(ENUMERATOR::MX, this);
            break;
        case 5:
            documentationDialog = new DocumentationDialog(ENUMERATOR::SSL, this);
            break;
        case 6:
            documentationDialog = new DocumentationDialog(ENUMERATOR::EMAIL, this);
            break;
        }
    }

    documentationDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    documentationDialog->show();
}

///
/// Public slots...
///

void MainWindow::onReceiveStatus(QString status){
    ui->statusbar->showMessage(status, 3000);
}

void MainWindow::onChangeTabToOsint(){
    ui->tabWidgetMain->setCurrentIndex(0);
    ui->tabWidgetPassive->setCurrentIndex(0);
}

void MainWindow::onChangeTabToRaw(){
    ui->tabWidgetMain->setCurrentIndex(0);
    ui->tabWidgetPassive->setCurrentIndex(1);
}

void MainWindow::onChangeTabToBrute(){
    ui->tabWidgetMain->setCurrentIndex(1);
    ui->tabWidgetActive->setCurrentIndex(0);
}

void MainWindow::onChangeTabToHost(){
    ui->tabWidgetMain->setCurrentIndex(1);
    ui->tabWidgetActive->setCurrentIndex(1);
}

void MainWindow::onChangeTabToIP(){
    ui->tabWidgetMain->setCurrentIndex(1);
    ui->tabWidgetActive->setCurrentIndex(2);
}

void MainWindow::onChangeTabToDns(){
    ui->tabWidgetMain->setCurrentIndex(1);
    ui->tabWidgetActive->setCurrentIndex(3);
}

void MainWindow::onChangeTabToSSL(){
    ui->tabWidgetMain->setCurrentIndex(1);
    ui->tabWidgetActive->setCurrentIndex(4);
}

void MainWindow::onChangeTabToURL(){
    ui->tabWidgetMain->setCurrentIndex(1);
    ui->tabWidgetActive->setCurrentIndex(5);
}

void MainWindow::onChangeTabToIpEnum(){
    ui->tabWidgetMain->setCurrentIndex(2);
    ui->tabWidgetEnums->setCurrentIndex(0);
}

void MainWindow::onChangeTabToAsnEnum(){
    ui->tabWidgetMain->setCurrentIndex(2);
    ui->tabWidgetEnums->setCurrentIndex(1);
}

void MainWindow::onChangeTabToCidrEnum(){
    ui->tabWidgetMain->setCurrentIndex(2);
    ui->tabWidgetEnums->setCurrentIndex(2);
}

void MainWindow::onChangeTabToNSEnum(){
    ui->tabWidgetMain->setCurrentIndex(2);
    ui->tabWidgetEnums->setCurrentIndex(3);
}

void MainWindow::onChangeTabToMXEnum(){
    ui->tabWidgetMain->setCurrentIndex(2);
    ui->tabWidgetEnums->setCurrentIndex(4);
}

void MainWindow::onChangeTabToSSLEnum(){
    ui->tabWidgetMain->setCurrentIndex(2);
    ui->tabWidgetEnums->setCurrentIndex(5);
}

void MainWindow::onChangeTabToEmailEnum(){
    ui->tabWidgetMain->setCurrentIndex(2);
    ui->tabWidgetEnums->setCurrentIndex(6);
}

///
/// Actions...
///

void MainWindow::on_actionAbout_triggered(){
    AboutDialog *aboutDialog = new AboutDialog(this);
    aboutDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    aboutDialog->show();
}

void MainWindow::on_actionAboutQt_triggered(){
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_actionExit_triggered(){
    QApplication::exit();
}

void MainWindow::on_actionBlog_triggered(){
    QDesktopServices::openUrl(QUrl("https://3nock.github.io", QUrl::TolerantMode));
}

void MainWindow::on_actionTwitter_triggered(){
    QDesktopServices::openUrl(QUrl("https://twitter.com/sub3suite", QUrl::TolerantMode));
}

void MainWindow::on_actionApiKeys_triggered(){
    ApiKeysDialog *apiKeys = new ApiKeysDialog(this);
    apiKeys->setAttribute(Qt::WA_DeleteOnClose, true);
    apiKeys->show();
}

void MainWindow::on_actionlogViewer_triggered(){
    LogViewerDialog *logViewerDialog = new LogViewerDialog(this);
    logViewerDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    logViewerDialog->show();
}

void MainWindow::on_actionDocumentation_triggered(){
    DocumentationDialog *documentationDialog = nullptr;
    documentationDialog = new DocumentationDialog(this);
    documentationDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    documentationDialog->show();
}

void MainWindow::on_actionConfig_triggered(){
    ProjectConfigDialog projectConfigDialog(projectModel, this);
    projectConfigDialog.setModal(true);
    projectConfigDialog.exec();
}

void MainWindow::on_actionOpenProject_triggered(){
    QString filename = QFileDialog::getOpenFileName(this, "Open", "./", "*.s3s");
    if(filename.isEmpty()){
        return;
    }

    QFile file(filename);
    if(file.exists()){
        QFileInfo fileInfo(file);
        QString name = fileInfo.fileName().remove(".s3s");

        /* opening the project */
        projectModel->openExistingProject(name, filename);
    }
}

void MainWindow::on_actionSourceCode_triggered(){
    QDesktopServices::openUrl(QUrl("https://github.com/3nock/sub3suite", QUrl::TolerantMode));
}

void MainWindow::on_actionDonate_triggered(){
    QDesktopServices::openUrl(QUrl("https://github.com/3nock/sub3suite/blob/main/SPONSOR.md", QUrl::TolerantMode));
}

void MainWindow::on_actionCheckUpdates_triggered(){
    m_updateChecker->checkForUpdates();
    ui->statusbar->showMessage("Checking For Updates...", 3000);
}

void MainWindow::on_actionSaveCopy_triggered(){
    SaveProjectDialog *saveProjectDialog = new SaveProjectDialog(projectModel, this);
    saveProjectDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    saveProjectDialog->show();
}

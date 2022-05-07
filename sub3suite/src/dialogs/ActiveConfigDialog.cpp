/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "ActiveConfigDialog.h"
#include "ui_ActiveConfigDialog.h"

#include "src/utils/Config.h"


/* for brute... */
ActiveConfigDialog::ActiveConfigDialog(QWidget *parent, brute::ScanConfig *config) :
    QDialog(parent),
    ui(new Ui::ActiveConfigDialog),
    m_configBrute(config),
    m_customNameserverListModel(new QStringListModel)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    brute = true;
    this->m_initWidgets();
    this->m_loadConfigBrute();

    /* hiding unused widgets */
    ui->groupBoxScheme->hide();
    ui->checkBoxRedirects->hide();
    ui->checkBoxScreenshot->hide();
    ui->checkBoxTitle->hide();
    ui->groupBoxPingScan->hide();
    ui->groupBoxPortScan->hide();

    this->adjustSize();
}

/* for host... */
ActiveConfigDialog::ActiveConfigDialog(QWidget *parent, host::ScanConfig *config) :
    QDialog(parent),
    ui(new Ui::ActiveConfigDialog),
    m_configHost(config),
    m_customNameserverListModel(new QStringListModel)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    host = true;
    this->m_initWidgets();
    this->m_loadConfigHost();

    /* hiding unused widgets */
    ui->checkBoxWildcards->hide();
    ui->groupBoxScheme->hide();
    ui->checkBoxRedirects->hide();
    ui->checkBoxScreenshot->hide();
    ui->checkBoxTitle->hide();
    ui->groupBoxPortScan->hide();
#if defined (Q_OS_UNIX)
    ui->groupBoxPingScan->hide();
#endif

    this->adjustSize();
}

/* for ip... */
ActiveConfigDialog::ActiveConfigDialog(QWidget *parent, reverseip::ScanConfig *config) :
    QDialog(parent),
    ui(new Ui::ActiveConfigDialog),
    m_configIP(config),
    m_customNameserverListModel(new QStringListModel)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    ip = true;
    this->m_initWidgets();
    this->m_loadConfigIP();

    /* hiding unused widgets */
    ui->checkBoxWildcards->hide();
    ui->groupBoxScheme->hide();
    ui->checkBoxRedirects->hide();
    ui->checkBoxScreenshot->hide();
    ui->checkBoxTitle->hide();
    ui->radioButtonA->hide();
    ui->radioButtonAAAA->hide();
    ui->radioButtonANY->hide();
    ui->labelRecordType->hide();
    ui->tabWidget->removeTab(1);
    ui->groupBoxPortScan->hide();
#if defined (Q_OS_UNIX)
    ui->groupBoxPingScan->hide();
#endif

    this->adjustSize();
}

/* for DNS... */
ActiveConfigDialog::ActiveConfigDialog(QWidget *parent, dns::ScanConfig *config) :
    QDialog(parent),
    ui(new Ui::ActiveConfigDialog),
    m_configDns(config),
    m_customNameserverListModel(new QStringListModel)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    dns = true;
    this->m_initWidgets();
    this->m_loadConfigDns();

    /* hiding unused widgets */
    ui->groupBoxScheme->hide();
    ui->checkBoxWildcards->hide();
    ui->radioButtonA->hide();
    ui->radioButtonAAAA->hide();
    ui->radioButtonANY->hide();
    ui->labelRecordType->hide();
    ui->checkBoxRedirects->hide();
    ui->checkBoxScreenshot->hide();
    ui->checkBoxTitle->hide();
    ui->groupBoxPingScan->hide();
    ui->groupBoxPortScan->hide();

    this->adjustSize();
}

/* for SSL... */
ActiveConfigDialog::ActiveConfigDialog(QWidget *parent, ssl::ScanConfig *config) :
    QDialog(parent),
    ui(new Ui::ActiveConfigDialog),
    m_configSSL(config),
    m_customNameserverListModel(new QStringListModel)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    ssl = true;
    this->m_initWidgets();
    this->m_loadConfigSSL();

    ui->groupBoxTimeout->setCheckable(false);

    /* hiding unused widgets */
    ui->groupBoxScheme->hide();
    ui->checkBoxWildcards->hide();
    ui->radioButtonA->hide();
    ui->radioButtonAAAA->hide();
    ui->radioButtonANY->hide();
    ui->labelRecordType->hide();
    ui->tabWidget->removeTab(1);
    ui->checkBoxRedirects->hide();
    ui->checkBoxScreenshot->hide();
    ui->checkBoxTitle->hide();
    ui->groupBoxPingScan->hide();
    ui->groupBoxPortScan->hide();

    this->adjustSize();
}

/* for URL... */
ActiveConfigDialog::ActiveConfigDialog(QWidget *parent, url::ScanConfig *config) :
    QDialog(parent),
    ui(new Ui::ActiveConfigDialog),
    m_configURL(config),
    m_customNameserverListModel(new QStringListModel)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    url = true;
    this->m_initWidgets();
    this->m_loadConfigURL();

    /* hiding unused widgets */
    ui->checkBoxWildcards->hide();
    ui->radioButtonA->hide();
    ui->radioButtonAAAA->hide();
    ui->radioButtonANY->hide();
    ui->labelRecordType->hide();
    ui->tabWidget->removeTab(1);
    ui->groupBoxPingScan->hide();
    ui->groupBoxPortScan->hide();
    ui->checkBoxScreenshot->hide();

    this->adjustSize();
}

ActiveConfigDialog::~ActiveConfigDialog(){
    delete m_customNameserverListModel;
    delete ui;
}

void ActiveConfigDialog::on_buttonCancel_clicked(){
    accept();
}

void ActiveConfigDialog::on_buttonOk_clicked(){
    if(brute)
        this->m_saveBrute();
    if(host)
        this->m_saveHost();
    if(dns)
        this->m_saveDns();
    if(ssl)
        this->m_saveSSL();
    if(url)
        this->m_saveURL();
    if(ip)
        this->m_saveIP();

    accept();
}

void ActiveConfigDialog::m_initWidgets(){
    /* setting placeholdertxts... */
    ui->lineEditTimeout->setPlaceholderText("e.g. 3");
    ui->lineEditThreads->setPlaceholderText("e.g. 100");

    /* custom-nameserver list */
    ui->customNameservers->setListName("Namerserver");
    ui->customNameservers->setListModel(m_customNameserverListModel);
}

///
/// get config settings from config file...
///

void ActiveConfigDialog::m_loadConfigBrute(){
    CONFIG.beginGroup(CFG_BRUTE);
    ui->lineEditThreads->setText(CONFIG.value(CFG_VAL_THREADS).toString());
    ui->lineEditTimeout->setText(CONFIG.value(CFG_VAL_TIMEOUT).toString());
    ui->checkBoxWildcards->setChecked(CONFIG.value(CFG_VAL_WILDCARD).toBool());
    ui->checkBoxAutosave->setChecked(CONFIG.value(CFG_VAL_AUTOSAVE).toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value(CFG_VAL_DUPLICATES).toBool());
    ui->groupBoxTimeout->setChecked(CONFIG.value(CFG_VAL_SETTIMEOUT).toBool());
    QString record = CONFIG.value(CFG_VAL_RECORD).toString();
    QString nsType = CONFIG.value(CFG_VAL_NAMESERVER).toString();
    CONFIG.endGroup();

    if(record == "A")
        ui->radioButtonA->setChecked(true);
    if(record == "AAAA")
        ui->radioButtonAAAA->setChecked(true);
    if(record == "ANY")
        ui->radioButtonANY->setChecked(true);

    if(nsType == "single")
        ui->radioButtonSingleNameserver->setChecked(true);
    if(nsType == "random")
        ui->radioButtonRandomNameservers->setChecked(true);
    if(nsType == "custom")
        ui->radioButtonCustomNameservers->setChecked(true);

    CONFIG.beginGroup(CFG_GRP_DEFAULT_NS);
    ui->comboBoxSingleNameserver->addItems(CONFIG.allKeys());
    CONFIG.endGroup();

    int size = CONFIG.beginReadArray("custom_nameservers_brute");
    for (int i = 0; i < size; ++i) {
        CONFIG.setArrayIndex(i);
        ui->customNameservers->add(CONFIG.value("value").toString());
    }
    CONFIG.endArray();
}

void ActiveConfigDialog::m_loadConfigHost(){
    CONFIG.beginGroup(CFG_ACTIVE);
    ui->lineEditTimeout->setText(CONFIG.value(CFG_VAL_TIMEOUT).toString());
    ui->lineEditThreads->setText(CONFIG.value(CFG_VAL_THREADS).toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value(CFG_VAL_AUTOSAVE).toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value(CFG_VAL_DUPLICATES).toBool());
    ui->groupBoxTimeout->setChecked(CONFIG.value(CFG_VAL_SETTIMEOUT).toBool());
    QString portScan = CONFIG.value(CFG_VAL_PORTSCAN).toString();
    QString record = CONFIG.value(CFG_VAL_RECORD).toString();
    QString nsType = CONFIG.value(CFG_VAL_NAMESERVER).toString();
    ui->lineEditPingTTL->setText(CONFIG.value("ping_ttl").toString());
    ui->lineEditPingByte->setText(CONFIG.value("ping_bytes").toString());
    CONFIG.endGroup();

    if(record == "A")
        ui->radioButtonA->setChecked(true);
    if(record == "AAAA")
        ui->radioButtonAAAA->setChecked(true);
    if(record == "ANY")
        ui->radioButtonANY->setChecked(true);

    if(portScan == "syn")
        ui->radioButtonSYN->setChecked(true);
    if(portScan == "connection")
        ui->radioButtonConnection->setChecked(true);

    if(nsType == "single")
        ui->radioButtonSingleNameserver->setChecked(true);
    if(nsType == "random")
        ui->radioButtonRandomNameservers->setChecked(true);
    if(nsType == "custom")
        ui->radioButtonCustomNameservers->setChecked(true);

    CONFIG.beginGroup(CFG_GRP_DEFAULT_NS);
    ui->comboBoxSingleNameserver->addItems(CONFIG.allKeys());
    CONFIG.endGroup();

    int size = CONFIG.beginReadArray("custom_nameservers_active");
    for (int i = 0; i < size; ++i) {
        CONFIG.setArrayIndex(i);
        ui->customNameservers->add(CONFIG.value("value").toString());
    }
    CONFIG.endArray();
}

void ActiveConfigDialog::m_loadConfigDns(){
    CONFIG.beginGroup(CFG_DNS);
    ui->lineEditThreads->setText(CONFIG.value(CFG_VAL_THREADS).toString());
    ui->lineEditTimeout->setText(CONFIG.value(CFG_VAL_TIMEOUT).toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value(CFG_VAL_AUTOSAVE).toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value(CFG_VAL_DUPLICATES).toBool());
    ui->groupBoxTimeout->setChecked(CONFIG.value(CFG_VAL_SETTIMEOUT).toBool());
    QString nsType = CONFIG.value(CFG_VAL_NAMESERVER).toString();
    CONFIG.endGroup();

    if(nsType == "single")
        ui->radioButtonSingleNameserver->setChecked(true);
    if(nsType == "random")
        ui->radioButtonRandomNameservers->setChecked(true);
    if(nsType == "custom")
        ui->radioButtonCustomNameservers->setChecked(true);

    CONFIG.beginGroup(CFG_GRP_DEFAULT_NS);
    ui->comboBoxSingleNameserver->addItems(CONFIG.allKeys());
    CONFIG.endGroup();

    int size = CONFIG.beginReadArray("custom_nameservers_dns");
    for (int i = 0; i < size; ++i) {
        CONFIG.setArrayIndex(i);
        ui->customNameservers->add(CONFIG.value("value").toString());
    }
    CONFIG.endArray();
}

void ActiveConfigDialog::m_loadConfigSSL(){
    CONFIG.beginGroup(CFG_SSL);
    ui->lineEditTimeout->setText(CONFIG.value(CFG_VAL_TIMEOUT).toString());
    ui->lineEditThreads->setText(CONFIG.value(CFG_VAL_THREADS).toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value(CFG_VAL_AUTOSAVE).toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value(CFG_VAL_DUPLICATES).toBool());
    ui->groupBoxTimeout->setChecked(CONFIG.value(CFG_VAL_SETTIMEOUT).toBool());
    CONFIG.endGroup();
}

void ActiveConfigDialog::m_loadConfigURL(){
    CONFIG.beginGroup(CFG_URL);
    ui->lineEditTimeout->setText(CONFIG.value(CFG_VAL_TIMEOUT).toString());
    ui->lineEditThreads->setText(CONFIG.value(CFG_VAL_THREADS).toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value(CFG_VAL_AUTOSAVE).toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value(CFG_VAL_DUPLICATES).toBool());
    ui->checkBoxRedirects->setChecked(CONFIG.value("follow_redirects").toBool());
    ui->checkBoxScreenshot->setChecked(CONFIG.value("take_screenshots").toBool());
    ui->checkBoxTitle->setChecked(CONFIG.value("get_title").toBool());
    ui->groupBoxTimeout->setChecked(CONFIG.value(CFG_VAL_SETTIMEOUT).toBool());
    ui->groupBoxScheme->setChecked(CONFIG.value(CFG_VAL_FORCESCHEME).toBool());

    QString scheme(CONFIG.value(CFG_VAL_SCHEME).toString());
    if(scheme == "https")
        ui->radioButtonHTTPS->setChecked(true);
    if(scheme == "http")
        ui->radioButtonHTTP->setChecked(true);
    if(scheme == "ftp")
        ui->radioButtonFTP->setChecked(true);

    CONFIG.endGroup();
}

void ActiveConfigDialog::m_loadConfigIP(){
    CONFIG.beginGroup(CFG_IP);
    ui->lineEditTimeout->setText(CONFIG.value(CFG_VAL_TIMEOUT).toString());
    ui->lineEditThreads->setText(CONFIG.value(CFG_VAL_THREADS).toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value(CFG_VAL_AUTOSAVE).toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value(CFG_VAL_DUPLICATES).toBool());
    ui->groupBoxTimeout->setChecked(CONFIG.value(CFG_VAL_SETTIMEOUT).toBool());
    QString nsType = CONFIG.value(CFG_VAL_NAMESERVER).toString();
    QString portScan = CONFIG.value(CFG_VAL_PORTSCAN).toString();
    ui->lineEditPingTTL->setText(CONFIG.value("ping_ttl").toString());
    ui->lineEditPingByte->setText(CONFIG.value("ping_bytes").toString());
    CONFIG.endGroup();

    if(nsType == "single")
        ui->radioButtonSingleNameserver->setChecked(true);
    if(nsType == "random")
        ui->radioButtonRandomNameservers->setChecked(true);
    if(nsType == "custom")
        ui->radioButtonCustomNameservers->setChecked(true);

    if(portScan == "syn")
        ui->radioButtonSYN->setChecked(true);
    if(portScan == "connection")
        ui->radioButtonConnection->setChecked(true);

    CONFIG.beginGroup(CFG_GRP_DEFAULT_NS);
    ui->comboBoxSingleNameserver->addItems(CONFIG.allKeys());
    CONFIG.endGroup();

    int size = CONFIG.beginReadArray("custom_nameservers_active");
    for (int i = 0; i < size; ++i) {
        CONFIG.setArrayIndex(i);
        ui->customNameservers->add(CONFIG.value("value").toString());
    }
    CONFIG.endArray();
}

///
/// saving configurations...
///

void ActiveConfigDialog::m_saveBrute(){
    /* get values... */

    QString thread = ui->lineEditThreads->text();
    QString timeout = ui->lineEditTimeout->text();

    bool wildcard = ui->checkBoxWildcards->isChecked();
    bool noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    bool autosaveToProject = ui->checkBoxAutosave->isChecked();
    bool setTimeout = ui->groupBoxTimeout->isChecked();

    bool recordA = ui->radioButtonA->isChecked();
    bool recordAAAA = ui->radioButtonAAAA->isChecked();
    bool recordANY = ui->radioButtonANY->isChecked();

    bool nsSingle = ui->radioButtonSingleNameserver->isChecked();
    bool nsRandom = ui->radioButtonRandomNameservers->isChecked();
    bool nsCustom = ui->radioButtonCustomNameservers->isChecked();



    /* saving values to config file... */

    CONFIG.beginGroup(CFG_BRUTE);
    CONFIG.setValue(CFG_VAL_THREADS, thread);
    CONFIG.setValue(CFG_VAL_TIMEOUT, timeout);
    CONFIG.setValue(CFG_VAL_WILDCARD, wildcard);
    CONFIG.setValue(CFG_VAL_DUPLICATES, noDuplicates);
    CONFIG.setValue(CFG_VAL_AUTOSAVE, autosaveToProject);
    CONFIG.setValue(CFG_VAL_SETTIMEOUT, setTimeout);
    CONFIG.setValue("nameserver", ui->comboBoxSingleNameserver->currentText());

    if(nsSingle)
        CONFIG.setValue(CFG_VAL_NAMESERVER, "single");
    if(nsRandom)
        CONFIG.setValue(CFG_VAL_NAMESERVER, "random");
    if(nsCustom)
        CONFIG.setValue(CFG_VAL_NAMESERVER, "custom");

    if(recordA)
        CONFIG.setValue(CFG_VAL_RECORD, "A");
    if(recordAAAA)
        CONFIG.setValue(CFG_VAL_RECORD, "AAAA");
    if(recordANY)
        CONFIG.setValue(CFG_VAL_RECORD, "ANY");
    CONFIG.endGroup();

    CONFIG.beginWriteArray("custom_nameservers_brute");
    QStringList customNameservers = m_customNameserverListModel->stringList();
    for (int i = 0; i < customNameservers.length(); ++i) {
        CONFIG.setArrayIndex(i);
        CONFIG.setValue("value", customNameservers.at(i));
    }
    CONFIG.endArray();

    /* saving to brute::ScanConfig structure... */

    m_configBrute->threads = thread.toInt();
    m_configBrute->timeout = timeout.toInt();

    m_configBrute->checkWildcard =  wildcard;
    m_configBrute->noDuplicates = noDuplicates;
    m_configBrute->autoSaveToProject = autosaveToProject;
    m_configBrute->setTimeout = setTimeout;

    if(recordA)
        m_configBrute->recordType = QDnsLookup::A;
    if(recordAAAA)
        m_configBrute->recordType = QDnsLookup::AAAA;
    if(recordANY)
        m_configBrute->recordType = QDnsLookup::ANY;

    m_configBrute->nameservers.clear();
    if(nsSingle){
        CONFIG.beginGroup(CFG_GRP_DEFAULT_NS);
        QString nameserver = CONFIG.value(ui->comboBoxSingleNameserver->currentText()).toString();
        m_configBrute->nameservers.enqueue(nameserver);
        CONFIG.endGroup();
    }
    if(nsRandom){
        CONFIG.beginGroup(CFG_GRP_DEFAULT_NS);
        QStringList nameservers = CONFIG.childKeys();
        foreach(const QString &key, nameservers)
            m_configBrute->nameservers.enqueue(CONFIG.value(key).toString());
        CONFIG.endGroup();
    }
    if(nsCustom){
        foreach(const QString &nameserver, m_customNameserverListModel->stringList())
            m_configBrute->nameservers.enqueue(nameserver);
    }

    /* save used nameservers to config file */
    CONFIG.beginWriteArray("nameservers_brute");
    for (int i = 0; i < m_configBrute->nameservers.length(); ++i) {
        CONFIG.setArrayIndex(i);
        CONFIG.setValue("value", m_configBrute->nameservers.at(i));
    }
    CONFIG.endArray();
}

void ActiveConfigDialog::m_saveHost(){
    /* get values */
    QString thread = ui->lineEditThreads->text();
    QString timeout = ui->lineEditTimeout->text();
    int ping_ttl = ui->lineEditPingTTL->text().toInt();
    int ping_byte = ui->lineEditPingByte->text().toInt();

    bool noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    bool autosaveToProject = ui->checkBoxAutosave->isChecked();
    bool setTimeout = ui->groupBoxTimeout->isChecked();

    bool recordA = ui->radioButtonA->isChecked();
    bool recordAAAA = ui->radioButtonAAAA->isChecked();
    bool recordANY = ui->radioButtonANY->isChecked();

    bool nsSingle = ui->radioButtonSingleNameserver->isChecked();
    bool nsRandom = ui->radioButtonRandomNameservers->isChecked();
    bool nsCustom = ui->radioButtonCustomNameservers->isChecked();

    bool syn = ui->radioButtonSYN->isChecked();
    bool connection = ui->radioButtonConnection->isChecked();


    /* saving values to config file... */
    CONFIG.beginGroup(CFG_ACTIVE);
    CONFIG.setValue(CFG_VAL_THREADS, thread);
    CONFIG.setValue(CFG_VAL_TIMEOUT, timeout);
    CONFIG.setValue(CFG_VAL_DUPLICATES, noDuplicates);
    CONFIG.setValue(CFG_VAL_AUTOSAVE, autosaveToProject);
    CONFIG.setValue(CFG_VAL_SETTIMEOUT, setTimeout);
    CONFIG.setValue("ping_ttl", ping_ttl);
    CONFIG.setValue("ping_bytes", ping_byte);

    if(nsSingle)
        CONFIG.setValue(CFG_VAL_NAMESERVER, "single");
    if(nsRandom)
        CONFIG.setValue(CFG_VAL_NAMESERVER, "random");
    if(nsCustom)
        CONFIG.setValue(CFG_VAL_NAMESERVER, "custom");

    if(recordA)
        CONFIG.setValue(CFG_VAL_RECORD, "A");
    if(recordAAAA)
        CONFIG.setValue(CFG_VAL_RECORD, "AAAA");
    if(recordANY)
        CONFIG.setValue(CFG_VAL_RECORD, "ANY");

    if(syn)
        CONFIG.setValue(CFG_VAL_PORTSCAN, "syn");
    if(connection)
        CONFIG.setValue(CFG_VAL_PORTSCAN, "connection");
    CONFIG.endGroup();

    CONFIG.beginWriteArray("custom_nameservers_active");
    QStringList customNameservers = m_customNameserverListModel->stringList();
    for (int i = 0; i < customNameservers.length(); ++i) {
        CONFIG.setArrayIndex(i);
        CONFIG.setValue("value", customNameservers.at(i));
    }
    CONFIG.endArray();

    /* saving to brute::ScanConfig structure... */

    m_configHost->timeout = timeout.toInt();
    m_configHost->threads = thread.toInt();
    m_configHost->noDuplicates = noDuplicates;
    m_configHost->autoSaveToProject = autosaveToProject;
    m_configHost->setTimeout = setTimeout;
    m_configHost->pingScanConfig->ttl = ping_ttl;
    m_configHost->pingScanConfig->data_size = ping_byte;

    if(recordA)
        m_configHost->recordType = QDnsLookup::A;
    if(recordAAAA)
        m_configHost->recordType = QDnsLookup::AAAA;
    if(recordANY)
        m_configHost->recordType = QDnsLookup::ANY;

    if(syn)
        m_configHost->portScanConfig->scan_type = port::ScanType::SYN;
    if(connection)
        m_configHost->portScanConfig->scan_type = port::ScanType::CONNECTION;

    m_configHost->nameservers.clear();
    if(nsSingle){
        CONFIG.beginGroup(CFG_GRP_DEFAULT_NS);
        QString nameserver = CONFIG.value(ui->comboBoxSingleNameserver->currentText()).toString();
        m_configHost->nameservers.enqueue(nameserver);
        CONFIG.endGroup();
    }
    if(nsRandom){
        CONFIG.beginGroup(CFG_GRP_DEFAULT_NS);
        QStringList nameservers = CONFIG.childKeys();
        foreach(const QString &key, nameservers)
            m_configHost->nameservers.enqueue(CONFIG.value(key).toString());
        CONFIG.endGroup();
    }
    if(nsCustom){
        foreach(const QString &nameserver, m_customNameserverListModel->stringList())
            m_configHost->nameservers.enqueue(nameserver);
    }

    /* save used nameservers to config file */
    CONFIG.beginWriteArray("nameservers_active");
    for (int i = 0; i < m_configHost->nameservers.length(); ++i) {
        CONFIG.setArrayIndex(i);
        CONFIG.setValue("value", m_configHost->nameservers.at(i));
    }
    CONFIG.endArray();
}

void ActiveConfigDialog::m_saveDns(){
    /* get values... */

    QString thread = ui->lineEditThreads->text();
    QString timeout = ui->lineEditTimeout->text();

    bool noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    bool autosaveToProject = ui->checkBoxAutosave->isChecked();
    bool setTimeout = ui->groupBoxTimeout->isChecked();

    bool nsSingle = ui->radioButtonSingleNameserver->isChecked();
    bool nsRandom = ui->radioButtonRandomNameservers->isChecked();
    bool nsCustom = ui->radioButtonCustomNameservers->isChecked();


    /* saving values to config file... */
    CONFIG.beginGroup(CFG_DNS);
    CONFIG.setValue(CFG_VAL_THREADS, thread);
    CONFIG.setValue(CFG_VAL_TIMEOUT, timeout);
    CONFIG.setValue(CFG_VAL_DUPLICATES, noDuplicates);
    CONFIG.setValue(CFG_VAL_AUTOSAVE, autosaveToProject);
    CONFIG.setValue(CFG_VAL_SETTIMEOUT, setTimeout);
    CONFIG.setValue("nameserver", ui->comboBoxSingleNameserver->currentText());

    if(nsSingle)
        CONFIG.setValue(CFG_VAL_NAMESERVER, "single");
    if(nsRandom)
        CONFIG.setValue(CFG_VAL_NAMESERVER, "random");
    if(nsCustom)
        CONFIG.setValue(CFG_VAL_NAMESERVER, "custom");
    CONFIG.endGroup();

    CONFIG.beginWriteArray("custom_nameservers_dns");
    QStringList customNameservers = m_customNameserverListModel->stringList();
    for (int i = 0; i < customNameservers.length(); ++i) {
        CONFIG.setArrayIndex(i);
        CONFIG.setValue("value", customNameservers.at(i));
    }
    CONFIG.endArray();

    /* saving to brute::ScanConfig structure... */

    m_configDns->threads = thread.toInt();
    m_configDns->timeout = timeout.toInt();
    m_configDns->noDuplicates = noDuplicates;
    m_configDns->autoSaveToProject = autosaveToProject;
    m_configDns->setTimeout = setTimeout;

    m_configDns->nameservers.clear();
    if(nsSingle){
        CONFIG.beginGroup(CFG_GRP_DEFAULT_NS);
        QString nameserver = CONFIG.value(ui->comboBoxSingleNameserver->currentText()).toString();
        m_configDns->nameservers.enqueue(nameserver);
        CONFIG.endGroup();
    }
    if(nsRandom){
        CONFIG.beginGroup(CFG_GRP_DEFAULT_NS);
        QStringList nameservers = CONFIG.childKeys();
        foreach(const QString &key, nameservers)
            m_configDns->nameservers.enqueue(CONFIG.value(key).toString());
        CONFIG.endGroup();
    }
    if(nsCustom){
        foreach(const QString &nameserver, m_customNameserverListModel->stringList())
            m_configDns->nameservers.enqueue(nameserver);
    }

    /* save used nameservers to config file */
    CONFIG.beginWriteArray("nameservers_dns");
    for (int i = 0; i < m_configDns->nameservers.length(); ++i) {
        CONFIG.setArrayIndex(i);
        CONFIG.setValue("value", m_configDns->nameservers.at(i));
    }
    CONFIG.endArray();
}

void ActiveConfigDialog::m_saveSSL(){
    /* get values... */

    QString thread = ui->lineEditThreads->text();
    QString timeout = ui->lineEditTimeout->text();

    bool noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    bool autosaveToProject = ui->checkBoxAutosave->isChecked();
    bool setTimeout = ui->groupBoxTimeout->isChecked();

    /* saving values to config file... */

    CONFIG.beginGroup(CFG_SSL);
    CONFIG.setValue(CFG_VAL_THREADS, thread);
    CONFIG.setValue(CFG_VAL_TIMEOUT, timeout);
    CONFIG.setValue(CFG_VAL_DUPLICATES, noDuplicates);
    CONFIG.setValue(CFG_VAL_AUTOSAVE, autosaveToProject);
    CONFIG.setValue(CFG_VAL_SETTIMEOUT, setTimeout);
    CONFIG.endGroup();

    /* saving to ssl::ScanConfig structure... */

    m_configSSL->timeout = timeout.toInt();
    m_configSSL->threads = thread.toInt();
    m_configSSL->noDuplicates = noDuplicates;
    m_configSSL->autoSaveToProject = autosaveToProject;
    m_configSSL->setTimeout = setTimeout;
}

void ActiveConfigDialog::m_saveURL(){
    /* get values... */

    QString thread = ui->lineEditThreads->text();
    QString timeout = ui->lineEditTimeout->text();

    QString scheme;
    if(ui->radioButtonHTTP->isChecked())
        scheme = "http";
    if(ui->radioButtonHTTPS->isChecked())
        scheme = "https";
    if(ui->radioButtonFTP->isChecked())
        scheme = "ftp";

    bool noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    bool autosaveToProject = ui->checkBoxAutosave->isChecked();
    bool setTimeout = ui->groupBoxTimeout->isChecked();
    bool force_scheme = ui->groupBoxScheme->isChecked();
    bool redirect = ui->checkBoxRedirects->isChecked();
    bool screenshot = ui->checkBoxScreenshot->isChecked();
    bool title = ui->checkBoxTitle->isChecked();

    /* saving values to config file... */

    CONFIG.beginGroup(CFG_URL);
    CONFIG.setValue(CFG_VAL_THREADS, thread);
    CONFIG.setValue(CFG_VAL_TIMEOUT, timeout);
    CONFIG.setValue(CFG_VAL_DUPLICATES, noDuplicates);
    CONFIG.setValue(CFG_VAL_AUTOSAVE, autosaveToProject);
    CONFIG.setValue(CFG_VAL_SETTIMEOUT, setTimeout);
    CONFIG.setValue(CFG_VAL_SCHEME, scheme);
    CONFIG.setValue(CFG_VAL_FORCESCHEME, force_scheme);
    CONFIG.setValue("take_screenshots", screenshot);
    CONFIG.setValue("follow_redirects", redirect);
    CONFIG.setValue("get_title", title);
    CONFIG.endGroup();

    /* saving to ssl::ScanConfig structure... */

    m_configURL->timeout = timeout.toInt();
    m_configURL->threads = thread.toInt();
    m_configURL->scheme = scheme;
    m_configURL->force_scheme = force_scheme;
    m_configURL->noDuplicates = noDuplicates;
    m_configURL->autoSaveToProject = autosaveToProject;
    m_configURL->setTimeout = setTimeout;
    m_configURL->follow_redirect = redirect;
    m_configURL->take_screenshots = screenshot;
    m_configURL->get_title = title;
}

void ActiveConfigDialog::m_saveIP(){
    /* get values... */
    QString thread = ui->lineEditThreads->text();
    QString timeout = ui->lineEditTimeout->text();
    int ping_ttl = ui->lineEditPingTTL->text().toInt();
    int ping_byte = ui->lineEditPingByte->text().toInt();

    bool noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    bool autosaveToProject = ui->checkBoxAutosave->isChecked();
    bool setTimeout = ui->groupBoxTimeout->isChecked();

    bool nsSingle = ui->radioButtonSingleNameserver->isChecked();
    bool nsRandom = ui->radioButtonRandomNameservers->isChecked();
    bool nsCustom = ui->radioButtonCustomNameservers->isChecked();

    bool syn = ui->radioButtonSYN->isChecked();
    bool connection = ui->radioButtonConnection->isChecked();

    /* saving values to config file... */
    CONFIG.beginGroup(CFG_IP);
    CONFIG.setValue(CFG_VAL_THREADS, thread);
    CONFIG.setValue(CFG_VAL_TIMEOUT, timeout);
    CONFIG.setValue(CFG_VAL_DUPLICATES, noDuplicates);
    CONFIG.setValue(CFG_VAL_AUTOSAVE, autosaveToProject);
    CONFIG.setValue(CFG_VAL_SETTIMEOUT, setTimeout);
    CONFIG.setValue("ping_ttl", ping_ttl);
    CONFIG.setValue("ping_bytes", ping_byte);

    if(nsSingle)
        CONFIG.setValue(CFG_VAL_NAMESERVER, "single");
    if(nsRandom)
        CONFIG.setValue(CFG_VAL_NAMESERVER, "random");
    if(nsCustom)
        CONFIG.setValue(CFG_VAL_NAMESERVER, "custom");

    if(syn)
        CONFIG.setValue(CFG_VAL_PORTSCAN, "syn");
    if(connection)
        CONFIG.setValue(CFG_VAL_PORTSCAN, "connection");
    CONFIG.endGroup();

    CONFIG.beginWriteArray("custom_nameservers_active");
    QStringList customNameservers = m_customNameserverListModel->stringList();
    for (int i = 0; i < customNameservers.length(); ++i) {
        CONFIG.setArrayIndex(i);
        CONFIG.setValue("value", customNameservers.at(i));
    }
    CONFIG.endArray();

    /* saving to brute::ScanConfig structure... */

    m_configIP->timeout = timeout.toInt();
    m_configIP->threads = thread.toInt();
    m_configIP->noDuplicates = noDuplicates;
    m_configIP->autoSaveToProject = autosaveToProject;
    m_configIP->setTimeout = setTimeout;
    m_configIP->pingScanConfig->ttl = ping_ttl;
    m_configIP->pingScanConfig->data_size = ping_byte;

    if(syn)
        m_configIP->portScanConfig->scan_type = port::ScanType::SYN;
    if(connection)
        m_configIP->portScanConfig->scan_type = port::ScanType::CONNECTION;

    m_configIP->nameservers.clear();
    if(nsSingle){
        CONFIG.beginGroup(CFG_GRP_DEFAULT_NS);
        QString nameserver = CONFIG.value(ui->comboBoxSingleNameserver->currentText()).toString();
        m_configIP->nameservers.enqueue(nameserver);
        CONFIG.endGroup();
    }
    if(nsRandom){
        CONFIG.beginGroup(CFG_GRP_DEFAULT_NS);
        QStringList nameservers = CONFIG.childKeys();
        foreach(const QString &key, nameservers)
            m_configIP->nameservers.enqueue(CONFIG.value(key).toString());
        CONFIG.endGroup();
    }
    if(nsCustom){
        foreach(const QString &nameserver, m_customNameserverListModel->stringList())
            m_configIP->nameservers.enqueue(nameserver);
    }

    /* save used nameservers to config file */
    CONFIG.beginWriteArray("nameservers_active");
    for (int i = 0; i < m_configIP->nameservers.length(); ++i) {
        CONFIG.setArrayIndex(i);
        CONFIG.setValue("value", m_configIP->nameservers.at(i));
    }
    CONFIG.endArray();
}

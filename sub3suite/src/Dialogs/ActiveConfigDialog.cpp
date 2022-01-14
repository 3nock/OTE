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

    brute = true;
    this->m_initWidgets();
    this->m_loadConfigBrute();
}

/* for active... */
ActiveConfigDialog::ActiveConfigDialog(QWidget *parent, active::ScanConfig *config) :
    QDialog(parent),
    ui(new Ui::ActiveConfigDialog),
    m_configActive(config),
    m_customNameserverListModel(new QStringListModel)
{
    ui->setupUi(this);

    active = true;
    this->m_initWidgets();
    this->m_loadConfigActive();

    /* hiding unused widgets */
    ui->groupBoxLevel->hide();
}

/* for records... */
ActiveConfigDialog::ActiveConfigDialog(QWidget *parent, dns::ScanConfig *config) :
    QDialog(parent),
    ui(new Ui::ActiveConfigDialog),
    m_configDns(config),
    m_customNameserverListModel(new QStringListModel)
{
    ui->setupUi(this);

    dns = true;
    this->m_initWidgets();
    this->m_loadConfigDns();

    /* hiding unused widgets */
    ui->groupBoxLevel->hide();
    ui->checkBoxWildcards->hide();
    ui->radioButtonA->hide();
    ui->radioButtonAAAA->hide();
    ui->labelRecordType->hide();
}

/* for SSL... */
ActiveConfigDialog::ActiveConfigDialog(QWidget *parent, ssl::ScanConfig *config) :
    QDialog(parent),
    ui(new Ui::ActiveConfigDialog),
    m_configSSL(config),
    m_customNameserverListModel(new QStringListModel)
{
    ui->setupUi(this);

    ssl = true;
    this->m_initWidgets();
}

/* for IP */
ActiveConfigDialog::ActiveConfigDialog(QWidget *parent, ip::ScanConfig *config) :
    QDialog(parent),
    ui(new Ui::ActiveConfigDialog),
    m_configIp(config),
    m_customNameserverListModel(new QStringListModel)
{
    ui->setupUi(this);

    ip = true;
    this->m_initWidgets();
}

ActiveConfigDialog::~ActiveConfigDialog(){
    delete ui;
    delete m_customNameserverListModel;
}

void ActiveConfigDialog::on_buttonCancel_clicked(){
    accept();
}

void ActiveConfigDialog::on_buttonOk_clicked(){
    if(brute)
        this->m_saveBrute();
    if(active)
        this->m_saveActive();
    if(dns)
        this->m_saveDns();
    if(ssl)
        this->m_saveSSL();
    if(ip)
        this->m_saveIp();

    /* close the dialog */
    accept();
}

void ActiveConfigDialog::m_initWidgets(){
    /* setting placeholdertxts... */
    ui->lineEditTimeout->setPlaceholderText("e.g. 3");
    ui->lineEditThreads->setPlaceholderText("e.g. 100");
    ui->lineEditLevels->setPlaceholderText("e.g. 2");

    /* setting validators... */
    ui->lineEditTimeout->setValidator(new QIntValidator(1, 1000, this));
    ui->lineEditThreads->setValidator(new QIntValidator(1, 200, this));
    ui->lineEditLevels->setValidator(new QIntValidator(1, 5, this));

    /* custom-nameserver list */
    ui->customNameservers->setListName("Namerserver");
    ui->customNameservers->setListModel(m_customNameserverListModel);
}

/* get config settings from config file... */

void ActiveConfigDialog::m_loadConfigBrute(){
    ui->lineEditTimeout->setText(CONFIG_BRUTE.value("timeout").toString());
    ui->lineEditThreads->setText(CONFIG_BRUTE.value("threads").toString());
    ui->lineEditLevels->setText(CONFIG_BRUTE.value("maxLevel").toString());
    ui->checkBoxWildcards->setChecked(CONFIG_BRUTE.value("wildcard").toBool());
    ui->groupBoxLevel->setChecked(CONFIG_BRUTE.value("useLevel").toInt());
    ui->checkBoxAutosave->setChecked(CONFIG_BRUTE.value("autosaveToProject").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG_BRUTE.value("noDuplicates").toBool());

    QString record = CONFIG_BRUTE.value("record").toString();
    if(record == "A")
        ui->radioButtonA->setChecked(true);
    if(record == "AAAA")
        ui->radioButtonAAAA->setChecked(true);

    QString nsType = CONFIG_BRUTE.value("nameserverType").toString();
    if(nsType == "single")
        ui->radioButtonSingleNameserver->setChecked(true);
    if(nsType == "random")
        ui->radioButtonRandomNameservers->setChecked(true);
    if(nsType == "custom")
        ui->radioButtonCustomNameservers->setChecked(true);

    CONFIG_BRUTE.beginGroup("Default-Nameservers");
    ui->comboBoxSingleNameserver->addItems(CONFIG_BRUTE.allKeys());
    CONFIG_BRUTE.endGroup();
    ui->comboBoxSingleNameserver->setCurrentText(CONFIG_BRUTE.value("nameserver").toString());

    int size = CONFIG_BRUTE.beginReadArray("Custom-Nameservers");
    for (int i = 0; i < size; ++i) {
        CONFIG_BRUTE.setArrayIndex(i);
        ui->customNameservers->add(CONFIG_BRUTE.value("value").toString());
    }
    CONFIG_BRUTE.endArray();
}

void ActiveConfigDialog::m_loadConfigActive(){
    ui->lineEditTimeout->setText(CONFIG_ACTIVE.value("timeout").toString());
    ui->lineEditThreads->setText(CONFIG_ACTIVE.value("threads").toString());
    ui->checkBoxWildcards->setChecked(CONFIG_ACTIVE.value("wildcard").toBool());
    ui->checkBoxAutosave->setChecked(CONFIG_ACTIVE.value("autosaveToProject").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG_ACTIVE.value("noDuplicates").toBool());

    QString record = CONFIG_ACTIVE.value("record").toString();
    if(record == "A")
        ui->radioButtonA->setChecked(true);
    if(record == "AAAA")
        ui->radioButtonAAAA->setChecked(true);

    QString nsType = CONFIG_ACTIVE.value("nameserverType").toString();
    if(nsType == "single")
        ui->radioButtonSingleNameserver->setChecked(true);
    if(nsType == "random")
        ui->radioButtonRandomNameservers->setChecked(true);
    if(nsType == "custom")
        ui->radioButtonCustomNameservers->setChecked(true);

    CONFIG_ACTIVE.beginGroup("Default-Nameservers");
    ui->comboBoxSingleNameserver->addItems(CONFIG_ACTIVE.allKeys());
    CONFIG_ACTIVE.endGroup();
    ui->comboBoxSingleNameserver->setCurrentText(CONFIG_ACTIVE.value("nameserver").toString());

    int size = CONFIG_ACTIVE.beginReadArray("Custom-Nameservers");
    for (int i = 0; i < size; ++i) {
        CONFIG_ACTIVE.setArrayIndex(i);
        ui->customNameservers->add(CONFIG_ACTIVE.value("value").toString());
    }
    CONFIG_ACTIVE.endArray();
}

void ActiveConfigDialog::m_loadConfigDns(){
    ui->lineEditTimeout->setText(CONFIG_DNS.value("timeout").toString());
    ui->lineEditThreads->setText(CONFIG_DNS.value("threads").toString());
    ui->checkBoxAutosave->setChecked(CONFIG_DNS.value("autosaveToProject").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG_DNS.value("noDuplicates").toBool());

    QString nsType = CONFIG_DNS.value("nameserverType").toString();
    if(nsType == "single")
        ui->radioButtonSingleNameserver->setChecked(true);
    if(nsType == "random")
        ui->radioButtonRandomNameservers->setChecked(true);
    if(nsType == "custom")
        ui->radioButtonCustomNameservers->setChecked(true);

    CONFIG_DNS.beginGroup("Default-Nameservers");
    ui->comboBoxSingleNameserver->addItems(CONFIG_DNS.allKeys());
    CONFIG_DNS.endGroup();
    ui->comboBoxSingleNameserver->setCurrentText(CONFIG_DNS.value("nameserver").toString());

    int size = CONFIG_DNS.beginReadArray("Custom-Nameservers");
    for (int i = 0; i < size; ++i) {
        CONFIG_DNS.setArrayIndex(i);
        ui->customNameservers->add(CONFIG_DNS.value("value").toString());
    }
    CONFIG_DNS.endArray();
}

/* saving configurations... */

void ActiveConfigDialog::m_saveBrute(){
    /* get values... */

    QString thread = ui->lineEditThreads->text();
    QString timeout = ui->lineEditTimeout->text();
    QString maxlevel = ui->lineEditLevels->text();

    bool wildcard = ui->checkBoxWildcards->isChecked();
    bool useLevel = ui->groupBoxLevel->isChecked();
    bool noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    bool autosaveToProject = ui->checkBoxAutosave->isChecked();

    bool recordA = ui->radioButtonA->isChecked();
    bool recordAAAA = ui->radioButtonAAAA->isChecked();

    bool nsSingle = ui->radioButtonSingleNameserver->isChecked();
    bool nsRandom = ui->radioButtonRandomNameservers->isChecked();
    bool nsCustom = ui->radioButtonCustomNameservers->isChecked();


    /* saving values to config file... */

    CONFIG_BRUTE.setValue("threads", thread);
    CONFIG_BRUTE.setValue("timeout", timeout);
    CONFIG_BRUTE.setValue("maxlevel", maxlevel);
    CONFIG_BRUTE.setValue("useLevel", useLevel);
    CONFIG_BRUTE.setValue("wildcard", wildcard);
    CONFIG_BRUTE.setValue("noDuplicates", noDuplicates);
    CONFIG_BRUTE.setValue("autosaveToProject", autosaveToProject);
    CONFIG_BRUTE.setValue("nameserver", ui->comboBoxSingleNameserver->currentText());

    if(nsSingle)
        CONFIG_BRUTE.setValue("nameserverType", "single");
    if(nsRandom)
        CONFIG_BRUTE.setValue("nameserverType", "random");
    if(nsCustom)
        CONFIG_BRUTE.setValue("nameserverType", "custom");

    if(recordA)
        CONFIG_BRUTE.setValue("record", "A");
    if(recordAAAA)
        CONFIG_BRUTE.setValue("record", "AAAA");

    CONFIG_BRUTE.beginWriteArray("Custom-Nameservers");
    QStringList customNameservers = m_customNameserverListModel->stringList();
    for (int i = 0; i < customNameservers.length(); ++i) {
        CONFIG_BRUTE.setArrayIndex(i);
        CONFIG_BRUTE.setValue("value", customNameservers.at(i));
    }
    CONFIG_BRUTE.endArray();

    /* saving to brute::ScanConfig structure... */

    m_configBrute->timeout = timeout.toInt();
    m_configBrute->threads = thread.toInt();
    m_configBrute->levels = maxlevel.toInt();

    m_configBrute->checkWildcard =  wildcard;
    m_configBrute->multiLevelScan = useLevel;
    m_configBrute->noDuplicates = noDuplicates;
    m_configBrute->autoSaveToProject = autosaveToProject;

    if(recordA)
        m_configBrute->recordType = QDnsLookup::A;
    if(recordAAAA)
        m_configBrute->recordType = QDnsLookup::AAAA;

    if(nsSingle){
        m_configBrute->nameservers.clear();
        CONFIG_BRUTE.beginGroup("Default-Nameservers");
        QString nameserver = CONFIG_BRUTE.value(ui->comboBoxSingleNameserver->currentText()).toString();
        m_configBrute->nameservers.append(nameserver);
        CONFIG_BRUTE.endGroup();
    }
    if(nsRandom){
        m_configBrute->nameservers.clear();
        CONFIG_BRUTE.beginGroup("Default-Nameservers");
        QStringList nameservers = CONFIG_BRUTE.allKeys();
        foreach(const QString &key, nameservers){
            QString nameserver = CONFIG_BRUTE.value(key).toString();
            m_configBrute->nameservers.append(nameserver);
        }
        CONFIG_BRUTE.endGroup();
    }
    if(nsCustom){
        m_configBrute->nameservers.clear();
        QStringList nameservers = m_customNameserverListModel->stringList();
        m_configBrute->nameservers = nameservers;
    }

    /* save used nameservers to config file */
    CONFIG_BRUTE.beginWriteArray("Nameservers");
    for (int i = 0; i < m_configBrute->nameservers.length(); ++i) {
        CONFIG_BRUTE.setArrayIndex(i);
        CONFIG_BRUTE.setValue("value", m_configBrute->nameservers.at(i));
    }
    CONFIG_BRUTE.endArray();
}

void ActiveConfigDialog::m_saveActive(){
    /* get values... */

    QString thread = ui->lineEditThreads->text();
    QString timeout = ui->lineEditTimeout->text();

    bool wildcard = ui->checkBoxWildcards->isChecked();
    bool noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    bool autosaveToProject = ui->checkBoxAutosave->isChecked();

    bool recordA = ui->radioButtonA->isChecked();
    bool recordAAAA = ui->radioButtonAAAA->isChecked();

    bool nsSingle = ui->radioButtonSingleNameserver->isChecked();
    bool nsRandom = ui->radioButtonRandomNameservers->isChecked();
    bool nsCustom = ui->radioButtonCustomNameservers->isChecked();


    /* saving values to config file... */

    CONFIG_ACTIVE.setValue("threads", thread);
    CONFIG_ACTIVE.setValue("timeout", timeout);
    CONFIG_ACTIVE.setValue("wildcard", wildcard);
    CONFIG_ACTIVE.setValue("noDuplicates", noDuplicates);
    CONFIG_ACTIVE.setValue("autosaveToProject", autosaveToProject);
    CONFIG_ACTIVE.setValue("nameserver", ui->comboBoxSingleNameserver->currentText());

    if(nsSingle)
        CONFIG_ACTIVE.setValue("nameserverType", "single");
    if(nsRandom)
        CONFIG_ACTIVE.setValue("nameserverType", "random");
    if(nsCustom)
        CONFIG_ACTIVE.setValue("nameserverType", "custom");

    if(recordA)
        CONFIG_ACTIVE.setValue("record", "A");
    if(recordAAAA)
        CONFIG_ACTIVE.setValue("record", "AAAA");

    CONFIG_ACTIVE.beginWriteArray("Custom-Nameservers");
    QStringList customNameservers = m_customNameserverListModel->stringList();
    for (int i = 0; i < customNameservers.length(); ++i) {
        CONFIG_ACTIVE.setArrayIndex(i);
        CONFIG_ACTIVE.setValue("value", customNameservers.at(i));
    }
    CONFIG_ACTIVE.endArray();

    /* saving to brute::ScanConfig structure... */

    m_configActive->timeout = timeout.toInt();
    m_configActive->threads = thread.toInt();
    m_configActive->checkWildcard =  wildcard;
    m_configActive->noDuplicates = noDuplicates;
    m_configActive->autoSaveToProject = autosaveToProject;

    if(recordA)
        m_configActive->recordType = QDnsLookup::A;
    if(recordAAAA)
        m_configActive->recordType = QDnsLookup::AAAA;

    if(nsSingle){
        m_configActive->nameservers.clear();
        CONFIG_ACTIVE.beginGroup("Default-Nameservers");
        QString nameserver = CONFIG_ACTIVE.value(ui->comboBoxSingleNameserver->currentText()).toString();
        m_configActive->nameservers.append(nameserver);
        CONFIG_ACTIVE.endGroup();
    }
    if(nsRandom){
        m_configActive->nameservers.clear();
        CONFIG_ACTIVE.beginGroup("Default-Nameservers");
        QStringList nameservers = CONFIG_ACTIVE.allKeys();
        foreach(const QString &key, nameservers){
            QString nameserver = CONFIG_ACTIVE.value(key).toString();
            m_configActive->nameservers.append(nameserver);
        }
        CONFIG_ACTIVE.endGroup();
    }
    if(nsCustom){
        m_configActive->nameservers.clear();
        QStringList nameservers = m_customNameserverListModel->stringList();
        m_configActive->nameservers = nameservers;
    }

    /* save used nameservers to config file */
    CONFIG_ACTIVE.beginWriteArray("Nameservers");
    for (int i = 0; i < m_configActive->nameservers.length(); ++i) {
        CONFIG_ACTIVE.setArrayIndex(i);
        CONFIG_ACTIVE.setValue("value", m_configActive->nameservers.at(i));
    }
    CONFIG_ACTIVE.endArray();
}

void ActiveConfigDialog::m_saveDns(){
    /* get values... */

    QString thread = ui->lineEditThreads->text();
    QString timeout = ui->lineEditTimeout->text();

    bool noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    bool autosaveToProject = ui->checkBoxAutosave->isChecked();

    bool nsSingle = ui->radioButtonSingleNameserver->isChecked();
    bool nsRandom = ui->radioButtonRandomNameservers->isChecked();
    bool nsCustom = ui->radioButtonCustomNameservers->isChecked();


    /* saving values to config file... */

    CONFIG_DNS.setValue("threads", thread);
    CONFIG_DNS.setValue("timeout", timeout);
    CONFIG_DNS.setValue("noDuplicates", noDuplicates);
    CONFIG_DNS.setValue("autosaveToProject", autosaveToProject);
    CONFIG_DNS.setValue("nameserver", ui->comboBoxSingleNameserver->currentText());

    if(nsSingle)
        CONFIG_DNS.setValue("nameserverType", "single");
    if(nsRandom)
        CONFIG_DNS.setValue("nameserverType", "random");
    if(nsCustom)
        CONFIG_DNS.setValue("nameserverType", "custom");

    CONFIG_DNS.beginWriteArray("Custom-Nameservers");
    QStringList customNameservers = m_customNameserverListModel->stringList();
    for (int i = 0; i < customNameservers.length(); ++i) {
        CONFIG_DNS.setArrayIndex(i);
        CONFIG_DNS.setValue("value", customNameservers.at(i));
    }
    CONFIG_DNS.endArray();

    /* saving to brute::ScanConfig structure... */

    m_configDns->timeout = timeout.toInt();
    m_configDns->threads = thread.toInt();
    m_configDns->noDuplicates = noDuplicates;
    m_configDns->autoSaveToProject = autosaveToProject;

    if(nsSingle){
        m_configDns->nameservers.clear();
        CONFIG_DNS.beginGroup("Default-Nameservers");
        QString nameserver = CONFIG_DNS.value(ui->comboBoxSingleNameserver->currentText()).toString();
        m_configDns->nameservers.append(nameserver);
        CONFIG_DNS.endGroup();
    }
    if(nsRandom){
        m_configDns->nameservers.clear();
        CONFIG_DNS.beginGroup("Default-Nameservers");
        QStringList nameservers = CONFIG_DNS.allKeys();
        foreach(const QString &key, nameservers){
            QString nameserver = CONFIG_DNS.value(key).toString();
            m_configDns->nameservers.append(nameserver);
        }
        CONFIG_DNS.endGroup();
    }
    if(nsCustom){
        m_configDns->nameservers.clear();
        QStringList nameservers = m_customNameserverListModel->stringList();
        m_configDns->nameservers = nameservers;
    }

    /* save used nameservers to config file */
    CONFIG_DNS.beginWriteArray("Nameservers");
    for (int i = 0; i < m_configDns->nameservers.length(); ++i) {
        CONFIG_DNS.setArrayIndex(i);
        CONFIG_DNS.setValue("value", m_configDns->nameservers.at(i));
    }
    CONFIG_DNS.endArray();
}

void ActiveConfigDialog::m_saveSSL(){

}

void ActiveConfigDialog::m_saveIp(){

}

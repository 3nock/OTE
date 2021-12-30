#include "ActiveConfigDialog.h"
#include "ui_ActiveConfigDialog.h"


/* for brute... */
ActiveConfigDialog::ActiveConfigDialog(QWidget *parent, brute::ScanConfig *config) :
    QDialog(parent),
    ui(new Ui::ActiveConfigDialog),
    m_configBrute(config),
    m_customNameserverListModel(new QStringListModel)
{
    ui->setupUi(this);

    m_settings = new QSettings(QDir::currentPath()+"/config/brute.ini", QSettings::IniFormat);
    brute = true;

    this->m_initWidgets();
    this->m_loadConfigValues();
    this->m_loadCustomNameservers();
    this->m_loadDefaultNameservers();
}

/* for active... */
ActiveConfigDialog::ActiveConfigDialog(QWidget *parent, active::ScanConfig *config) :
    QDialog(parent),
    ui(new Ui::ActiveConfigDialog),
    m_configActive(config),
    m_customNameserverListModel(new QStringListModel)
{
    ui->setupUi(this);

    m_settings = new QSettings(QDir::currentPath()+"/config/active.ini", QSettings::IniFormat);
    active = true;

    this->m_initWidgets();
    this->m_loadConfigValues();
    this->m_loadCustomNameservers();
    this->m_loadDefaultNameservers();
}

/* for records... */
ActiveConfigDialog::ActiveConfigDialog(QWidget *parent, dns::ScanConfig *config) :
    QDialog(parent),
    ui(new Ui::ActiveConfigDialog),
    m_configDns(config),
    m_customNameserverListModel(new QStringListModel)
{
    ui->setupUi(this);

    m_settings = new QSettings(QDir::currentPath()+"/config/dns.ini", QSettings::IniFormat);
    dns = true;

    /* ... */
    this->m_initWidgets();
    this->m_loadConfigValues();
    this->m_loadCustomNameservers();
    this->m_loadDefaultNameservers();
}

/* for SSL... */
ActiveConfigDialog::ActiveConfigDialog(QWidget *parent, ssl::ScanConfig *config) :
    QDialog(parent),
    ui(new Ui::ActiveConfigDialog),
    m_configSSL(config),
    m_customNameserverListModel(new QStringListModel)
{
    ui->setupUi(this);

    m_settings = new QSettings(QDir::currentPath()+"/config/ssl.ini", QSettings::IniFormat);
    ssl = true;

    this->m_initWidgets();
    this->m_loadConfigValues();
    this->m_loadCustomNameservers();
    this->m_loadDefaultNameservers();
}

/* for IP */
ActiveConfigDialog::ActiveConfigDialog(QWidget *parent, ip::ScanConfig *config) :
    QDialog(parent),
    ui(new Ui::ActiveConfigDialog),
    m_configIp(config),
    m_customNameserverListModel(new QStringListModel)
{
    ui->setupUi(this);

    m_settings = new QSettings(QDir::currentPath()+"/config/ip.ini", QSettings::IniFormat);
    ip = true;

    this->m_initWidgets();
    this->m_loadConfigValues();
    this->m_loadCustomNameservers();
    this->m_loadDefaultNameservers();
}

ActiveConfigDialog::~ActiveConfigDialog(){
    delete ui;
    delete m_customNameserverListModel;
    delete m_settings;
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

void ActiveConfigDialog::m_loadConfigValues(){
    /* get config settings from config file... */
    m_settings->beginGroup("Scan");
    int timeout = m_settings->value("timeout").toInt();
    int threads = m_settings->value("threads").toInt();
    int maxLevel = m_settings->value("maxLevel").toInt();
    bool wildcard = m_settings->value("wildcard").toBool();
    bool useLevel = m_settings->value("useLevel").toInt();
    QString record = m_settings->value("record").toString();
    m_settings->endGroup();

    m_settings->beginGroup("Result");
    bool noDuplicates = m_settings->value("noDuplicates").toBool();
    bool autosaveToProject = m_settings->value("autosaveToProject").toBool();
    m_settings->endGroup();

    m_settings->beginGroup("Current");
    QString nsType = m_settings->value("nameserverType").toString();
    m_settings->endGroup();

    /* set config to the widgets... */
    ui->lineEditTimeout->setText(QString::number(timeout));
    ui->lineEditThreads->setText(QString::number(threads));
    ui->lineEditLevels->setText(QString::number(maxLevel));
    ui->checkBoxWildcards->setChecked(wildcard);
    ui->checkBoxLevel->setChecked(useLevel);
    ui->checkBoxAutosave->setChecked(autosaveToProject);
    ui->checkBoxNoDuplicates->setChecked(noDuplicates);

    if(record == "A")
        ui->radioButtonA->setChecked(true);
    if(record == "AAAA")
        ui->radioButtonAAAA->setChecked(true);
    if(record == "CNAME")
        ui->radioButtonCNAME->setChecked(true);

    if(nsType == "single")
        ui->radioButtonSingleNameserver->setChecked(true);
    if(nsType == "random")
        ui->radioButtonRandomNameservers->setChecked(true);
    if(nsType == "custom")
        ui->radioButtonCustomNameservers->setChecked(true);

}

void ActiveConfigDialog::m_loadDefaultNameservers(){
    m_settings->beginGroup("Default-Nameservers");
    QStringList nameservers = m_settings->allKeys();
    m_settings->endGroup();

    m_settings->beginGroup("Current");
    QString currentNameserver = m_settings->value("nameserver").toString();
    m_settings->endGroup();

    ui->comboBoxSingleNameserver->addItems(nameservers);
    ui->comboBoxSingleNameserver->setCurrentText(currentNameserver);

}

void ActiveConfigDialog::m_loadCustomNameservers(){
    int size = m_settings->beginReadArray("Custom-Nameservers");
    for (int i = 0; i < size; ++i) {
        m_settings->setArrayIndex(i);
        ui->customNameservers->add(m_settings->value("value").toString());
    }
    m_settings->endArray();
}


///
/// saving configurations...
///
void ActiveConfigDialog::m_saveBrute(){
    /* get values... */

    QString thread = ui->lineEditThreads->text();
    QString timeout = ui->lineEditTimeout->text();
    QString maxlevel = ui->lineEditLevels->text();

    bool wildcard = ui->checkBoxWildcards->isChecked();
    bool useLevel = ui->checkBoxLevel->isChecked();
    bool noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    bool autosaveToProject = ui->checkBoxAutosave->isChecked();

    bool recordA = ui->radioButtonA->isChecked();
    bool recordAAAA = ui->radioButtonAAAA->isChecked();
    bool recordCNAME = ui->radioButtonCNAME->isChecked();

    bool nsSingle = ui->radioButtonSingleNameserver->isChecked();
    bool nsRandom = ui->radioButtonRandomNameservers->isChecked();
    bool nsCustom = ui->radioButtonCustomNameservers->isChecked();


    /* saving values to config file... */

    m_settings->beginGroup("Scan");
    m_settings->setValue("threads", thread);
    m_settings->setValue("timeout", timeout);
    m_settings->setValue("maxlevel", maxlevel);
    m_settings->setValue("useLevel", useLevel);
    m_settings->setValue("wildcard", wildcard);
    if(recordA)
        m_settings->setValue("record", "A");
    if(recordAAAA)
        m_settings->setValue("record", "AAAA");
    if(recordCNAME)
        m_settings->setValue("record", "CNAME");
    m_settings->endGroup();

    m_settings->beginGroup("Result");
    m_settings->setValue("noDuplicates", noDuplicates);
    m_settings->setValue("autosaveToProject", autosaveToProject);
    m_settings->endGroup();

    m_settings->beginWriteArray("Custom-Nameservers");
    QStringList customNameservers = m_customNameserverListModel->stringList();
    for (int i = 0; i < customNameservers.length(); ++i) {
        m_settings->setArrayIndex(i);
        m_settings->setValue("value", customNameservers.at(i));
    }
    m_settings->endArray();

    m_settings->beginGroup("Current");
    m_settings->setValue("nameserver", ui->comboBoxSingleNameserver->currentText());
    if(nsSingle)
        m_settings->setValue("nameserverType", "single");
    if(nsRandom)
        m_settings->setValue("nameserverType", "random");
    if(nsCustom)
        m_settings->setValue("nameserverType", "custom");
    m_settings->endGroup();

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
    if(recordCNAME)
        m_configBrute->recordType = QDnsLookup::CNAME;

    if(nsSingle){
        m_configBrute->nameservers.clear();
        m_settings->beginGroup("Default-Nameservers");
        QString nameserver = m_settings->value(ui->comboBoxSingleNameserver->currentText()).toString();
        m_configBrute->nameservers.append(nameserver);
        m_settings->endGroup();
    }
    if(nsRandom){
        m_configBrute->nameservers.clear();
        m_settings->beginGroup("Default-Nameservers");
        QStringList nameservers = m_settings->allKeys();
        foreach(const QString &key, nameservers){
            QString nameserver = m_settings->value(key).toString();
            m_configBrute->nameservers.append(nameserver);
        }
        m_settings->endGroup();
    }
    if(nsCustom){
        m_configBrute->nameservers.clear();
        QStringList nameservers = m_customNameserverListModel->stringList();
        m_configBrute->nameservers = nameservers;
    }
}

void ActiveConfigDialog::m_saveActive(){

}

void ActiveConfigDialog::m_saveDns(){

}

void ActiveConfigDialog::m_saveSSL(){

}

void ActiveConfigDialog::m_saveIp(){

}

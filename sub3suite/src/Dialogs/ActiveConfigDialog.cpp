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
    ui->checkBoxLevel->setChecked(CONFIG_BRUTE.value("useLevel").toInt());
    ui->checkBoxAutosave->setChecked(CONFIG_BRUTE.value("autosaveToProject").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG_BRUTE.value("noDuplicates").toBool());

    QString record = CONFIG_BRUTE.value("record").toString();
    if(record == "A")
        ui->radioButtonA->setChecked(true);
    if(record == "AAAA")
        ui->radioButtonAAAA->setChecked(true);
    if(record == "CNAME")
        ui->radioButtonCNAME->setChecked(true);

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

/* saving configurations... */

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
    if(recordCNAME)
        CONFIG_BRUTE.setValue("record", "CNAME");

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
    if(recordCNAME)
        m_configBrute->recordType = QDnsLookup::CNAME;

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

}

void ActiveConfigDialog::m_saveDns(){

}

void ActiveConfigDialog::m_saveSSL(){

}

void ActiveConfigDialog::m_saveIp(){

}

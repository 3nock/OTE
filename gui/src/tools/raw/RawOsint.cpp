#include "RawOsint.h"
#include "ui_RawOsint.h"
//...
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QClipboard>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCursor>


/*
 * later on use custom icons to indicate this is a string, array or object instead
 * of numbers in the jsonarray..
 */
RawOsint::RawOsint(QWidget *parent) : QDialog(parent), ui(new Ui::RawOsint),
    m_model(new QStandardItemModel),
    m_scanArgs(new ScanArgs)
{
    ui->setupUi(this);
    ///
    /// ...
    ///
    ui->buttonStop->setDisabled(true);
    ///
    /// ...
    ///
    ui->lineEditTarget->setPlaceholderText("Enter target...");
    this->loadEngines();
    ///
    ///...
    ///
    ui->treeView->setModel(m_model);
    m_model->setColumnCount(2);
    m_model->setHorizontalHeaderLabels({"Key", "Value"});
    /*
    m_model->setHorizontalHeaderItem(0, new QStandardItem("Key"));
    m_model->setHorizontalHeaderItem(0, new QStandardItem("Value"));
    */
}
RawOsint::~RawOsint(){
    delete m_scanArgs;
    delete m_model;
    delete ui;
}

void RawOsint::onEnumerationComplete(){
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void RawOsint::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("[hh:mm]  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void RawOsint::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("[hh:mm]  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void RawOsint::onResults(QByteArray reply){
    QJsonDocument document = QJsonDocument::fromJson(reply);
    setJsonText(document);
    setJsonTree(document);
}

void RawOsint::setJsonText(QJsonDocument &results){
    if(!results.isNull() && !results.isEmpty())
        ui->plainTextEdit->appendPlainText(results.toJson());
}

void RawOsint::setJsonTree(QJsonDocument &results){
    if(results.isNull() || results.isEmpty())
        return;

    QStandardItem *item = new QStandardItem(m_scanArgs->module+"("+m_scanArgs->target+")");
    m_model->invisibleRootItem()->appendRow(item);

    if(results.isArray())
        this->treeArray(results.array(), item);
    if(results.isObject())
        this->treeObject(results.object(), item);
}

void RawOsint::treeObject(QJsonObject object, QStandardItem *item){
    QStringList keys = object.keys();
    foreach(const QString &key, keys)
    {
        QJsonValue value = object.value(key);
        if(value.isUndefined())
            continue;

        QStandardItem * objectItem = new QStandardItem(key);

        if(value.isString())
            item->appendRow({objectItem, new QStandardItem(value.toString())});
        if(value.isDouble())
            item->appendRow({objectItem, new QStandardItem(QString::number(value.toDouble()))});
        if(value.isNull())
            item->appendRow({objectItem, new QStandardItem("null")});
        if(value.isBool()){
            if(value.toBool())
                item->appendRow({objectItem, new QStandardItem("true")});
            else
                item->appendRow({objectItem, new QStandardItem("false")});
        }
        if(value.isArray()){
            item->appendRow(objectItem);
            this->treeArray(value.toArray(), objectItem);
        }
        if(value.isObject()){
            item->appendRow(objectItem);
            this->treeObject(value.toObject(), objectItem);
        }
    }
}

void RawOsint::treeArray(QJsonArray array, QStandardItem *item){
    int count = 0;
    foreach(const QJsonValue &value, array)
    {
        if(value.isUndefined())
            continue;

        QStandardItem *arrayItem = new QStandardItem(QString::number(count));

        if(value.isString())
            item->appendRow({arrayItem, new QStandardItem(value.toString())});
        if(value.isDouble())
            item->appendRow({arrayItem, new QStandardItem(QString::number(value.toDouble()))});
        if(value.isNull())
            item->appendRow({arrayItem, new QStandardItem("null")});
        if(value.isBool()){
            if(value.toBool())
                item->appendRow({arrayItem, new QStandardItem("true")});
            else
                item->appendRow({arrayItem, new QStandardItem("false")});
        }
        if(value.isArray()){
            item->appendRow(arrayItem);
            this->treeArray(value.toArray(), arrayItem);
        }
        if(value.isObject()){
            item->appendRow(arrayItem);
            this->treeObject(value.toObject(), arrayItem);
        }
        count++;
    }
}

void RawOsint::loadEngines(){
    ui->comboBoxModule->addItems({"certspotter",
                                 "otx",
                                 "sublist3r",
                                 "threatminer",
                                 "threatcrowd",
                                 "hackertarget",
                                 "dnsbufferoverrun",
                                 "anubis",
                                 "projectdiscovery",
                                 "spyse",
                                 "googleCert",
                                 "omnisint",
                                 "qwant",
                                 "urlscan",
                                 "virustotalapi",
                                 "waybackmachine",
                                 "archivetoday",
                                 "archiveit",
                                 "bgpview",
                                 "binaryedge",
                                 "c99",
                                 "commoncrawl",
                                 "github",
                                 "huntersearch",
                                 "ipinfo",
                                 "mnemonic",
                                 "riskiq",
                                 "robtex",
                                 "securitytrails",
                                 "shodan",
                                 "threatbook",
                                 "whoisxmlapi",
                                 "zetalytics",
                                 "zoomeye",
                                 "ipapi",
                                 "viewDns"});
}

void RawOsint::startScan(){

    m_scanArgs->module = ui->comboBoxModule->currentText();
    m_scanArgs->target = ui->lineEditTarget->text();
    m_scanArgs->raw = true;

    QThread *cThread = new QThread(this);
    switch(ui->comboBoxModule->currentIndex()){
    case 0:
    {
        Certspotter *certspotter = new Certspotter(m_scanArgs);
        certspotter->Enumerator(cThread);
        certspotter->moveToThread(cThread);
        //...
        connect(certspotter, &Certspotter::rawResults, this, &RawOsint::onResults);
        connect(certspotter, &Certspotter::errorLog, this, &RawOsint::onErrorLog);
        connect(certspotter, &Certspotter::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, certspotter, &Certspotter::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 1:
    {
        Otx *otx = new Otx(m_scanArgs);

        otx->Enumerator(cThread);
        otx->moveToThread(cThread);
        //...
        connect(otx, &Otx::rawResults, this, &RawOsint::onResults);
        connect(otx, &Otx::errorLog, this, &RawOsint::onErrorLog);
        connect(otx, &Otx::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, otx, &Otx::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 2:
    {
        Sublist3r *sublist3r = new Sublist3r(m_scanArgs);
        sublist3r->Enumerator(cThread);
        sublist3r->moveToThread(cThread);
        //...
        connect(sublist3r, &Sublist3r::rawResults, this, &RawOsint::onResults);
        connect(sublist3r, &Sublist3r::errorLog, this, &RawOsint::onErrorLog);
        connect(sublist3r, &Sublist3r::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, sublist3r, &Sublist3r::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 3:
    {
        Threatminer *threatminer = new Threatminer(m_scanArgs);
        threatminer->Enumerator(cThread);
        threatminer->moveToThread(cThread);
        //...
        connect(threatminer, &Threatminer::rawResults, this, &RawOsint::onResults);
        connect(threatminer, &Threatminer::errorLog, this, &RawOsint::onErrorLog);
        connect(threatminer, &Threatminer::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, threatminer, &Threatminer::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 4:
    {
        Threatcrowd *threatcrowd = new Threatcrowd(m_scanArgs);
        threatcrowd->Enumerator(cThread);
        threatcrowd->moveToThread(cThread);
        //...
        connect(threatcrowd, &Threatcrowd::rawResults, this, &RawOsint::onResults);
        connect(threatcrowd, &Threatcrowd::errorLog, this, &RawOsint::onErrorLog);
        connect(threatcrowd, &Threatcrowd::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, threatcrowd, &Threatcrowd::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 5:
    {
        Hackertarget *hackertarget = new Hackertarget(m_scanArgs);
        hackertarget->Enumerator(cThread);
        hackertarget->moveToThread(cThread);
        //...
        connect(hackertarget, &Hackertarget::rawResults, this, &RawOsint::onResults);
        connect(hackertarget, &Hackertarget::errorLog, this, &RawOsint::onErrorLog);
        connect(hackertarget, &Hackertarget::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, hackertarget, &Hackertarget::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 6:
    {
        Dnsbufferoverun *dnsbufferoverun = new Dnsbufferoverun(m_scanArgs);
        dnsbufferoverun->Enumerator(cThread);
        dnsbufferoverun->moveToThread(cThread);
        //...
        connect(dnsbufferoverun, &Dnsbufferoverun::rawResults, this, &RawOsint::onResults);
        connect(dnsbufferoverun, &Dnsbufferoverun::errorLog, this, &RawOsint::onErrorLog);
        connect(dnsbufferoverun, &Dnsbufferoverun::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, dnsbufferoverun, &Dnsbufferoverun::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 7:
    {
        Anubis *anubis = new Anubis(m_scanArgs);

        anubis->Enumerator(cThread);
        anubis->moveToThread(cThread);
        //...
        connect(anubis, &Anubis::rawResults, this, &RawOsint::onResults);
        connect(anubis, &Anubis::errorLog, this, &RawOsint::onErrorLog);
        connect(anubis, &Anubis::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, anubis, &Anubis::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 8:
    {
        Projectdiscovery *projectdiscovery = new Projectdiscovery(m_scanArgs);
        projectdiscovery->Enumerator(cThread);
        projectdiscovery->moveToThread(cThread);
        //...
        connect(projectdiscovery, &Projectdiscovery::rawResults, this, &RawOsint::onResults);
        connect(projectdiscovery, &Projectdiscovery::errorLog, this, &RawOsint::onErrorLog);
        connect(projectdiscovery, &Projectdiscovery::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, projectdiscovery, &Projectdiscovery::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 9:
    {
        Spyse *spyse = new Spyse(m_scanArgs);
        spyse->Enumerator(cThread);
        spyse->moveToThread(cThread);
        //...
        connect(spyse, &Spyse::rawResults, this, &RawOsint::onResults);
        connect(spyse, &Spyse::errorLog, this, &RawOsint::onErrorLog);
        connect(spyse, &Spyse::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, spyse, &Spyse::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 10:
    {
        GoogleCert *googlecert = new GoogleCert(m_scanArgs);
        googlecert->Enumerator(cThread);
        googlecert->moveToThread(cThread);
        //...
        connect(googlecert, &GoogleCert::rawResults, this, &RawOsint::onResults);
        connect(googlecert, &GoogleCert::errorLog, this, &RawOsint::onErrorLog);
        connect(googlecert, &GoogleCert::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, googlecert, &GoogleCert::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 11:
    {
        Omnisint *omnisint = new Omnisint(m_scanArgs);
        omnisint->Enumerator(cThread);
        omnisint->moveToThread(cThread);
        //...
        connect(omnisint, &Omnisint::rawResults, this, &RawOsint::onResults);
        connect(omnisint, &Omnisint::errorLog, this, &RawOsint::onErrorLog);
        connect(omnisint, &Omnisint::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, omnisint, &Omnisint::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 12:
    {
        Qwant *qwant = new Qwant(m_scanArgs);

        qwant->Enumerator(cThread);
        qwant->moveToThread(cThread);
        //...
        connect(qwant, &Qwant::rawResults, this, &RawOsint::onResults);
        connect(qwant, &Qwant::errorLog, this, &RawOsint::onErrorLog);
        connect(qwant, &Qwant::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, qwant, &Qwant::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 13:
    {
        VirusTotal *virustotal = new VirusTotal(m_scanArgs);
        virustotal->Enumerator(cThread);
        virustotal->moveToThread(cThread);
        //...
        connect(virustotal, &VirusTotal::rawResults, this, &RawOsint::onResults);
        connect(virustotal, &VirusTotal::errorLog, this, &RawOsint::onErrorLog);
        connect(virustotal, &VirusTotal::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, virustotal, &VirusTotal::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 14:
    {
        Urlscan *urlscan = new Urlscan(m_scanArgs);
        urlscan->Enumerator(cThread);
        urlscan->moveToThread(cThread);
        //...
        connect(urlscan, &Urlscan::rawResults, this, &RawOsint::onResults);
        connect(urlscan, &Urlscan::errorLog, this, &RawOsint::onErrorLog);
        connect(urlscan, &Urlscan::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, urlscan, &Urlscan::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 15:
    {
        Waybackmachine *waybackmachine = new Waybackmachine(m_scanArgs);
        waybackmachine->Enumerator(cThread);
        waybackmachine->moveToThread(cThread);
        //...
        connect(waybackmachine, &Waybackmachine::rawResults, this, &RawOsint::onResults);
        connect(waybackmachine, &Waybackmachine::errorLog, this, &RawOsint::onErrorLog);
        connect(waybackmachine, &Waybackmachine::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, waybackmachine, &Waybackmachine::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 16:
    {
        ArchiveToday *archivetoday = new ArchiveToday(m_scanArgs);
        archivetoday->Enumerator(cThread);
        archivetoday->moveToThread(cThread);
        //...
        connect(archivetoday, &ArchiveToday::rawResults, this, &RawOsint::onResults);
        connect(archivetoday, &ArchiveToday::errorLog, this, &RawOsint::onErrorLog);
        connect(archivetoday, &ArchiveToday::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, archivetoday, &ArchiveToday::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 17:
    {
        ArchiveIt *archiveit = new ArchiveIt(m_scanArgs);
        archiveit->Enumerator(cThread);
        archiveit->moveToThread(cThread);
        //...
        connect(archiveit, &ArchiveIt::rawResults, this, &RawOsint::onResults);
        connect(archiveit, &ArchiveIt::errorLog, this, &RawOsint::onErrorLog);
        connect(archiveit, &ArchiveIt::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, archiveit, &ArchiveIt::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 18:
    {
        Bgpview *bgpview = new Bgpview(m_scanArgs);
        bgpview->Enumerator(cThread);
        bgpview->moveToThread(cThread);
        //...
        connect(bgpview, &Bgpview::rawResults, this, &RawOsint::onResults);
        connect(bgpview, &Bgpview::errorLog, this, &RawOsint::onErrorLog);
        connect(bgpview, &Bgpview::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, bgpview, &Bgpview::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 19:
    {
        BinaryEdge *binaryedge = new BinaryEdge(m_scanArgs);
        binaryedge->Enumerator(cThread);
        binaryedge->moveToThread(cThread);
        //...
        connect(binaryedge, &BinaryEdge::rawResults, this, &RawOsint::onResults);
        connect(binaryedge, &BinaryEdge::errorLog, this, &RawOsint::onErrorLog);
        connect(binaryedge, &BinaryEdge::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, binaryedge, &BinaryEdge::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 20:
    {
        C99 *c99 = new C99(m_scanArgs);
        c99->Enumerator(cThread);
        c99->moveToThread(cThread);
        //...
        connect(c99, &C99::rawResults, this, &RawOsint::onResults);
        connect(c99, &C99::errorLog, this, &RawOsint::onErrorLog);
        connect(c99, &C99::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, c99, &C99::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 21:
    {
        CommonCrawl *commonCrawl = new CommonCrawl(m_scanArgs);
        commonCrawl->Enumerator(cThread);
        commonCrawl->moveToThread(cThread);
        //...
        connect(commonCrawl, &CommonCrawl::rawResults, this, &RawOsint::onResults);
        connect(commonCrawl, &CommonCrawl::errorLog, this, &RawOsint::onErrorLog);
        connect(commonCrawl, &CommonCrawl::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, commonCrawl, &CommonCrawl::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 22:
    {
        Github *github = new Github(m_scanArgs);
        github->Enumerator(cThread);
        github->moveToThread(cThread);
        //...
        connect(github, &Github::rawResults, this, &RawOsint::onResults);
        connect(github, &Github::errorLog, this, &RawOsint::onErrorLog);
        connect(github, &Github::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, github, &Github::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 23:
    {
        HunterSearch *huntersearch = new HunterSearch(m_scanArgs);
        huntersearch->Enumerator(cThread);
        huntersearch->moveToThread(cThread);
        //...
        connect(huntersearch, &HunterSearch::rawResults, this, &RawOsint::onResults);
        connect(huntersearch, &HunterSearch::errorLog, this, &RawOsint::onErrorLog);
        connect(huntersearch, &HunterSearch::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, huntersearch, &HunterSearch::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 24:
    {
        IpInfo *ipinfo = new IpInfo(m_scanArgs);
        ipinfo->Enumerator(cThread);
        ipinfo->moveToThread(cThread);
        //...
        connect(ipinfo, &IpInfo::rawResults, this, &RawOsint::onResults);
        connect(ipinfo, &IpInfo::errorLog, this, &RawOsint::onErrorLog);
        connect(ipinfo, &IpInfo::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, ipinfo, &IpInfo::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 25:{
        Mnemonic *mnemonic = new Mnemonic(m_scanArgs);
        mnemonic->Enumerator(cThread);
        mnemonic->moveToThread(cThread);
        //...
        connect(mnemonic, &Mnemonic::rawResults, this, &RawOsint::onResults);
        connect(mnemonic, &Mnemonic::errorLog, this, &RawOsint::onErrorLog);
        connect(mnemonic, &Mnemonic::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, mnemonic, &Mnemonic::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 26:
    {
        RiskIq *riskiq = new RiskIq(m_scanArgs);
        riskiq->Enumerator(cThread);
        riskiq->moveToThread(cThread);
        //...
        connect(riskiq, &RiskIq::rawResults, this, &RawOsint::onResults);
        connect(riskiq, &RiskIq::errorLog, this, &RawOsint::onErrorLog);
        connect(riskiq, &RiskIq::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, riskiq, &RiskIq::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 27:
    {
        Robtex *robtex = new Robtex(m_scanArgs);
        robtex->Enumerator(cThread);
        robtex->moveToThread(cThread);
        //...
        connect(robtex, &Robtex::rawResults, this, &RawOsint::onResults);
        connect(robtex, &Robtex::errorLog, this, &RawOsint::onErrorLog);
        connect(robtex, &Robtex::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, robtex, &Robtex::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 28:
    {
        SecurityTrails *securitytrails = new SecurityTrails(m_scanArgs);
        securitytrails->Enumerator(cThread);
        securitytrails->moveToThread(cThread);
        //...
        connect(securitytrails, &SecurityTrails::rawResults, this, &RawOsint::onResults);
        connect(securitytrails, &SecurityTrails::errorLog, this, &RawOsint::onErrorLog);
        connect(securitytrails, &SecurityTrails::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, securitytrails, &SecurityTrails::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 29:
    {
        Shodan *shodan = new Shodan(m_scanArgs);
        shodan->Enumerator(cThread);
        shodan->moveToThread(cThread);
        //...
        connect(shodan, &Shodan::rawResults, this, &RawOsint::onResults);
        connect(shodan, &Shodan::errorLog, this, &RawOsint::onErrorLog);
        connect(shodan, &Shodan::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, shodan, &Shodan::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 30:
    {
        ThreatBook *threatbook = new ThreatBook(m_scanArgs);
        threatbook->Enumerator(cThread);
        threatbook->moveToThread(cThread);
        //...
        connect(threatbook, &ThreatBook::rawResults, this, &RawOsint::onResults);
        connect(threatbook, &ThreatBook::errorLog, this, &RawOsint::onErrorLog);
        connect(threatbook, &ThreatBook::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, threatbook, &ThreatBook::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 31:
    {
        WhoisXmlApi *whoisxmlapi = new WhoisXmlApi(m_scanArgs);
        whoisxmlapi->Enumerator(cThread);
        whoisxmlapi->moveToThread(cThread);
        //...
        connect(whoisxmlapi, &WhoisXmlApi::rawResults, this, &RawOsint::onResults);
        connect(whoisxmlapi, &WhoisXmlApi::errorLog, this, &RawOsint::onErrorLog);
        connect(whoisxmlapi, &WhoisXmlApi::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, whoisxmlapi, &WhoisXmlApi::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 32:
    {
        ZETAlytics *zetalytics = new ZETAlytics(m_scanArgs);
        zetalytics->Enumerator(cThread);
        zetalytics->moveToThread(cThread);
        //...
        connect(zetalytics, &ZETAlytics::rawResults, this, &RawOsint::onResults);
        connect(zetalytics, &ZETAlytics::errorLog, this, &RawOsint::onErrorLog);
        connect(zetalytics, &ZETAlytics::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, zetalytics, &ZETAlytics::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 33:
    {
        ZoomEye *zoomeye = new ZoomEye(m_scanArgs);
        zoomeye->Enumerator(cThread);
        zoomeye->moveToThread(cThread);
        //...
        connect(zoomeye, &ZoomEye::rawResults, this, &RawOsint::onResults);
        connect(zoomeye, &ZoomEye::errorLog, this, &RawOsint::onErrorLog);
        connect(zoomeye, &ZoomEye::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, zoomeye, &ZoomEye::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 34:
    {
        IpApi *ipapi = new IpApi(m_scanArgs);
        ipapi->Enumerator(cThread);
        ipapi->moveToThread(cThread);
        //...
        connect(ipapi, &IpApi::rawResults, this, &RawOsint::onResults);
        connect(ipapi, &IpApi::errorLog, this, &RawOsint::onErrorLog);
        connect(ipapi, &IpApi::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, ipapi, &IpApi::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    case 35:
    {
        ViewDns *viewdns = new ViewDns(m_scanArgs);
        viewdns->Enumerator(cThread);
        viewdns->moveToThread(cThread);
        //...
        connect(viewdns, &ViewDns::rawResults, this, &RawOsint::onResults);
        connect(viewdns, &ViewDns::errorLog, this, &RawOsint::onErrorLog);
        connect(viewdns, &ViewDns::infoLog, this, &RawOsint::onInfoLog);
        connect(cThread, &QThread::finished, this, &RawOsint::onEnumerationComplete);
        connect(cThread, &QThread::finished, viewdns, &ViewDns::deleteLater);
        connect(cThread, &QThread::finished, cThread, &QThread::deleteLater);
        //...
        cThread->start();
        break;
    }
    default:
        ui->buttonStart->setEnabled(true);
        ui->buttonStop->setDisabled(true);
        break;
    }
}

void RawOsint::on_buttonStart_clicked(){
    ///
    /// checks...
    ///
    if(ui->lineEditTarget->text().isEmpty()){
        QMessageBox::warning(this, "Error!", "Please Target Domain For Enumerations!");
        return;
    }

    ui->buttonStart->setDisabled(true);
    ui->buttonStop->setEnabled(true);
    this->startScan();
}

void RawOsint::on_buttonClear_clicked(){
    ui->plainTextEdit->clear();
}

void RawOsint::on_buttonCopy_clicked(){
    ///
    /// checks...
    ///
    if(ui->plainTextEdit->toPlainText().isEmpty())
        return;

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->plainTextEdit->toPlainText());
}

void RawOsint::on_buttonSave_clicked(){
    ///
    /// checks...
    ///
    if(ui->plainTextEdit->toPlainText().isEmpty())
        return;

    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty()){
        return;
    }
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        file.write(ui->plainTextEdit->toPlainText().toUtf8());
        file.close();
    }
}

void RawOsint::on_buttonNext_clicked(){
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive->isChecked()){
        flags |= QTextDocument::FindCaseSensitively;
    }

    this->find(ui->lineEditFind->text(), flags);
}

void RawOsint::on_buttonPrev_clicked(){
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive->isChecked()){
        flags |= QTextDocument::FindCaseSensitively;
    }
    flags |= QTextDocument::FindBackward;

    this->find(ui->lineEditFind->text(), flags);
}

void RawOsint::find(QString searchTerm, QTextDocument::FindFlags flags){
    if(ui->plainTextEdit->find(searchTerm, flags)){
        ui->lineEditFind->setStyleSheet("color: white");
    }else{
        ui->lineEditFind->setStyleSheet("color: rgb(255, 86, 80);");
        return;
    }
}

void RawOsint::on_lineEditFind_textEdited(const QString &searchTerm){
    ///
    /// set cursor to the begining of the document...
    ///
    QTextCursor currentCursorPosition;
    currentCursorPosition.setPosition(QTextCursor::Start);
    ui->plainTextEdit->setTextCursor(currentCursorPosition);
    ///
    /// get option flags...
    ///
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive->isChecked()){
        flags |= QTextDocument::FindCaseSensitively;
    }

    this->find(searchTerm, flags);
}

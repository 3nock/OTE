#include "DocumentationDialog.h"
#include "ui_DocumentationDialog.h"


DocumentationDialog::DocumentationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocumentationDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/doc.png"));
    ui->textBrowser->setProperty("documentation", true);

    /* text broswer */
    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser->setSearchPaths(QStringList() << QCoreApplication::applicationDirPath()+"/doc"
                                                  << QCoreApplication::applicationDirPath()+"/doc/tools"
                                                  << QCoreApplication::applicationDirPath()+"/doc/enums");

    this->setWindowTitle("Documentation");
    ui->textBrowser->setSource(QUrl("index.html"));
}

DocumentationDialog::DocumentationDialog(TOOL engine, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocumentationDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/doc.png"));
    ui->textBrowser->setProperty("documentation", true);

    /* text broswer */
    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser->setSearchPaths(QStringList() << QCoreApplication::applicationDirPath()+"/doc/tools"
                                                  << QCoreApplication::applicationDirPath()+"/doc");

    switch(engine){
    case TOOL::OSINT:
        this->setWindowTitle("About OSINT TOOL");
        ui->textBrowser->setSource(QUrl("tools/osint.html"));
        break;
    case TOOL::RAW:
        this->setWindowTitle("About RAW TOOL");
        ui->textBrowser->setSource(QUrl("tools/raw.html"));
        break;
    case TOOL::BRUTE:
        this->setWindowTitle("About BRUTEFORCE TOOL");
        ui->textBrowser->setSource(QUrl("tools/brute.html"));
        break;
    case TOOL::HOST:
        this->setWindowTitle("About HOSTNAME TOOL");
        ui->textBrowser->setSource(QUrl("tools/host.html"));
        break;
    case TOOL::IP:
        this->setWindowTitle("About IP-ADDRESS TOOL");
        ui->textBrowser->setSource(QUrl("tools/ip.html"));
        break;
    case TOOL::DNS:
        this->setWindowTitle("About DNS TOOL");
        ui->textBrowser->setSource(QUrl("tools/dns.html"));
        break;
    case TOOL::SSL:
        this->setWindowTitle("About SSL TOOL");
        ui->textBrowser->setSource(QUrl("tools/ssl.html"));
        break;
    case TOOL::URL:
        this->setWindowTitle("About URL TOOL");
        ui->textBrowser->setSource(QUrl("tools/url.html"));
        break;
    default:
        break;
    }
}

DocumentationDialog::DocumentationDialog(ENUMERATOR enumerator, QWidget *parent):
    QDialog(parent),
    ui(new Ui::DocumentationDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/doc.png"));
    ui->textBrowser->setProperty("documentation", true);

    /* text broswer */
    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser->setSearchPaths(QStringList() << QCoreApplication::applicationDirPath()+"/doc/enums"
                                                  << QCoreApplication::applicationDirPath()+"/doc");

    switch(enumerator){
    case ENUMERATOR::IP:
        this->setWindowTitle("About IP Enumerator");
        ui->textBrowser->setSource(QUrl("enums/ip.html"));
        break;
    case ENUMERATOR::ASN:
        this->setWindowTitle("About ASN Enumerator");
        ui->textBrowser->setSource(QUrl("enums/asn.html"));
        break;
    case ENUMERATOR::CIDR:
        this->setWindowTitle("About CIDR Enumerator");
        ui->textBrowser->setSource(QUrl("enums/cidr.html"));
        break;
    case ENUMERATOR::NS:
        this->setWindowTitle("About NS Enumerator");
        ui->textBrowser->setSource(QUrl("enums/ns.html"));
        break;
    case ENUMERATOR::MX:
        this->setWindowTitle("About MX Enumerator");
        ui->textBrowser->setSource(QUrl("enums/mx.html"));
        break;
    case ENUMERATOR::SSL:
        this->setWindowTitle("About SSL Enumerator");
        ui->textBrowser->setSource(QUrl("enums/ssl.html"));
        break;
    case ENUMERATOR::EMAIL:
        this->setWindowTitle("About Email Enumerator");
        ui->textBrowser->setSource(QUrl("enums/email.html"));
        break;
    }
}

DocumentationDialog::~DocumentationDialog(){
    delete ui;
}

void DocumentationDialog::on_buttonOk_clicked(){
    accept();
}

void DocumentationDialog::on_buttonBack_clicked(){
    ui->textBrowser->backward();
}

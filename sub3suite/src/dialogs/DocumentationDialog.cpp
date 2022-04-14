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
                                                  << QCoreApplication::applicationDirPath()+"/doc/engines"
                                                  << QCoreApplication::applicationDirPath()+"/doc/enums");

    this->setWindowTitle("Documentation");
    ui->textBrowser->setSource(QUrl("index.html"));
}

DocumentationDialog::DocumentationDialog(ENGINE engine, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocumentationDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/doc.png"));
    ui->textBrowser->setProperty("documentation", true);

    /* text broswer */
    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser->setSearchPaths(QStringList() << QCoreApplication::applicationDirPath()+"/doc/engines"
                                                  << QCoreApplication::applicationDirPath()+"/doc");

    switch(engine){
    case ENGINE::OSINT:
        this->setWindowTitle("About OSINT");
        ui->textBrowser->setSource(QUrl("engines/osint.html"));
        break;
    case ENGINE::RAW:
        this->setWindowTitle("About RAW");
        ui->textBrowser->setSource(QUrl("engines/raw.html"));
        break;
    case ENGINE::BRUTE:
        this->setWindowTitle("About BRUTE");
        ui->textBrowser->setSource(QUrl("engines/brute.html"));
        break;
    case ENGINE::ACTIVE:
        this->setWindowTitle("About ACTIVE");
        ui->textBrowser->setSource(QUrl("engines/active.html"));
        break;
    case ENGINE::DNS:
        this->setWindowTitle("About DNS");
        ui->textBrowser->setSource(QUrl("engines/dns.html"));
        break;
    case ENGINE::SSL:
        this->setWindowTitle("About SSL");
        ui->textBrowser->setSource(QUrl("engines/ssl.html"));
        break;
    case ENGINE::URL:
        this->setWindowTitle("About URL");
        ui->textBrowser->setSource(QUrl("engines/url.html"));
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

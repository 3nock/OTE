#include "DocumentationDialog.h"
#include "ui_DocumentationDialog.h"

#include <QFile>


DocumentationDialog::DocumentationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocumentationDialog)
{
    ui->setupUi(this);

    /* text broswer */
    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser->setSearchPaths(QStringList() << ":/documentation/res/documentation/engines"
                                                  << ":/documentation/res/documentation");

    this->general_documentation();
}

DocumentationDialog::DocumentationDialog(ENGINE engine, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocumentationDialog)
{
    ui->setupUi(this);

    /* text broswer */
    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser->setSearchPaths(QStringList() << ":/documentation/res/documentation/engines"
                                                  << ":/documentation/res/documentation");

    switch(engine){
    case ENGINE::OSINT:
        this->about_osint();
        break;
    case ENGINE::RAW:
        this->about_raw();
        break;
    case ENGINE::BRUTE:
        this->about_brute();
        break;
    case ENGINE::ACTIVE:
        this->about_active();
        break;
    case ENGINE::DNS:
        this->about_dns();
        break;
    case ENGINE::CERT:
        this->about_ssl();
        break;
    default:
        break;
    }
}

DocumentationDialog::~DocumentationDialog(){
    delete ui;
}

void DocumentationDialog::general_documentation(){
    this->setWindowTitle("Documentation");

    ui->textBrowser->setSource(QUrl("index.html"));
}

void DocumentationDialog::about_osint(){
    this->setWindowTitle("About OSINT");

    ui->textBrowser->setSource(QUrl("engines/osint.html"));
}

void DocumentationDialog::about_raw(){
    this->setWindowTitle("About RAW");

    ui->textBrowser->setSource(QUrl("engines/raw.html"));
}

void DocumentationDialog::about_brute(){
    this->setWindowTitle("About BRUTE");

    ui->textBrowser->setSource(QUrl("engines/brute.html"));
}

void DocumentationDialog::about_active(){
    this->setWindowTitle("About ACTIVE");

    ui->textBrowser->setSource(QUrl("engines/active.html"));
}

void DocumentationDialog::about_dns(){
    this->setWindowTitle("About DNS");

    ui->textBrowser->setSource(QUrl("engines/dns.html"));
}

void DocumentationDialog::about_ssl(){
    this->setWindowTitle("About SSL");

    ui->textBrowser->setSource(QUrl("engines/ssl.html"));
}

void DocumentationDialog::about_ip(){
    this->setWindowTitle("About IP");

    ui->textBrowser->setSource(QUrl("engines/ip.html"));
}

void DocumentationDialog::on_buttonOk_clicked(){
    accept();
}

void DocumentationDialog::on_buttonBack_clicked(){
    ui->textBrowser->backward();
}

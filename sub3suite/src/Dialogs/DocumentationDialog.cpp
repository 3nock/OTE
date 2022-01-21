#include "DocumentationDialog.h"
#include "ui_DocumentationDialog.h"

#include <QFile>


DocumentationDialog::DocumentationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocumentationDialog)
{
    ui->setupUi(this);

    this->general_documentation();

    /* text broswer */
    ui->textBrowser->setOpenExternalLinks(true);
}

DocumentationDialog::DocumentationDialog(ENGINE engine, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocumentationDialog)
{
    ui->setupUi(this);

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

    /* text broswer */
    ui->textBrowser->setOpenExternalLinks(true);
}

DocumentationDialog::~DocumentationDialog(){
    delete ui;
}

void DocumentationDialog::general_documentation(){
    this->setWindowTitle("Documentation");

    QFile file(":/documentation/res/documentation/index.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->textBrowser->append(file.readAll());
        file.close();
    }
}

void DocumentationDialog::about_osint(){
    this->setWindowTitle("About OSINT");

    QFile file(":/documentation/res/documentation/osint.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->textBrowser->append(file.readAll());
        file.close();
    }
}

void DocumentationDialog::about_raw(){
    this->setWindowTitle("About RAW");

    QFile file(":/documentation/res/documentation/raw.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->textBrowser->append(file.readAll());
        file.close();
    }
}

void DocumentationDialog::about_brute(){
    this->setWindowTitle("About BRUTE");

    QFile file(":/documentation/res/documentation/brute.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->textBrowser->append(file.readAll());
        file.close();
    }
}

void DocumentationDialog::about_active(){
    this->setWindowTitle("About ACTIVE");

    QFile file(":/documentation/res/documentation/active.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->textBrowser->append(file.readAll());
        file.close();
    }
}

void DocumentationDialog::about_dns(){
    this->setWindowTitle("About DNS");

    QFile file(":/documentation/res/documentation/dns.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->textBrowser->append(file.readAll());
        file.close();
    }
}

void DocumentationDialog::about_ssl(){
    this->setWindowTitle("About SSL");

    QFile file(":/documentation/res/documentation/ssl.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->textBrowser->append(file.readAll());
        file.close();
    }
}

void DocumentationDialog::about_ip(){
    this->setWindowTitle("About IP");

    QFile file(":/documentation/res/documentation/ip.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->textBrowser->append(file.readAll());
        file.close();
    }
}

void DocumentationDialog::on_buttonOk_clicked(){
    accept();
}

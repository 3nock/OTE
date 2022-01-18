#include "AboutEngineDialog.h"
#include "ui_AboutEngineDialog.h"

#include <QFile>


AboutEngineDialog::AboutEngineDialog(ENGINE engine, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutEngineDialog)
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
    case ENGINE::IP:
        this->about_ip();
        break;
    default:
        break;
    }

    /* text broswer */
    ui->textBrowser->setOpenExternalLinks(true);
}
AboutEngineDialog::~AboutEngineDialog(){
    delete ui;
}

void AboutEngineDialog::about_osint(){
    this->setWindowTitle("About OSINT");

    QFile file(":/files/res/files/about/osint.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->textBrowser->append(file.readAll());
        file.close();
    }
}

void AboutEngineDialog::about_raw(){
    this->setWindowTitle("About RAW");

    QFile file(":/files/res/files/about/raw.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->textBrowser->append(file.readAll());
        file.close();
    }
}

void AboutEngineDialog::about_brute(){
    this->setWindowTitle("About BRUTE");

    QFile file(":/files/res/files/about/brute.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->textBrowser->append(file.readAll());
        file.close();
    }
}

void AboutEngineDialog::about_active(){
    this->setWindowTitle("About ACTIVE");

    QFile file(":/files/res/files/about/active.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->textBrowser->append(file.readAll());
        file.close();
    }
}

void AboutEngineDialog::about_dns(){
    this->setWindowTitle("About DNS");

    QFile file(":/files/res/files/about/dns.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->textBrowser->append(file.readAll());
        file.close();
    }
}

void AboutEngineDialog::about_ssl(){
    this->setWindowTitle("About SSL");

    QFile file(":/files/res/files/about/ssl.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->textBrowser->append(file.readAll());
        file.close();
    }
}

void AboutEngineDialog::about_ip(){
    this->setWindowTitle("About IP");

    QFile file(":/files/res/files/about/ip.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->textBrowser->append(file.readAll());
        file.close();
    }
}

void AboutEngineDialog::on_buttonOk_clicked(){
    accept();
}

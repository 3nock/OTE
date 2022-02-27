#ifndef DNSMODEL_H
#define DNSMODEL_H

#include <QStandardItem>
#include <QJsonObject>
#include <QSet>

/*
 * TODO:
 *      for now it is poorly designed
 */

namespace s3s_struct {
struct DNS {
    QString dns;

    QSet<QString> A;
    QSet<QString> AAAA;
    QSet<QString> CNAME;
    QSet<QString> NS;
    QSet<QString> MX;
    QSet<QString> TXT;
    QSet<QStringList> SRV;
};
}

namespace s3s_item {

class DNS: public QStandardItem {
public:
    DNS(): QStandardItem(),
        A(new QStandardItem("A")),
        AAAA(new QStandardItem("AAAA")),
        CNAME(new QStandardItem("CNAME")),
        NS(new QStandardItem("NS")),
        MX(new QStandardItem("MX")),
        TXT(new QStandardItem("TXT")),
        SRV(new QStandardItem("SRV"))
    {
        this->setForeground(Qt::white);
        this->setIcon(QIcon(":/img/res/icons/folder.png"));

        A->setForeground(Qt::white);
        AAAA->setForeground(Qt::white);
        CNAME->setForeground(Qt::white);
        NS->setForeground(Qt::white);
        MX->setForeground(Qt::white);
        TXT->setForeground(Qt::white);
        SRV->setForeground(Qt::white);

        A->setIcon(QIcon(":/img/res/icons/folder2.png"));
        AAAA->setIcon(QIcon(":/img/res/icons/folder2.png"));
        CNAME->setIcon(QIcon(":/img/res/icons/folder2.png"));
        NS->setIcon(QIcon(":/img/res/icons/folder2.png"));
        MX->setIcon(QIcon(":/img/res/icons/folder2.png"));
        TXT->setIcon(QIcon(":/img/res/icons/folder2.png"));
        SRV->setIcon(QIcon(":/img/res/icons/folder2.png"));
    }
    ~DNS()
    {
    }

public:
    QStandardItem *A;
    QStandardItem *AAAA;
    QStandardItem *CNAME;
    QStandardItem *NS;
    QStandardItem *MX;
    QStandardItem *TXT;
    QStandardItem *SRV;
    bool _A = false;
    bool _AAAA = false;
    bool _CNAME = false;
    bool _NS = false;
    bool _MX = false;
    bool _TXT = false;
    bool _SRV = false;

    void setValues(const s3s_struct::DNS &dns){
        /* append to DNS item */
        if(!dns.A.isEmpty() && !_A){
            this->appendRow(A);
            _A = true;
        }
        if(!dns.AAAA.isEmpty() && !_AAAA){
            this->appendRow(AAAA);
            _AAAA = true;
        }
        if(!dns.CNAME.isEmpty() && !_CNAME){
            this->appendRow(CNAME);
            _CNAME = true;
        }
        if(!dns.NS.isEmpty() && !_NS){
            this->appendRow(NS);
            _NS = true;
        }
        if(!dns.MX.isEmpty() && !_MX){
            this->appendRow(MX);
            _MX = true;
        }
        if(!dns.TXT.isEmpty() && !_TXT){
            this->appendRow(TXT);
            _TXT = true;
        }
        if(!dns.SRV.isEmpty() && !_SRV){
            this->appendRow(SRV);
            _SRV = true;
        }

        this->setText(dns.dns);

        /* enter results */
        int count = 0;
        foreach(const QString &a, dns.A){
            A->appendRow({new QStandardItem(QString::number(count)),
                          new QStandardItem(a)});
            count++;
        }
        count = 0;
        foreach(const QString &aaaa, dns.AAAA){
            AAAA->appendRow({new QStandardItem(QString::number(count)),
                             new QStandardItem(aaaa)});
            count++;
        }
        count = 0;
        foreach(const QString &cname, dns.CNAME){
            CNAME->appendRow({new QStandardItem(QString::number(count)),
                              new QStandardItem(cname)});
            count++;
        }
        count = 0;
        foreach(const QString &ns, dns.NS){
            NS->appendRow({new QStandardItem(QString::number(count)),
                           new QStandardItem(ns)});
            count++;
        }
        count = 0;
        foreach(const QString &mx, dns.MX){
            MX->appendRow({new QStandardItem(QString::number(count)),
                           new QStandardItem(mx)});
            count++;
        }
        count = 0;
        foreach(const QString &txt, dns.TXT){
            TXT->appendRow({new QStandardItem(QString::number(count)),
                            new QStandardItem(txt)});
            count++;
        }
        foreach(const QStringList &srv, dns.SRV)
            SRV->appendRow({new QStandardItem(srv.at(0)),
                           new QStandardItem(srv.at(1)),
                           new QStandardItem(srv.at(2))});
    }
};
}

s3s_struct::DNS dns_to_struct(s3s_item::DNS*);

QJsonObject dns_to_json(s3s_item::DNS*);

void json_to_dns(const QJsonObject&, s3s_item::DNS*);

#endif // DNSMODEL_H

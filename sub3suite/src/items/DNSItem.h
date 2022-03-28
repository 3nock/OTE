#ifndef DNSITEM_H
#define DNSITEM_H

#include <QStandardItem>
#include <QJsonObject>
#include <QDate>
#include <QSet>

#define JSON_ARRAY "arr"
#define JSON_OBJECT "obj"

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

        A->setWhatsThis(JSON_ARRAY);
        AAAA->setWhatsThis(JSON_ARRAY);
        CNAME->setWhatsThis(JSON_ARRAY);
        NS->setWhatsThis(JSON_ARRAY);
        MX->setWhatsThis(JSON_ARRAY);
        TXT->setWhatsThis(JSON_ARRAY);
        SRV->setWhatsThis(JSON_ARRAY);

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

    bool A_appended = false;
    bool AAAA_appended = false;
    bool CNAME_appended = false;
    bool TXT_appended = false;
    bool NS_appended = false;
    bool MX_appended = false;
    bool SRV_appended = false;

    /* summary */
    QString last_modified;
    QString comment;

    void update_items(){
        if(A->rowCount() && !A_appended){
            this->appendRow(A); A_appended = true;
        }
        if(AAAA->rowCount() && !AAAA_appended){
            this->appendRow(AAAA); AAAA_appended = true;
        }
        if(CNAME->rowCount() && !CNAME_appended){
            this->appendRow(CNAME);  CNAME_appended = true;
        }
        if(TXT->rowCount() && !TXT_appended){
            this->appendRow(TXT);  TXT_appended = true;
        }
        if(NS->rowCount() && !NS_appended){
            this->appendRow(NS);  NS_appended = true;
        }
        if(MX->rowCount() && !MX_appended){
            this->appendRow(MX);  MX_appended = true;
        }
        if(SRV->rowCount() && !SRV_appended){
            this->appendRow(SRV);  SRV_appended = true;
        }
    }

    void addSRV(const s3s_struct::DNS &dns){
        foreach(const QStringList &srv, dns.SRV)
            SRV->appendRow({new QStandardItem(srv.at(0)),
                           new QStandardItem(srv.at(1)),
                           new QStandardItem(srv.at(2))});

        this->update_items();

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }

    void setValues(const s3s_struct::DNS &dns){
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
        foreach(const QStringList &srv, dns.SRV){
            SRV->appendRow({new QStandardItem(srv.at(0)),
                           new QStandardItem(srv.at(1)),
                           new QStandardItem(srv.at(2))});
        }

        this->update_items();

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }
};
}

s3s_struct::DNS dns_to_struct(s3s_item::DNS*);

QJsonObject dns_to_json(s3s_item::DNS*);

void json_to_dns(const QJsonObject&, s3s_item::DNS*);

#endif // DNSITEM_H

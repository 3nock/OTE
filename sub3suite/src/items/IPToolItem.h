#ifndef IPTOOLITEM_H
#define IPTOOLITEM_H


#include "src/utils/utils.h"

#include <QStandardItem>
#include <QDate>
#include <QSet>

#define JSON_ARRAY "arr"

namespace s3s_struct {
struct IPTool {
    QString ip;
    QSet<QString> ports;
};
}

namespace s3s_item {

class IPTool: public QStandardItem {

public:
    IPTool(): QStandardItem(),
        ports(new QStandardItem)
    {
    }
    ~IPTool()
    {
    }

public:
    QStandardItem *ports;
    QSet<QString> set_ports;

    /* summary */
    QString last_modified;
    QString comment;

    void setValues(const s3s_struct::IPTool &ip){
        this->setText(ip.ip);

        /* open ports */
        foreach(const QString &port, ip.ports){
            if(!set_ports.contains(port)){
                set_ports.insert(port);
                ports->setText(ports->text()+port+" ");
            }
        }

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }

    void setValues(const QString &ip, const QString &port){
        this->setText(ip);

        /* open ports */
        if(!set_ports.contains(port)){
            set_ports.insert(port);
            ports->setText(ports->text()+port+" ");
        }

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }

    void addPort(const QString &port){
        /* open ports */
        if(!set_ports.contains(port)){
            set_ports.insert(port);
            ports->setText(ports->text()+port+" ");
        }

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }
};
}

s3s_struct::IPTool iptool_to_struct(s3s_item::IPTool*);

QJsonObject iptool_to_json(s3s_item::IPTool*);

void json_to_iptool(const QJsonObject&, s3s_item::IPTool*);

#endif // IPTOOLITEM_H

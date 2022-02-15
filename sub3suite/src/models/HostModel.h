#ifndef HOSTMODEL_H
#define HOSTMODEL_H

#include <QStandardItem>
#include <QMap>


namespace s3s_struct {
struct HOST {
    QString host;
    QString ipv4;
    QString ipv6;
    int ping_time;

    /* for open ports */
    QList<quint16> ports;
};
}

namespace s3s_item {
class HOST: public QStandardItem {
public:
    HOST(): QStandardItem(),
        ipv4(new QStandardItem),
        ipv6(new QStandardItem),
        ports(new QStandardItem)
    {
    }
    ~HOST()
    {
    }

public:
    QStandardItem *ipv4;
    QStandardItem *ipv6;
    QStandardItem *ports;

    /* item summary */
    QList<quint16> open_ports;
    QString last_modified;
    QString notes;

    void setValues(const s3s_struct::HOST &host){
        this->setText(host.host);
        ipv4->setText(host.ipv4);
        ipv6->setText(host.ipv6);

        if(!host.ports.isEmpty()){
            QString port_list(ports->text());
            foreach(const quint16 &port, host.ports)
            {
                port_list.append(QString::number(port));
                port_list.append(",");
                open_ports.append(port);
            }
            port_list.chop(1);
            ports->setText(port_list);
        }
    }

    void setValue_ipv4(const QString &_ipv4){
        ipv4->setText(_ipv4);
    }

    void setValue_ipv6(const QString &_ipv6){
        ipv6->setText(_ipv6);
    }

    void setValue_ports(const s3s_struct::HOST &host){
        this->setText(host.host);

        QString port_list(ports->text());
        foreach(const quint16 &port, host.ports)
        {
            port_list.append(QString::number(port));
            port_list.append(",");
            open_ports.append(port);
        }
        port_list.chop(1);
        ports->setText(port_list);
    }
};
}

s3s_struct::HOST host_to_struct(s3s_item::HOST*);

QJsonObject host_to_json(s3s_item::HOST*);

void json_to_host(const QJsonObject&, s3s_item::HOST*);

#endif // HOSTMODEL_H

#ifndef HOSTMODEL_H
#define HOSTMODEL_H

#include <QStandardItem>


namespace s3s_struct {
struct HOST {
    QString host;
    QString ipv4;
    QString ipv6;
    int ping_time;
};
}

namespace s3s_item {
class HOST: public QStandardItem {
public:
    HOST(): QStandardItem(),
        ipv4(new QStandardItem),
        ipv6(new QStandardItem)
    {
    }
    ~HOST()
    {
    }

public:
    QStandardItem *ipv4;
    QStandardItem *ipv6;

    /* item summary */
    QStringList open_ports;
    QString last_modified;
    QString notes;

    void setValues(const s3s_struct::HOST &host){
        this->setText(host.host);
        ipv4->setText(host.ipv4);
        ipv6->setText(host.ipv6);
    }

    void setValue_ipv4(const QString &_ipv4){
        ipv4->setText(_ipv4);
    }

    void setValue_ipv6(const QString &_ipv6){
        ipv6->setText(_ipv6);
    }
};
}

s3s_struct::HOST host_to_struct(s3s_item::HOST*);

QJsonObject host_to_json(s3s_item::HOST*);

void json_to_host(const QJsonObject&, s3s_item::HOST*);

#endif // HOSTMODEL_H

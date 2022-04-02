#ifndef WILDCARDITEM_H
#define WILDCARDITEM_H

#include <QStandardItem>
#include <QDate>



namespace s3s_struct {
struct Wildcard {
    QString wildcard;
    QString ipv4;
    QString ipv6;
};
}

namespace s3s_item {
class Wildcard: public QStandardItem {
public:
    Wildcard(): QStandardItem(),
        ipv4(new QStandardItem),
        ipv6(new QStandardItem)
    {
    }
    ~Wildcard()
    {
    }

public:
    QStandardItem *ipv4;
    QStandardItem *ipv6;

    /* summary */
    QString last_modified;
    QString comment;

    void setValues(const s3s_struct::Wildcard &host){
        this->setText(host.wildcard);
        ipv4->setText(host.ipv4);
        ipv6->setText(host.ipv6);

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }
};
}

s3s_struct::Wildcard wildcard_to_struct(s3s_item::Wildcard*);

QJsonObject wildcard_to_json(s3s_item::Wildcard*);

void json_to_wildcard(const QJsonObject&, s3s_item::Wildcard*);

#endif // WILDCARDITEM_H

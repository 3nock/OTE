#ifndef URLITEM_H
#define URLITEM_H

#include <QStandardItem>
#include <QJsonObject>
#include <QString>
#include <QDate>


namespace s3s_struct {
struct URL {
    QString url;
    QString banner;
    QString content_type;
    int status_code;
};
}

namespace s3s_item {

class URL: public QStandardItem {
public:
    URL(): QStandardItem(),
        banner(new QStandardItem),
        content_type(new QStandardItem),
        status_code(new QStandardItem)
    {
    }
    ~URL()
    {
    }

public:
    QStandardItem *banner;
    QStandardItem *content_type;
    QStandardItem *status_code;

    /* summary */
    QString last_modified;
    QString comment;

    void setValues(const s3s_struct::URL &url){
        this->setText(url.url);
        banner->setText(url.banner);
        content_type->setText(url.content_type);
        status_code->setText(QString::number(url.status_code));

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }
};
}

s3s_struct::URL url_to_struct(s3s_item::URL*);

QJsonObject url_to_json(s3s_item::URL*);

void json_to_url(const QJsonObject&, s3s_item::URL*);

#endif // URLITEM_H

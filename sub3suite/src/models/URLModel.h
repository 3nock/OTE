#ifndef URLMODEL_H
#define URLMODEL_H


#include <QStandardItem>
#include <QJsonObject>


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
        auto banner_text = new QStandardItem("Banner");
        auto content_type_text = new QStandardItem("Content-Type");
        auto status_code_text = new QStandardItem("Status-code");
        banner_text->setForeground(Qt::white);
        content_type_text->setForeground(Qt::white);
        status_code_text->setForeground(Qt::white);

        this->appendRow({banner_text, banner});
        this->appendRow({content_type_text, content_type});
        this->appendRow({status_code_text, status_code});
    }
    ~URL()
    {
    }

public:
    QStandardItem *banner;
    QStandardItem *content_type;
    QStandardItem *status_code;

    void setValues(const s3s_struct::URL &url){
        this->setText(url.url);
        banner->setText(url.banner);
        content_type->setText(url.content_type);
        status_code->setText(QString::number(url.status_code));
    }
};
}

s3s_struct::URL url_to_struct(s3s_item::URL*);

QJsonObject url_to_json(s3s_item::URL*);

void json_to_url(const QJsonObject&, s3s_item::URL*);

#endif // URLMODEL_H

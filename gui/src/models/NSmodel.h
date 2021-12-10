#ifndef NSMODEL_H
#define NSMODEL_H

#include <QStandardItem>
#include <QSet>


/* structure for sending NSModel data along signals & slots */
struct NSModelStruct{
    QString info_ns;
    QString info_ip;
    QSet<QString> domains;
};

/* the main NS results data Model */
class NSModel{
public:
    NSModel():
        model(new QStandardItemModel),

        /* ... */
        info(new QStandardItem("Info")),
        domains(new QStandardItem("Domains")),

        /* info */
        info_ns(new QStandardItem),
        info_ip(new QStandardItem)
    {
        info->setForeground(QColor(220,220,220));
        domains->setForeground(QColor(220,220,220));

        QFont font("Segoe UI", 9, QFont::Bold);
        info->setFont(font);
        domains->setFont(font);

        info->appendRow({new QStandardItem("NameServer"), info_ns});
        info->appendRow({new QStandardItem("Ip"), info_ip});

        /* the model */
        model->setColumnCount(2);
        model->setHorizontalHeaderLabels({"  Property", "  Value"});

        /* append to the model */
        model->appendRow(info);
        model->appendRow(domains);
    }
    ~NSModel(){}

public:
    QStandardItemModel *model;

    /* ... */
    QStandardItem *info;
    QStandardItem *domains;

    /* info */
    QStandardItem *info_ns;
    QStandardItem *info_ip;
};

#endif // NSMODEL_H

#ifndef MODELS_H
#define MODELS_H

#include <QStandardItemModel>
#include <QStandardItem>
#include <QSet>
#include <QString>

/*************** ENUMS *******************/
enum RESULTS{
    subdomains = 0,
    tlds = 1,
    srv = 2,
    a = 3,
    aaaa = 4,
    ns = 5,
    mx = 6,
    txt = 7,
    cname = 8
};

/**************** CLASS ******************/
class ProjectDataModel{
    private:
        ///
        /// Sets containers that saves all enumerated results...
        ///
        QSet<QString> m_subdomainsSet;
        QSet<QString> m_tldsSet;
        QSet<QString> m_aSet;
        QSet<QString> m_aaaaSet;
        QSet<QString> m_nsSet;
        QSet<QString> m_mxSet;
        QSet<QString> m_txtSet;
        QSet<QString> m_cnameSet;
        QSet<QString> m_srvSet;
        ///
        /// main items...
        ///
        QStandardItemModel *m_projectModel;
        QStandardItem *m_rootItem;
        QStandardItem *m_subdomains;
        QStandardItem *m_tlds;
        QStandardItem *m_records;
        ///
        /// sub-items...
        ///
        QStandardItem *m_a;
        QStandardItem *m_aaaa;
        QStandardItem *m_ns;
        QStandardItem *m_mx;
        QStandardItem *m_txt;
        QStandardItem *m_cname;
        QStandardItem *m_srv;

    public:
        ProjectDataModel(QStandardItemModel *projectModel);
        //...
        void append(QStringList items, RESULTS resultType);
};

#endif // MODELS_H

#ifndef MODELS_H
#define MODELS_H

#include <QStandardItemModel>
#include <QSet>

class ProjectDataModel{

    public:
        ProjectDataModel();
        ~ProjectDataModel();
        //...
        void addSubdomain(QStringList);
        void addTLD(QStringList);
        void addIp(QStringList);
        void addA(QStringList);
        void addAAAA(QStringList);
        void addNS(QStringList);
        void addMX(QStringList);
        void addTXT(QStringList);
        void addCNAME(QStringList);
        void addSRV(QStringList);
        //...
        QStandardItemModel *projectModel;

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
};

#endif // MODELS_H

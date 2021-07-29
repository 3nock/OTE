#include "ProjectDataModel.h"

ProjectDataModel::ProjectDataModel(QStandardItemModel *projectModel):
    m_projectModel(projectModel),
    m_subdomains(new QStandardItem("Subdomains")),
    m_tlds(new QStandardItem("Tlds")),
    m_records(new QStandardItem("Records")),
    //...
    m_a(new QStandardItem("A")),
    m_aaaa(new QStandardItem("AAAA")),
    m_ns(new QStandardItem("NS")),
    m_mx(new QStandardItem("MX")),
    m_txt(new QStandardItem("TXT")),
    m_cname(new QStandardItem("CNAME")),
    m_srv(new QStandardItem("SRV"))
{
    m_rootItem = m_projectModel->invisibleRootItem();
    m_projectModel->setColumnCount(3);

    //...
    m_subdomains->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_tlds->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_records->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_a->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_aaaa->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_ns->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_mx->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_txt->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_cname->setIcon(QIcon(":/img/res/icons/folder2.png"));
    m_srv->setIcon(QIcon(":/img/res/icons/folder2.png"));
    //...
    m_rootItem->appendRow(m_subdomains);
    m_rootItem->appendRow(m_tlds);
    m_rootItem->appendRow(m_records);
    //...
    m_records->appendRow(m_a);
    m_records->appendRow(m_aaaa);
    m_records->appendRow(m_ns);
    m_records->appendRow(m_mx);
    m_records->appendRow(m_txt);
    m_records->appendRow(m_cname);
    m_records->appendRow(m_srv);
}

void ProjectDataModel::append(QList<QString> items, RESULTS resultsType){
    ///
    /// checks...
    ///
    if(items.isEmpty()){
        return;
    }
    ///
    /// ...
    ///
    switch(resultsType){
    case RESULTS::subdomains:
        if(!m_subdomainsSet.contains(items[0])){
            m_subdomainsSet.insert(items[0]);
            m_subdomains->appendRow({new QStandardItem(items[0]), new QStandardItem(items[1])});
        }
        break;
    case RESULTS::tlds:
        if(!m_tldsSet.contains(items[0])){
            m_tldsSet.insert(items[0]);
            m_tlds->appendRow({new QStandardItem(items[0]), new QStandardItem(items[1])});
        }
        break;
    case RESULTS::a:
        if(!m_aSet.contains(items[0])){
            m_aSet.insert(items[0]);
            m_a->appendRow(new QStandardItem(items[0]));
        }
        break;
    case RESULTS::aaaa:
        if(!m_aaaaSet.contains(items[0])){
            m_aaaaSet.insert(items[0]);
            m_aaaa->appendRow(new QStandardItem(items[0]));
        }
        break;
    case RESULTS::ns:
        if(!m_nsSet.contains(items[0])){
            m_nsSet.insert(items[0]);
            m_ns->appendRow(new QStandardItem(items[0]));
        }
        break;
    case RESULTS::mx:
        if(!m_mxSet.contains(items[0])){
            m_mxSet.insert(items[0]);
            m_mx->appendRow(new QStandardItem(items[0]));
        }
        break;
    case RESULTS::txt:
        if(!m_txtSet.contains(items[0])){
            m_txtSet.insert(items[0]);
            m_txt->appendRow(new QStandardItem(items[0]));
        }
        break;
    case RESULTS::cname:
        if(!m_cnameSet.contains(items[0])){
            m_cnameSet.insert(items[0]);
            m_cname->appendRow(new QStandardItem(items[0]));
        }
        break;
    case RESULTS::srv:
        if(!m_srvSet.contains(items[0])){
            m_srvSet.insert(items[0]);
            m_srv->appendRow({new QStandardItem(items[0]), new QStandardItem(items[1])});
        }
        break;
    }
}

void ProjectDataModel::setColumnCount(int columnCount){
    m_projectModel->setColumnCount(columnCount);
}


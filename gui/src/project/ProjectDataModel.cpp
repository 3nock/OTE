#include "ProjectDataModel.h"

ProjectDataModel::ProjectDataModel():
    projectModel(new QStandardItemModel),
    //...
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
    m_rootItem = projectModel->invisibleRootItem();
    projectModel->setColumnCount(3);
    ///
    /// ...
    ///
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
    ///
    /// ...
    ///
    m_rootItem->appendRow(m_subdomains);
    m_rootItem->appendRow(m_tlds);
    m_rootItem->appendRow(m_records);
    ///
    /// ...
    ///
    m_records->appendRow(m_a);
    m_records->appendRow(m_aaaa);
    m_records->appendRow(m_ns);
    m_records->appendRow(m_mx);
    m_records->appendRow(m_txt);
    m_records->appendRow(m_cname);
    m_records->appendRow(m_srv);
}
ProjectDataModel::~ProjectDataModel(){
    ///
    /// save all files first using RAII model...
    ///

    ///
    /// then delete all items...
    ///
    delete projectModel;
    //...
    delete m_subdomains;
    delete m_tlds;
    delete m_records;
    //...
    delete m_a;
    delete m_aaaa;
    delete m_ns;
    delete m_mx;
    delete m_txt;
    delete m_cname;
    delete m_srv;
}

void ProjectDataModel::addSubdomain(QStringList items){
    int prevSize = m_subdomainsSet.count();
    m_subdomainsSet.insert(items[0]);
    if(m_subdomainsSet.count() > prevSize)
        m_subdomains->appendRow({new QStandardItem(items[0]), new QStandardItem(items[1]), new QStandardItem(items[2])});
}

void ProjectDataModel::addTLD(QStringList items){
    int prevSize = m_tldsSet.count();
    m_tldsSet.insert(items[0]);
    if(m_tldsSet.count() > prevSize)
        m_tlds->appendRow({new QStandardItem(items[0]), new QStandardItem(items[1]), new QStandardItem(items[2])});
}

void ProjectDataModel::addIp(QStringList items){
    Q_UNUSED(items);
}

void ProjectDataModel::addA(QStringList items){
    int prevSize = m_aSet.count();
    m_aSet.insert(items[0]);
    if(m_aSet.count() > prevSize)
        m_a->appendRow({new QStandardItem(items[0]), nullptr, new QStandardItem(items[1])});
}

void ProjectDataModel::addAAAA(QStringList items){
    int prevSize = m_aaaaSet.count();
    m_aaaaSet.insert(items[0]);
    if(m_aaaaSet.count() > prevSize)
        m_aaaa->appendRow({new QStandardItem(items[0]), nullptr, new QStandardItem(items[1])});
}

void ProjectDataModel::addNS(QStringList items){
    int prevSize = m_nsSet.count();
    m_nsSet.insert(items[0]);
    if(m_nsSet.count() > prevSize)
        m_ns->appendRow({new QStandardItem(items[0]), nullptr, new QStandardItem(items[1])});
}

void ProjectDataModel::addMX(QStringList items){
    int prevSize = m_mxSet.count();
    m_mxSet.insert(items[0]);
    if(m_mxSet.count() > prevSize)
        m_mx->appendRow({new QStandardItem(items[0]), nullptr, new QStandardItem(items[1])});
}

void ProjectDataModel::addTXT(QStringList items){
    int prevSize = m_txtSet.count();
    m_txtSet.insert(items[0]);
    if(m_txtSet.count() > prevSize)
        m_txt->appendRow({new QStandardItem(items[0]), nullptr, new QStandardItem(items[1])});
}

void ProjectDataModel::addCNAME(QStringList items){
    int prevSize = m_cnameSet.count();
    m_cnameSet.insert(items[0]);
    if(m_cnameSet.count() > prevSize)
        m_cname->appendRow({new QStandardItem(items[0]), nullptr, new QStandardItem(items[1])});
}

void ProjectDataModel::addSRV(QStringList items){
    int prevSize = m_tldsSet.count();
    m_tldsSet.insert(items[0]);
    if(m_tldsSet.count() > prevSize)
        m_srv->appendRow({new QStandardItem(items[0]), new QStandardItem(items[1]), new QStandardItem(items[2])});
}

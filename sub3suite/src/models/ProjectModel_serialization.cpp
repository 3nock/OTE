#include "ProjectModel.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QGuiApplication>

#include "src/utils/Config.h"
#include "src/models/SSLModel.h"
#include "src/models/ASNModel.h"


void ProjectModel::openExistingProject(QString name, QString path){
    ProjectStruct project_info;
    project_info.name = name;
    project_info.path = path;
    project_info.isExisting = true;

    /* clear models */
    qDebug() << "Clearing current project models";
    this->clearModels();

    /* open project */
    qDebug() << "Opening Another Project";
    this->openProject(project_info);
}

void ProjectModel::saveProject(){
    qDebug() << "Saving the Project To: " << projectInfo.path;

    QFile file(projectInfo.path);
    if(file.open(QIODevice::WriteOnly))
    {
        /* compress the data then save */
        file.write(qCompress(this->getJson()));
        file.close();

        CONFIG.beginGroup("recent_projects");
        CONFIG.setValue(projectInfo.name, projectInfo.path);
        CONFIG.endGroup();
    }
    else{
        qWarning() << "Failed To Open Project File";
    }
}

void ProjectModel::closeProject(){
    QByteArray project_json(this->getJson());
    if(m_project_hash == QCryptographicHash::hash(project_json, QCryptographicHash::Md5))
        return; // no changes made to project

    int retVal = QMessageBox::warning(nullptr, "Sub3 Suite",
                               "The project has been modified. Do you want to save changes?",
                                   QMessageBox::Save |
                                   QMessageBox::Cancel,
                                   QMessageBox::Save);
    if(retVal == QMessageBox::Save)
        this->saveProject();
}

void ProjectModel::openProject(ProjectStruct projectStruct){
    projectInfo = projectStruct;

    if(projectInfo.isTemporary){
        projectInfo.name = "Temp";
        projectInfo.path = QGuiApplication::applicationDirPath()+"/projects/";

        /* set project name on the project explorer */
        project_explorer->setText(projectInfo.name);
        return;
    }

    if(projectInfo.isNew){
        /* set project name on the project explorer */
        project_explorer->setText(projectInfo.name);
        return;
    }

    /* if it is an existing project */
    /* set project name on the project explorer */
    project_explorer->setText(projectInfo.name);

    /* opening the the project */
    qDebug() << "Opening Project: " << projectInfo.path;

    QFile file(projectInfo.path);
    if(!file.open(QIODevice::ReadOnly)){
        qWarning() << "Failed To Open Project File.";
        return;
    }

    /* uncompress the file then open json */
    QByteArray project_json(qUncompress(file.readAll()));

    /* get the project hash, so as to alert on closing if any changes made */
    m_project_hash = QCryptographicHash::hash(project_json, QCryptographicHash::Md5);

    qDebug() << "Parsing the Project...";
    QJsonDocument document = QJsonDocument::fromJson(project_json);
    file.close();
    if(document.isNull() || document.isEmpty()){
        qWarning() << "Error parsing the project file";
        return;
    }

    QJsonObject mainObj = document.object();

    ///
    /// Project Information
    ///
    QJsonObject info = mainObj["general"].toObject();

    ///
    /// Project Data
    ///
    QJsonObject data = mainObj["data"].toObject();

    /* active subdomainIp */
    foreach(const QJsonValue &value, data["active_subdomainIP"].toArray())
        m_activeSubdomainIp_rootItem->appendRow({new QStandardItem(value.toArray()[0].toString()),
                                                 new QStandardItem(value.toArray()[1].toString())});

    /* active subdomain */
    foreach(const QJsonValue &value, data["active_subdomain"].toArray())
        m_activeSubdomain_rootItem->appendRow(new QStandardItem(value.toString()));

    /* active TLD */
    foreach(const QJsonValue &value, data["active_TLD"].toArray())
        m_activeTld_rootItem->appendRow({new QStandardItem(value.toArray()[0].toString()),
                                         new QStandardItem(value.toArray()[1].toString())});

    /* active Wildcards */
    foreach(const QJsonValue &value, data["active_wildcard"].toArray())
        m_activeWildcard_rootItem->appendRow({new QStandardItem(value.toArray()[0].toString()),
                                              new QStandardItem(value.toArray()[1].toString())});

    /* active dns */
    foreach(const QJsonValue &value, data["active_dns"].toArray()){
        s3s_item::DNS *item = new s3s_item::DNS;
        json_to_dns(value.toObject(), item);
        m_activeDNS_rootItem->appendRow(item);
    }

    /* active SSL sha1 */
    foreach(const QJsonValue &value, data["active_SSL_sha1"].toArray())
        m_activeSSL_sha1_rootItem->appendRow(new QStandardItem(value.toString()));

    /* active SSL sha256 */
    foreach(const QJsonValue &value, data["active_SSL_sha256"].toArray())
        m_activeSSL_sha256_rootItem->appendRow(new QStandardItem(value.toString()));

    /* active SSL */
    foreach(const QJsonValue &value, data["active_SSL"].toArray()){
        s3s_item::SSL *item = new s3s_item::SSL;
        json_to_ssl(value.toObject(), item);
        m_activeSSL_rootItem->appendRow(item);
    }

    /* passive subdomainIP */
    foreach(const QJsonValue &value, data["passive_subdomainIP"].toArray())
        m_passiveSubdomainIp_rootItem->appendRow({new QStandardItem(value.toArray()[0].toString()),
                                                 new QStandardItem(value.toArray()[1].toString())});

    /* passive ASN */
    foreach(const QJsonValue &value, data["passive_ASN"].toArray())
        m_passiveAsn_rootItem->appendRow({new QStandardItem(value.toArray()[0].toString()),
                                                 new QStandardItem(value.toArray()[1].toString())});

    /* passive Subdomain */
    foreach(const QJsonValue &value, data["passive_subdomain"].toArray())
        m_passiveSubdomain_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive A */
    foreach(const QJsonValue &value, data["passive_A"].toArray())
        m_passiveA_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive AAAA */
    foreach(const QJsonValue &value, data["passive_AAAA"].toArray())
        m_passiveAAAA_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive CIDR */
    foreach(const QJsonValue &value, data["passive_CIDR"].toArray())
        m_passiveCidr_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive NS */
    foreach(const QJsonValue &value, data["passive_NS"].toArray())
        m_passiveNS_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive MX */
    foreach(const QJsonValue &value, data["passive_MX"].toArray())
        m_passiveMX_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive TXT */
    foreach(const QJsonValue &value, data["passive_TXT"].toArray())
        m_passiveTXT_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive CNAME */
    foreach(const QJsonValue &value, data["passive_CNAME"].toArray())
        m_passiveCNAME_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive Email */
    foreach(const QJsonValue &value, data["passive_Email"].toArray())
        m_passiveEmail_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive URL */
    foreach(const QJsonValue &value, data["passive_URL"].toArray())
        m_passiveUrl_rootItem->appendRow(new QStandardItem(value.toString()));

    /* passive SSL */
    foreach(const QJsonValue &value, data["passive_SSL"].toArray())
        m_passiveSSL_rootItem->appendRow(new QStandardItem(value.toString()));

    /* enum ASN */
    foreach(const QJsonValue &value, data["enum_ASN"].toArray()){
        s3s_item::ASN *item = new s3s_item::ASN;
        json_to_asn(value.toObject(), item);
        m_enumASN_rootItem->appendRow(item);
    }
    /* enum CIDR" */
    foreach(const QJsonValue &value, data["enum_CIDR"].toArray()){
        s3s_item::CIDR *item = new s3s_item::CIDR;
        json_to_cidr(value.toObject(), item);
        m_enumCIDR_rootItem->appendRow(item);
    }
    /* enum IP */
    foreach(const QJsonValue &value, data["enum_IP"].toArray()){
        s3s_item::IP *item = new s3s_item::IP;
        json_to_ip(value.toObject(), item);
        m_enumIp_rootItem->appendRow(item);
    }
    /* enum MX" */
    foreach(const QJsonValue &value, data["enum_MX"].toArray()){
        s3s_item::MX *item = new s3s_item::MX;
        json_to_mx(value.toObject(), item);
        m_enumMX_rootItem->appendRow(item);
    }
    /* enum NS" */
    foreach(const QJsonValue &value, data["enum_NS"].toArray()){
        s3s_item::NS *item = new s3s_item::NS;
        json_to_ns(value.toObject(), item);
        m_enumNS_rootItem->appendRow(item);
    }
    /* enum SSL" */
    foreach(const QJsonValue &value, data["enum_SSL"].toArray()){
        s3s_item::SSL *item = new s3s_item::SSL;
        json_to_ssl(value.toObject(), item);
        m_enumSSL_rootItem->appendRow(item);
    }
    /* enum Email" */
    foreach(const QJsonValue &value, data["enum_Email"].toArray()){
        s3s_item::Email *item = new s3s_item::Email;
        json_to_email(value.toObject(), item);
        m_enumEmail_rootItem->appendRow(item);
    }
    /* enum URL" */
    /* custom" */

    qDebug() << "Project " << projectInfo.name << " Opened.";
}

QByteArray ProjectModel::getJson(){
    QJsonArray passive_SSL_array;
    QJsonArray passive_URL_array;
    QJsonArray passive_Email_array;
    QJsonArray passive_CNAME_array;
    QJsonArray passive_TXT_array;
    QJsonArray passive_NS_array;
    QJsonArray passive_MX_array;
    QJsonArray passive_CIDR_array;
    QJsonArray passive_A_array;
    QJsonArray passive_AAAA_array;
    QJsonArray passive_Subdomain_array;
    QJsonArray passive_ASN_array;
    QJsonArray passive_SubdomainIp_array;

    QJsonArray active_SSL_sha1_array;
    QJsonArray active_SSL_sha256_array;
    QJsonArray active_Subdomain_array;
    QJsonArray active_TLD_array;
    QJsonArray active_wildcard_array;
    QJsonArray active_SubdomainIp_array;
    QJsonArray active_SSL_array;
    QJsonArray active_DNS_array;

    QJsonArray enum_IP_array;
    QJsonArray enum_MX_array;
    QJsonArray enum_NS_array;
    QJsonArray enum_ASN_array;
    QJsonArray enum_CIDR_array;
    QJsonArray enum_SSL_array;
    QJsonArray enum_Email_array;
    QJsonArray enum_URL_array;
    QJsonArray custom_array;

    /* passive SSL */
    for(int i = 0; i != m_passiveSSL_rootItem->rowCount(); ++i)
        passive_SSL_array.append(m_passiveSSL_rootItem->child(i, 0)->text());

    /* passive URL */
    for(int i = 0; i != m_passiveUrl_rootItem->rowCount(); ++i)
        passive_URL_array.append(m_passiveUrl_rootItem->child(i, 0)->text());

    /* passive Email */
    for(int i = 0; i != m_passiveEmail_rootItem->rowCount(); ++i)
        passive_Email_array.append(m_passiveEmail_rootItem->child(i, 0)->text());

    /* passive CNAME */
    for(int i = 0; i != m_passiveCNAME_rootItem->rowCount(); ++i)
        passive_CNAME_array.append(m_passiveCNAME_rootItem->child(i, 0)->text());

    /* passive TXT */
    for(int i = 0; i != m_passiveTXT_rootItem->rowCount(); ++i)
        passive_TXT_array.append(m_passiveTXT_rootItem->child(i, 0)->text());

    /* passive NS */
    for(int i = 0; i != m_passiveNS_rootItem->rowCount(); ++i)
        passive_NS_array.append(m_passiveNS_rootItem->child(i, 0)->text());

    /* passive MX */
    for(int i = 0; i != m_passiveMX_rootItem->rowCount(); ++i)
        passive_MX_array.append(m_passiveMX_rootItem->child(i, 0)->text());

    /* passive CIDR */
    for(int i = 0; i != m_passiveCidr_rootItem->rowCount(); ++i)
        passive_CIDR_array.append(m_passiveCidr_rootItem->child(i, 0)->text());

    /* passive A */
    for(int i = 0; i != m_passiveA_rootItem->rowCount(); ++i)
        passive_A_array.append(m_passiveA_rootItem->child(i, 0)->text());

    /* passive AAAA */
    for(int i = 0; i != m_passiveAAAA_rootItem->rowCount(); ++i)
        passive_AAAA_array.append(m_passiveAAAA_rootItem->child(i, 0)->text());

    /* passive Subdomain */
    for(int i = 0; i != m_passiveSubdomain_rootItem->rowCount(); ++i)
        passive_Subdomain_array.append(m_passiveSubdomain_rootItem->child(i, 0)->text());

    /* passive ASN */
    for(int i = 0; i != m_passiveAsn_rootItem->rowCount(); ++i){
        QJsonArray asn;
        asn.append(m_passiveAsn_rootItem->child(i, 0)->text());
        asn.append(m_passiveAsn_rootItem->child(i, 1)->text());
        passive_ASN_array.append(asn);
    }

    /* passive subdomainIP */
    for(int i = 0; i != m_passiveSubdomainIp_rootItem->rowCount(); ++i){
        QJsonArray subdomainIp;
        subdomainIp.append(m_passiveSubdomainIp_rootItem->child(i, 0)->text());
        subdomainIp.append(m_passiveSubdomainIp_rootItem->child(i, 1)->text());
        passive_SubdomainIp_array.append(subdomainIp);
    }

    /* active SSL sha1 */
    for(int i = 0; i != m_activeSSL_sha1_rootItem->rowCount(); ++i)
        active_SSL_sha1_array.append(m_activeSSL_sha1_rootItem->child(i, 0)->text());

    /* active SSL sha256 */
    for(int i = 0; i != m_activeSSL_sha256_rootItem->rowCount(); ++i)
        active_SSL_sha256_array.append(m_activeSSL_sha256_rootItem->child(i, 0)->text());

    /* active Subdomain */
    for(int i = 0; i != m_activeSubdomain_rootItem->rowCount(); ++i)
        active_Subdomain_array.append(m_activeSubdomain_rootItem->child(i, 0)->text());

    /* active TLD */
    for(int i = 0; i != m_activeTld_rootItem->rowCount(); ++i){
        QJsonArray tld;
        tld.append(m_activeTld_rootItem->child(i, 0)->text());
        tld.append(m_activeTld_rootItem->child(i, 1)->text());
        active_TLD_array.append(tld);
    }

    /* active Wildcard */
    for(int i = 0; i != m_activeWildcard_rootItem->rowCount(); ++i){
        QJsonArray wildcard;
        wildcard.append(m_activeWildcard_rootItem->child(i, 0)->text());
        wildcard.append(m_activeWildcard_rootItem->child(i, 1)->text());
        active_wildcard_array.append(wildcard);
    }

    /* active SubdomainIp */
    for(int i = 0; i != m_activeSubdomainIp_rootItem->rowCount(); ++i){
        QJsonArray subdomainIp;
        subdomainIp.append(m_activeSubdomainIp_rootItem->child(i, 0)->text());
        subdomainIp.append(m_activeSubdomainIp_rootItem->child(i, 1)->text());
        active_SubdomainIp_array.append(subdomainIp);
    }

    /* active DNS */
    for(int i = 0; i < m_activeDNS_rootItem->rowCount(); ++i){
        QModelIndex index = m_activeDNS_rootItem->child(i, 0)->index();
        s3s_item::DNS *item = static_cast<s3s_item::DNS*>(activeDNS_model->item(index.row(), index.column()));

        active_DNS_array.append(dns_to_json(item));
    }

    /* active SSL */
    for(int i = 0; i < m_activeSSL_rootItem->rowCount(); ++i){
        QModelIndex index = m_activeSSL_rootItem->child(i, 0)->index();
        s3s_item::SSL *item = static_cast<s3s_item::SSL*>(activeSSL_model->item(index.row(), index.column()));

        active_SSL_array.append(ssl_to_json(item));
    }

    /* enum ASN */
    for(int i = 0; i < m_enumASN_rootItem->rowCount(); ++i){
        QModelIndex index = m_enumASN_rootItem->child(i, 0)->index();
        s3s_item::ASN *item = static_cast<s3s_item::ASN*>(enumASN_model->item(index.row(), index.column()));

        enum_ASN_array.append(asn_to_json(item));
    }

    /* enum CIDR */
    for(int i = 0; i < m_enumCIDR_rootItem->rowCount(); ++i){
        QModelIndex index = m_enumCIDR_rootItem->child(i, 0)->index();
        s3s_item::CIDR *item = static_cast<s3s_item::CIDR*>(enumCIDR_model->item(index.row(), index.column()));

        enum_CIDR_array.append(cidr_to_json(item));
    }

    /* enum IP */
    for(int i = 0; i < m_enumIp_rootItem->rowCount(); ++i){
        QModelIndex index = m_enumIp_rootItem->child(i, 0)->index();
        s3s_item::IP *item = static_cast<s3s_item::IP*>(enumIp_model->item(index.row(), index.column()));

        enum_IP_array.append(ip_to_json(item));
    }

    /* enum MX */
    for(int i = 0; i < m_enumMX_rootItem->rowCount(); ++i){
        QModelIndex index = m_enumMX_rootItem->child(i, 0)->index();
        s3s_item::MX *item = static_cast<s3s_item::MX*>(enumMX_model->item(index.row(), index.column()));

        enum_MX_array.append(mx_to_json(item));
    }

    /* enum NS */
    for(int i = 0; i < m_enumNS_rootItem->rowCount(); ++i){
        QModelIndex index = m_enumNS_rootItem->child(i, 0)->index();
        s3s_item::NS *item = static_cast<s3s_item::NS*>(enumNS_model->item(index.row(), index.column()));

        enum_NS_array.append(ns_to_json(item));
    }

    /* enum SSL */
    for(int i = 0; i < m_enumSSL_rootItem->rowCount(); ++i){
        QModelIndex index = m_enumSSL_rootItem->child(i, 0)->index();
        s3s_item::SSL *item = static_cast<s3s_item::SSL*>(enumSSL_model->item(index.row(), index.column()));

        enum_SSL_array.append(ssl_to_json(item));
    }

    /* enum Email */
    for(int i = 0; i < m_enumEmail_rootItem->rowCount(); ++i){
        QModelIndex index = m_enumEmail_rootItem->child(i, 0)->index();
        s3s_item::Email *item = static_cast<s3s_item::Email*>(enumEmail_model->item(index.row(), index.column()));

        enum_Email_array.append(email_to_json(item));
    }

    QJsonObject general;
    general.insert("path", "C:/Users/inner peace/Desktop/sub3suite/MSVC2017_64-Release/projects");
    general.insert("name", "one");

    QJsonObject data;
    data.insert("active_subdomainIP", active_SubdomainIp_array);
    data.insert("active_subdomain", active_Subdomain_array);
    data.insert("active_TLD", active_TLD_array);
    data.insert("active_wildcard", active_wildcard_array);
    data.insert("active_dns", active_DNS_array);
    data.insert("active_SSL", active_SSL_array);
    data.insert("active_SSL_sha1", active_SSL_sha1_array);
    data.insert("active_SSL_sha256", active_SSL_sha256_array);
    data.insert("passive_subdomainIP", passive_SubdomainIp_array);
    data.insert("passive_subdomain", passive_Subdomain_array);
    data.insert("passive_A", passive_A_array);
    data.insert("passive_AAAA", passive_AAAA_array);
    data.insert("passive_CIDR", passive_CIDR_array);
    data.insert("passive_NS", passive_NS_array);
    data.insert("passive_MX", passive_MX_array);
    data.insert("passive_TXT", passive_TXT_array);
    data.insert("passive_CNAME", passive_CNAME_array);
    data.insert("passive_Email", passive_Email_array);
    data.insert("passive_URL", passive_URL_array);
    data.insert("passive_ASN", passive_ASN_array);
    data.insert("passive_SSL", passive_SSL_array);
    data.insert("enum_IP", enum_IP_array);
    data.insert("enum_MX", enum_MX_array);
    data.insert("enum_NS", enum_NS_array);
    data.insert("enum_ASN", enum_ASN_array);
    data.insert("enum_CIDR", enum_CIDR_array);
    data.insert("enum_SSL", enum_SSL_array);
    data.insert("enum_Email", enum_Email_array);
    data.insert("enum_URL", enum_URL_array);
    data.insert("custom", custom_array);

    QJsonObject mainObj;
    mainObj.insert("general", general);
    mainObj.insert("data", data);

    QJsonDocument document;
    document.setObject(mainObj);
    return document.toJson(QJsonDocument::Compact);
}

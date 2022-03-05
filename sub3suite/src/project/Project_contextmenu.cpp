#include "Project.h"
#include "ui_Project.h"


void Project::on_treeViewTree_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->treeViewTree->selectionModel()->isSelected(ui->treeViewTree->currentIndex()))
        return;

    /* getting the selected items... */
    m_selectionModel = ui->treeViewTree->selectionModel();

    /* extract menu */
    QMenu extractMenu(this);
    extractMenu.setTitle(tr("Extract"));
    extractMenu.setIcon(QIcon(":/img/res/icons/extract.png"));
    extractMenu.addAction(tr("Subdomain(*.)"), this, [=](){this->action_extract_selected(true, false);});
    extractMenu.addAction(tr("Top level domain(.*)"), this, [=](){this->action_extract_selected(false, true);});

    /* main menu */
    QMenu menu(this);

    menu.addAction(tr("Remove"), this, [=](){this->action_remove_selected();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    menu.addSeparator();
    menu.addAction(tr("Save"), this, [=](){this->action_save_selected();})->setIcon(QIcon(":/img/res/icons/save.png"));
    menu.addAction(tr("Copy"), this, [=](){this->action_copy_selected();})->setIcon(QIcon(":/img/res/icons/copy.png"));
    menu.addSeparator();

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeHost:
        menu.addAction(tr("Open in Browser"), this, [=](){this->action_openInBrowser();})->setIcon(QIcon(":/img/res/icons/browser.png"));
        if(m_selectionModel->columnIntersectsSelection(0, m_selectionModel->currentIndex().parent()))
            menu.addMenu(&extractMenu);
        menu.addSeparator();
        if(m_selectionModel->columnIntersectsSelection(1, m_selectionModel->currentIndex().parent()) ||
           m_selectionModel->columnIntersectsSelection(2, m_selectionModel->currentIndex().parent())){
            menu.addAction(tr("Send IpAddress to OSINT"), this, [=](){this->action_send_selected_toEngine(ENGINE::OSINT, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
            menu.addAction(tr("Send IpAddress to RAW"), this, [=](){this->action_send_selected_toEngine(ENGINE::RAW, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
            menu.addSeparator();
        }
        if(m_selectionModel->columnIntersectsSelection(0, m_selectionModel->currentIndex().parent())){
            menu.addAction(tr("Send Hostname to OSINT"), this, [=](){this->action_send_selected_toEngine(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname to RAW"), this, [=](){this->action_send_selected_toEngine(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname to BRUTE"), this, [=](){this->action_send_selected_toEngine(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname to ACTIVE"), this, [=](){this->action_send_selected_toEngine(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname to DNS"), this, [=](){this->action_send_selected_toEngine(ENGINE::DNS, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname to SSL"), this, [=](){this->action_send_selected_toEngine(ENGINE::SSL, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addSeparator();
        }
        if(m_selectionModel->columnIntersectsSelection(1, m_selectionModel->currentIndex().parent()) ||
           m_selectionModel->columnIntersectsSelection(2, m_selectionModel->currentIndex().parent()))
            menu.addAction(tr("Send IpAddress to IP-Enum"), this, [=](){this->action_send_selected_toEnum(TOOL::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
        break;
    case ExplorerType::activeWildcard:
        if(m_selectionModel->columnIntersectsSelection(1, m_selectionModel->currentIndex().parent()) ||
           m_selectionModel->columnIntersectsSelection(2, m_selectionModel->currentIndex().parent())){
            menu.addAction(tr("Send IpAddress to OSINT"), this, [=](){this->action_send_selected_toEngine(ENGINE::OSINT, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
            menu.addAction(tr("Send IpAddress to RAW"), this, [=](){this->action_send_selected_toEngine(ENGINE::RAW, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
            menu.addSeparator();
            menu.addAction(tr("Send IpAddress to IP-Enum"), this, [=](){this->action_send_selected_toEnum(TOOL::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
        }
        break;
    case ExplorerType::activeDNS:
        if(m_selectionModel->columnIntersectsSelection(0, m_selectionModel->currentIndex().parent())){
            menu.addAction(tr("Send IpAddress to OSINT"), this, [=](){this->action_send_selected_toEngine(ENGINE::OSINT, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
            menu.addAction(tr("Send IpAddress to RAW"), this, [=](){this->action_send_selected_toEngine(ENGINE::RAW, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
            menu.addSeparator();
            menu.addAction(tr("Send Hostname to OSINT"), this, [=](){this->action_send_selected_toEngine(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname to RAW"), this, [=](){this->action_send_selected_toEngine(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname to BRUTE"), this, [=](){this->action_send_selected_toEngine(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname to ACTIVE"), this, [=](){this->action_send_selected_toEngine(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname to DNS"), this, [=](){this->action_send_selected_toEngine(ENGINE::DNS, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname to SSL"), this, [=](){this->action_send_selected_toEngine(ENGINE::SSL, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addSeparator();
            menu.addAction(tr("Send IpAddress to IP-Enum"), this, [=](){this->action_send_selected_toEnum(TOOL::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
            menu.addAction(tr("Send MX to MX-Enum"), this, [=](){this->action_send_selected_toEnum(TOOL::MX);})->setIcon(QIcon(":/img/res/icons/mx.png"));
            menu.addAction(tr("Send NS to NS-Enum"), this, [=](){this->action_send_selected_toEnum(TOOL::NS);})->setIcon(QIcon(":/img/res/icons/ns.png"));
        }
        break;
    case ExplorerType::activeDNS_A:
    case ExplorerType::activeDNS_AAAA:
    case ExplorerType::passive_A:
    case ExplorerType::passive_AAAA:
        menu.addAction(tr("Open in Browser"), this, [=](){this->action_openInBrowser();})->setIcon(QIcon(":/img/res/icons/browser.png"));
        menu.addSeparator();
        menu.addAction(tr("Send IpAddress to OSINT"), this, [=](){this->action_send_selected_toEngine(ENGINE::OSINT, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
        menu.addAction(tr("Send IpAddress to RAW"), this, [=](){this->action_send_selected_toEngine(ENGINE::RAW, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
        menu.addSeparator();
        menu.addAction(tr("Send IpAddress to IP-Enum"), this, [=](){this->action_send_selected_toEnum(TOOL::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
        break;
    case ExplorerType::activeDNS_NS:
    case ExplorerType::passive_NS:
        menu.addAction(tr("Open in Browser"), this, [=](){this->action_openInBrowser();})->setIcon(QIcon(":/img/res/icons/browser.png"));
        menu.addSeparator();
        menu.addAction(tr("Send NS to OSINT"), this, [=](){this->action_send_selected_toEngine(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/ns.png"));
        menu.addAction(tr("Send NS to RAW"), this, [=](){this->action_send_selected_toEngine(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/ns.png"));
        menu.addAction(tr("Send NS to BRUTE"), this, [=](){this->action_send_selected_toEngine(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/ns.png"));
        menu.addAction(tr("Send NS to ACTIVE"), this, [=](){this->action_send_selected_toEngine(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/ns.png"));
        menu.addAction(tr("Send NS to DNS"), this, [=](){this->action_send_selected_toEngine(ENGINE::DNS, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/ns.png"));
        menu.addAction(tr("Send NS to SSL"), this, [=](){this->action_send_selected_toEngine(ENGINE::SSL, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/ns.png"));
        menu.addSeparator();
        menu.addAction(tr("Send NS to NS-Enum"), this, [=](){this->action_send_selected_toEnum(TOOL::NS);})->setIcon(QIcon(":/img/res/icons/ns.png"));
        break;
    case ExplorerType::activeDNS_MX:
    case ExplorerType::passive_MX:
        menu.addAction(tr("Open in Browser"), this, [=](){this->action_openInBrowser();})->setIcon(QIcon(":/img/res/icons/browser.png"));
        menu.addSeparator();
        menu.addAction(tr("Send MX to OSINT"), this, [=](){this->action_send_selected_toEngine(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/mx.png"));
        menu.addAction(tr("Send MX to RAW"), this, [=](){this->action_send_selected_toEngine(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/mx.png"));
        menu.addAction(tr("Send MX to BRUTE"), this, [=](){this->action_send_selected_toEngine(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/mx.png"));
        menu.addAction(tr("Send MX to ACTIVE"), this, [=](){this->action_send_selected_toEngine(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/mx.png"));
        menu.addAction(tr("Send MX to DNS"), this, [=](){this->action_send_selected_toEngine(ENGINE::DNS, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/mx.png"));
        menu.addAction(tr("Send MX to SSL"), this, [=](){this->action_send_selected_toEngine(ENGINE::SSL, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/mx.png"));
        menu.addSeparator();
        menu.addAction(tr("Send MX to MX-Enum"), this, [=](){this->action_send_selected_toEnum(TOOL::MX);})->setIcon(QIcon(":/img/res/icons/mx.png"));
        break;
    case ExplorerType::activeDNS_CNAME:
    case ExplorerType::passive_CNAME:
    case ExplorerType::activeSSL_altNames:
    case ExplorerType::passive_subdomain:
        menu.addAction(tr("Open in Browser"), this, [=](){this->action_openInBrowser();})->setIcon(QIcon(":/img/res/icons/browser.png"));
        menu.addSeparator();
        menu.addMenu(&extractMenu);
        menu.addSeparator();
        menu.addAction(tr("Send Hostname to OSINT"), this, [=](){this->action_send_selected_toEngine(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname to RAW"), this, [=](){this->action_send_selected_toEngine(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname to BRUTE"), this, [=](){this->action_send_selected_toEngine(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname to ACTIVE"), this, [=](){this->action_send_selected_toEngine(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname to DNS"), this, [=](){this->action_send_selected_toEngine(ENGINE::DNS, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        menu.addAction(tr("Send Hostname to SSL"), this, [=](){this->action_send_selected_toEngine(ENGINE::SSL, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        break;
    case ExplorerType::activeSSL_sha1:
    case ExplorerType::activeSSL_sha256:
    case ExplorerType::passive_SSL:
        menu.addAction(tr("Send To SSL-Enum"), this, [=](){this->action_send_selected_toEnum(TOOL::SSL);})->setIcon(QIcon(":/img/res/icons/ssl.png"));
        break;
    case ExplorerType::activeURL:
    case ExplorerType::passive_URL:
        if(m_selectionModel->columnIntersectsSelection(0, m_selectionModel->currentIndex().parent())){
            menu.addAction(tr("Open in Browser"), this, [=](){this->action_openInBrowser();})->setIcon(QIcon(":/img/res/icons/browser.png"));
            menu.addSeparator();
            menu.addAction(tr("Send URL to OSINT"), this, [=](){this->action_send_selected_toEngine(ENGINE::OSINT, RESULT_TYPE::URL);})->setIcon(QIcon(":/img/res/icons/url.png"));
            menu.addAction(tr("Send URL to RAW"), this, [=](){this->action_send_selected_toEngine(ENGINE::RAW, RESULT_TYPE::URL);})->setIcon(QIcon(":/img/res/icons/url.png"));
            menu.addAction(tr("Send URL to URL"), this, [=](){this->action_send_selected_toEngine(ENGINE::URL, RESULT_TYPE::URL);})->setIcon(QIcon(":/img/res/icons/url.png"));
        }
        break;
    case ExplorerType::passive_subdomainIp:
        if(m_selectionModel->columnIntersectsSelection(1, m_selectionModel->currentIndex().parent())){
            menu.addAction(tr("Send IpAddress to OSINT"), this, [=](){this->action_send_selected_toEngine(ENGINE::OSINT, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
            menu.addAction(tr("Send IpAddress to RAW"), this, [=](){this->action_send_selected_toEngine(ENGINE::RAW, RESULT_TYPE::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
            menu.addSeparator();
            menu.addAction(tr("Send IpAddress to IP-Enum"), this, [=](){this->action_send_selected_toEnum(TOOL::IP);})->setIcon(QIcon(":/img/res/icons/ip.png"));
        }
        if(m_selectionModel->columnIntersectsSelection(0, m_selectionModel->currentIndex().parent())){
            menu.addAction(tr("Send Hostname to OSINT"), this, [=](){this->action_send_selected_toEngine(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname to RAW"), this, [=](){this->action_send_selected_toEngine(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname to BRUTE"), this, [=](){this->action_send_selected_toEngine(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname to ACTIVE"), this, [=](){this->action_send_selected_toEngine(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname to DNS"), this, [=](){this->action_send_selected_toEngine(ENGINE::DNS, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
            menu.addAction(tr("Send Hostname to SSL"), this, [=](){this->action_send_selected_toEngine(ENGINE::SSL, RESULT_TYPE::SUBDOMAIN);})->setIcon(QIcon(":/img/res/icons/domain.png"));
        }
        break;
    case ExplorerType::passive_Email:
        menu.addAction(tr("Send Email to OSINT"), this, [=](){this->action_send_selected_toEngine(ENGINE::OSINT, RESULT_TYPE::EMAIL);})->setIcon(QIcon(":/img/res/icons/email.png"));
        menu.addAction(tr("Send Email to RAW"), this, [=](){this->action_send_selected_toEngine(ENGINE::RAW, RESULT_TYPE::EMAIL);})->setIcon(QIcon(":/img/res/icons/email.png"));
        menu.addSeparator();
        menu.addAction(tr("Send Email to Email-Enum"), this, [=](){this->action_send_selected_toEnum(TOOL::EMAIL);})->setIcon(QIcon(":/img/res/icons/email.png"));
        break;
    case ExplorerType::passive_ASN:
        menu.addAction(tr("Send ASN to OSINT"), this, [=](){this->action_send_selected_toEngine(ENGINE::OSINT, RESULT_TYPE::ASN);})->setIcon(QIcon(":/img/res/icons/asn.png"));
        menu.addAction(tr("Send ASN to RAW"), this, [=](){this->action_send_selected_toEngine(ENGINE::RAW, RESULT_TYPE::ASN);})->setIcon(QIcon(":/img/res/icons/asn.png"));
        menu.addSeparator();
        menu.addAction(tr("Send ASN to ASN-Enum"), this, [=](){this->action_send_selected_toEnum(TOOL::ASN);})->setIcon(QIcon(":/img/res/icons/asn.png"));
        break;
    case ExplorerType::enum_IP:
    case ExplorerType::enum_ASN:
    case ExplorerType::enum_CIDR:
    case ExplorerType::enum_NS:
    case ExplorerType::enum_MX:
    case ExplorerType::enum_Email:
    case ExplorerType::enum_SSL:
    case ExplorerType::raw:
        break;
    }

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}

void Project::init_action_send(){

}

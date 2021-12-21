#include "Ssl.h"
#include "ui_Ssl.h"

#include <QClipboard>
#include <QDesktopServices>


void Ssl::m_initActions(){
    connect(&a_ClearResults, &QAction::triggered, this, [=](){this->m_clearResults();});
    connect(&a_RemoveResults, &QAction::triggered, this, [=](){this->m_removeResults(selectionModel);});
    connect(&a_ExpandResults, &QAction::triggered, this, [=](){this->m_expandResults();});
    connect(&a_CollapseResults, &QAction::triggered, this, [=](){this->m_collapseResults();});
    connect(&a_OpenInBrowser, &QAction::triggered, this, [=](){this->m_openInBrowser(selectionModel);});
    /* ... */
    connect(&a_SendAllCertToOsint, &QAction::triggered, this, [=](){this->m_sendCertToEngine(ENGINE::OSINT);});
    connect(&a_SendAllCertToRaw, &QAction::triggered, this, [=](){this->m_sendCertToEngine(ENGINE::RAW);});
    connect(&a_SendAllHostToOsint, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::OSINT);});
    connect(&a_SendAllHostToRaw, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::RAW);});
    connect(&a_SendAllHostToBrute, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::BRUTE);});
    connect(&a_SendAllHostToActive, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::ACTIVE);});
    connect(&a_SendAllHostToDns, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::DNS);});
    connect(&a_SendAllHostToCert, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::CERT);});
    connect(&a_SendAllCertToCertTool, &QAction::triggered, this, [=](){this->m_sendCertToTool(TOOL::CERT);});
    connect(&a_SendAllHostToCertTool, &QAction::triggered, this, [=](){this->m_sendSubdomainToTool(TOOL::CERT);});
    connect(&a_SendAllHostToDomainTool, &QAction::triggered, this, [=](){this->m_sendSubdomainToTool(TOOL::DOMAINTOOL);});
    /* ... */
    connect(&a_SendSelectedCertToOsint, &QAction::triggered, this, [=](){this->m_sendCertToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedCertToRaw, &QAction::triggered, this, [=](){this->m_sendCertToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedHostToOsint, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedHostToRaw, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedHostToBrute, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::BRUTE, selectionModel);});
    connect(&a_SendSelectedHostToActive, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::ACTIVE, selectionModel);});
    connect(&a_SendSelectedHostToDns, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::DNS, selectionModel);});
    connect(&a_SendSelectedHostToCert, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::CERT, selectionModel);});
    connect(&a_SendSelectedCertToCertTool, &QAction::triggered, this, [=](){this->m_sendCertToTool(TOOL::CERT, selectionModel);});
    connect(&a_SendSelectedHostToCertTool, &QAction::triggered, this, [=](){this->m_sendSubdomainToTool(TOOL::CERT, selectionModel);});
    connect(&a_SendSelectedHostToDomainTool, &QAction::triggered, this, [=](){this->m_sendSubdomainToTool(TOOL::DOMAINTOOL, selectionModel);});
    /* ... */
    connect(&a_Save, &QAction::triggered, this, [=](){this->m_saveResults(selectionModel);});
    connect(&a_Copy, &QAction::triggered, this, [=](){this->m_copyResults(selectionModel);});
}

void Ssl::on_buttonAction_clicked(){
    /* check if there are results available else dont show the context menu */
    if(m_resultProxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());

    /* creating the context menu... */
    QMenu *mainMenu = new QMenu(this);
    mainMenu->setAttribute(Qt::WA_DeleteOnClose, true);

    /* adding actions */
    mainMenu->addAction(&a_ClearResults);
    mainMenu->addSeparator();
    mainMenu->addAction(&a_Save);
    mainMenu->addAction(&a_Copy);
    mainMenu->addSeparator();

    switch (ui->comboBoxOutput->currentIndex()) {
    case ssl::OUTPUT::SUBDOMAIN:
        mainMenu->addAction(&a_SendAllHostToOsint);
        mainMenu->addAction(&a_SendAllHostToRaw);
        mainMenu->addAction(&a_SendAllHostToBrute);
        mainMenu->addAction(&a_SendAllHostToActive);
        mainMenu->addAction(&a_SendAllHostToDns);
        mainMenu->addAction(&a_SendAllHostToCert);
        mainMenu->addSeparator();
        mainMenu->addAction(&a_SendAllHostToCertTool);
        mainMenu->addAction(&a_SendAllHostToDomainTool);
        break;
    case ssl::OUTPUT::CERT_ID:
        mainMenu->addAction(&a_SendAllCertToOsint);
        mainMenu->addAction(&a_SendAllCertToRaw);
        mainMenu->addSeparator();
        mainMenu->addAction(&a_SendAllCertToCertTool);
        break;
    case ssl::OUTPUT::CERT_INFO:
        mainMenu->addAction(&a_ExpandResults);
        mainMenu->addAction(&a_CollapseResults);
    }

    /* showing the context menu... */
    mainMenu->exec(pos);
}

void Ssl::on_treeViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->treeViewResults->selectionModel()->isSelected(ui->treeViewResults->currentIndex()))
        return;

    /* getting the selected items... */
    selectionModel = ui->treeViewResults->selectionModel();

    /* creating the context menu... */
    QMenu *mainMenu = new QMenu(this);
    mainMenu->setAttribute(Qt::WA_DeleteOnClose, true);

    /* adding actions */
    mainMenu->addAction(&a_RemoveResults);
    mainMenu->addAction(&a_OpenInBrowser);
    mainMenu->addSeparator();
    mainMenu->addAction(&a_Save);
    mainMenu->addAction(&a_Copy);
    mainMenu->addSeparator();

    switch (ui->comboBoxOutput->currentIndex()) {
    case ssl::OUTPUT::SUBDOMAIN:
        mainMenu->addAction(&a_SendSelectedHostToOsint);
        mainMenu->addAction(&a_SendSelectedHostToRaw);
        mainMenu->addAction(&a_SendSelectedHostToBrute);
        mainMenu->addAction(&a_SendSelectedHostToActive);
        mainMenu->addAction(&a_SendSelectedHostToDns);
        mainMenu->addAction(&a_SendSelectedHostToCert);
        mainMenu->addSeparator();
        mainMenu->addAction(&a_SendSelectedHostToCertTool);
        mainMenu->addAction(&a_SendSelectedHostToDomainTool);
        break;
    case ssl::OUTPUT::CERT_ID:
        mainMenu->addAction(&a_SendSelectedCertToOsint);
        mainMenu->addAction(&a_SendSelectedCertToRaw);
        mainMenu->addSeparator();
        mainMenu->addAction(&a_SendSelectedCertToCertTool);
        break;
    case ssl::OUTPUT::CERT_INFO:
        /* nothing */
        break;
    }

    /* showing the context menu... */
    mainMenu->exec(QCursor::pos());
}

void Ssl::m_clearResults(){
    /* clear appropriate model */
    switch (ui->comboBoxOutput->currentIndex()) {
    case ssl::OUTPUT::SUBDOMAIN:
        m_resultModelSubdomain->clear();
        break;
    case ssl::OUTPUT::CERT_ID:
        m_resultModelCertId->clear();
        break;
    case ssl::OUTPUT::CERT_INFO:
        m_resultModelCertInfo->clear();
    }

    /* clear the filter and the result count */
    ui->lineEditFilter->clear();
    ui->labelResultsCount->clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void Ssl::m_expandResults(){
    ui->treeViewResults->expandAll();
}

void Ssl::m_collapseResults(){
    ui->treeViewResults->collapseAll();
}

void Ssl::m_openInBrowser(QItemSelectionModel *selectionModel){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void Ssl::m_removeResults(QItemSelectionModel *selectionModel){
    QStandardItemModel *model = nullptr;
    switch (ui->comboBoxOutput->currentIndex()) {
    case ssl::OUTPUT::SUBDOMAIN:
        model = m_resultModelSubdomain;
        break;
    case ssl::OUTPUT::CERT_ID:
        model = m_resultModelCertId;
        break;
    case ssl::OUTPUT::CERT_INFO:
        model = m_resultModelCertInfo;
    }

    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
        QModelIndex index = m_resultProxyModel->mapToSource(proxyIndex);
        model->removeRow(index.row());
    }
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}

void Ssl::m_saveResults(RESULT_TYPE resultType){
    /* checks... */
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen())
        return;

    /* writing to file */
    QString item;
    switch(resultType){
    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
        {
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
            file.write(item.toUtf8());
        }
        break;
    case RESULT_TYPE::CERT_ID:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
        {
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
            file.write(item.toUtf8());
        }
        break;
    case RESULT_TYPE::CERT_INFO:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
        {
            /* convert it to json */
            file.write(item.toUtf8());
        }
        break;

    default:
        break;
    }

    file.close();
}

void Ssl::m_saveResults(QItemSelectionModel *selectionModel){
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    /* saving to file */
    QString data;
    QString item;
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            item = index.data().toString();
            data.append(item.append(NEWLINE));
        }
        file.write(data.toUtf8());
        file.close();
    }
}


void Ssl::m_copyResults(RESULT_TYPE resultType){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QString item;

    /* copying result type */
    switch(resultType){
    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
        {
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    case RESULT_TYPE::CERT_ID:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
        {
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    case RESULT_TYPE::CERT_INFO:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
        {
            /* convert to json */
        }
        break;

    default:
        break;
    }

    clipboard->setText(clipboardData.trimmed());
}

void Ssl::m_copyResults(QItemSelectionModel *selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;
    QString item;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        item = index.data().toString();
        data.append(item.append(NEWLINE));
    }

    clipboard->setText(data.trimmed());
}

void Ssl::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::SUBDOMAIN){
        ui->targets->add(target);
    }

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

/*****************************************************************************
                            SENDING RESULTS
******************************************************************************/
void Ssl::m_sendSubdomainToEngine(ENGINE engine){
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToOsint(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToRaw(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToBrute(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToActive(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToDns(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToDns();
        break;
    case ENGINE::CERT:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToCert(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToCert();
        break;
    default:
        break;
    }
}

void Ssl::m_sendCertToEngine(ENGINE engine){
    QString item;

    switch (engine) {
    case ENGINE::OSINT:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 1)).toString();
            emit sendResultsToOsint(item, RESULT_TYPE::CERT_ID);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 1)).toString();
            emit sendResultsToRaw(item, RESULT_TYPE::CERT_ID);
        }
        emit changeTabToRaw();
        break;

    default:
        break;
    }
}

void Ssl::m_sendSubdomainToEngine(ENGINE engine, QItemSelectionModel *selection){
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToOsint(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToRaw(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToBrute(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToActive(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToDns(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToDns();
        break;
    case ENGINE::CERT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToCert(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToCert();
        break;
    default:
        break;
    }
}

void Ssl::m_sendCertToEngine(ENGINE engine, QItemSelectionModel *selection){
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToOsint(item, RESULT_TYPE::CERT_ID);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToRaw(item, RESULT_TYPE::CERT_ID);
        }
        emit changeTabToRaw();
        break;
    case ENGINE::IP:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToRaw(item, RESULT_TYPE::CERT_ID);
        }
        emit changeTabToRaw();
        break;

    default:
        break;
    }
}

void Ssl::m_sendSubdomainToTool(TOOL){
    /* not yet */
}

void Ssl::m_sendCertToTool(TOOL){
    /* not yet */
}

void Ssl::m_sendSubdomainToTool(TOOL, QItemSelectionModel *){
    /* not yet */
}

void Ssl::m_sendCertToTool(TOOL, QItemSelectionModel *){
    /* not yet */
}

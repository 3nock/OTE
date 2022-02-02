#include "ASNEnum.h"
#include "ui_ASNEnum.h"


void ASNEnum::m_clearResults(){
    /* clear the results... */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({"    ASN", "    Value"});
    m_resultsSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void ASNEnum::m_removeResults(QItemSelectionModel *selectionModel){
    /* loop to delete all selected items */
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes())
    {
        QModelIndex index = proxyModel->mapToSource(proxyIndex);

        /* remove entire asn */
        if(index.parent() == m_model->invisibleRootItem()->index()){
            m_resultsSet.remove(index.data().toString());
            m_model->removeRow(index.row());
        }
        /* remove a certain row in the asn item */
        else{
            m_model->removeRow(index.row());
        }
    }
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void ASNEnum::m_saveResults(){

}

void ASNEnum::m_saveResults(QItemSelectionModel *){

}

void ASNEnum::m_copyResults(){

}

void ASNEnum::m_copyResults(QItemSelectionModel *){

}

///
/// send all...
///

void ASNEnum::m_sendToProject(){

}

void ASNEnum::m_sendASNToEngine(ENGINE engine){
    QString item;

    switch (engine) {
    case ENGINE::OSINT:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            s3s_item::ASN *asn;
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            s3s_item::ASN *asn;
        }
        emit changeTabToRaw();
        break;

    default:
        break;
    }
}

void ASNEnum::m_sendCIDRToEngine(ENGINE engine){

}

void ASNEnum::m_sendASNToEnum(){

}

void ASNEnum::m_sendCIDRToEnum(){

}

///
/// send selected...
///

void ASNEnum::m_sendToProject(QItemSelectionModel *selectionModel){

}

void ASNEnum::m_sendASNToEngine(ENGINE engine, QItemSelectionModel *selectionModel){

}

void ASNEnum::m_sendCIDRToEngine(ENGINE engine, QItemSelectionModel *selectionModel){

}

void ASNEnum::m_sendASNToEnum(QItemSelectionModel *selectionModel){

}

void ASNEnum::m_sendCIDRToEnum(QItemSelectionModel *selectionModel){

}

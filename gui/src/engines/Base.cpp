#include "Base.h"

BaseClass::BaseClass(QWidget *parent, ResultsModel *resultsModel) : QWidget(parent),
    m_resultsModel(resultsModel)
{

}

/***************************************************************************
                            CONTEXT MENU
****************************************************************************/
void BaseClass::contextMenu_actionButton(ENGINE engineName, QPoint &pos){
    QMenu *contextMenu_save = new QMenu(this);
    QMenu *contextMenu_copy = new QMenu(this);
    QMenu *contextMenu_main = new QMenu(this);
    ///
    /// ...
    ///
    contextMenu_main->setAttribute( Qt::WA_DeleteOnClose, true );
    //...
    contextMenu_save->setTitle("Save");
    contextMenu_copy->setTitle("Copy");
    //...
    contextMenu_save->setObjectName("saveMenu");
    contextMenu_copy->setObjectName("saveMenu");
    contextMenu_main->setObjectName("rightClickMenu");
    ///
    /// ...
    ///
    QAction actionSaveAll("subdomain | ip");
    QAction actionSaveSubdomains("subdomains");
    QAction actionSaveIpAddresses("ip-addresses");
    //...
    QAction actionCopyAll("subdomain | ip");
    QAction actionCopySubdomains("subdomains");
    QAction actionCopyIpAddresses("ip-addresses");
    //...
    QAction actionSendToIp("Send ip-addresses to Ip");
    QAction actionSendToOsint("Send subdomains to Osint");
    QAction actionSendToActive("Send subdomains to Active");
    QAction actionSendToBrute("Send subdomains to Brute");
    QAction actionSendToSave("Send subdomains to Save");
    QAction actionSendToLevel("Send subdomains to Level");
    QAction actionSendToRecords("Send subdomains to Records");
    ///
    /// ...
    ///
    connect(&actionSaveSubdomains, &QAction::triggered, this, [=](){this->actionSave(engineName, CHOICE::susbdomains);});
    connect(&actionSaveIpAddresses, &QAction::triggered, this, [=](){this->actionSave(engineName, CHOICE::ipaddress);});
    connect(&actionSaveAll, &QAction::triggered, this, [=](){this->actionSave(engineName, CHOICE::all);});
    //...
    connect(&actionCopySubdomains, &QAction::triggered, this, [=](){this->actionCopy(engineName, CHOICE::susbdomains);});
    connect(&actionCopyIpAddresses, &QAction::triggered, this, [=](){this->actionCopy(engineName, CHOICE::ipaddress);});
    connect(&actionCopyAll, &QAction::triggered, this, [=](){this->actionCopy(engineName, CHOICE::all);});
    //...
    connect(&actionSendToIp, &QAction::triggered, this, [=](){this->actionSendToIp(engineName);});
    connect(&actionSendToOsint, &QAction::triggered, this, [=](){this->actionSendToOsint(engineName);});
    connect(&actionSendToSave, &QAction::triggered, this, [=](){this->actionSendToSave(engineName);});
    connect(&actionSendToBrute, &QAction::triggered, this, [=](){this->actionSendToBrute(engineName);});
    connect(&actionSendToActive, &QAction::triggered, this, [=](){this->actionSendToActive(engineName);});
    connect(&actionSendToRecords, &QAction::triggered, this, [=](){this->actionSendToRecords(engineName);});
    connect(&actionSendToLevel, &QAction::triggered, this, [=](){this->actionSendToLevel(engineName);});
    ///
    /// ...
    ///
    contextMenu_save->addAction(&actionSaveSubdomains);
    contextMenu_save->addAction(&actionSaveIpAddresses);
    contextMenu_save->addAction(&actionSaveAll);
    //...
    contextMenu_copy->addAction(&actionCopySubdomains);
    contextMenu_copy->addAction(&actionCopyIpAddresses);
    contextMenu_copy->addAction(&actionCopyAll);
    //...
    contextMenu_main->addMenu(contextMenu_copy);
    contextMenu_main->addMenu(contextMenu_save);
    contextMenu_main->addSeparator();
    contextMenu_main->addAction(&actionSendToIp);
    contextMenu_main->addSeparator();
    contextMenu_main->addAction(&actionSendToOsint);
    contextMenu_main->addAction(&actionSendToBrute);
    contextMenu_main->addAction(&actionSendToActive);
    contextMenu_main->addAction(&actionSendToRecords);
    contextMenu_main->addAction(&actionSendToLevel);
    contextMenu_main->addAction(&actionSendToSave);
    //...
    contextMenu_main->exec(QPoint(pos.x()+76, pos.y()));
}

void BaseClass::contextMenu_rightClick(QItemSelectionModel* selectionModel){
    QMenu *contextMenu_main = new QMenu(this);
    contextMenu_main->setAttribute( Qt::WA_DeleteOnClose, true );
    contextMenu_main->setObjectName("rightClickMenu");
    ///
    /// ...
    ///
    QAction actionSave("Save");
    QAction actionCopy("Copy");
    //...
    QAction actionOpenInBrowser("Open selected in Browser");
    QAction actionSendToOsint("Send selected to Osint");
    QAction actionSendToIp("Send selected to Ip");
    QAction actionSendToActive("Send selected to Active");
    QAction actionSendToBrute("Send selected to Brute");
    QAction actionSendToSave("Send selected to Save");
    QAction actionSendToLevel("Send selected to Level");
    QAction actionSendToRecords("Send selected to Records");
    ///
    /// ...
    ///
    connect(&actionSave, &QAction::triggered, this, [=](){this->cursorSave(selectionModel);});
    connect(&actionCopy, &QAction::triggered, this, [=](){this->cursorCopy(selectionModel);});
    //...
    connect(&actionOpenInBrowser, &QAction::triggered, this, [=](){this->cursorOpenInBrowser(selectionModel);});
    //...
    connect(&actionSendToOsint, &QAction::triggered, this, [=](){this->cursorSendToOsint(selectionModel);});
    connect(&actionSendToIp, &QAction::triggered, this, [=](){this->cursorSendToIp(selectionModel);});
    connect(&actionSendToSave, &QAction::triggered, this, [=](){this->cursorSendToSave(selectionModel);});
    connect(&actionSendToBrute, &QAction::triggered, this, [=](){this->cursorSendToBrute(selectionModel);});
    connect(&actionSendToActive, &QAction::triggered, this, [=](){this->cursorSendToActive(selectionModel);});
    connect(&actionSendToRecords, &QAction::triggered, this, [=](){this->cursorSendToRecords(selectionModel);});
    connect(&actionSendToLevel, &QAction::triggered, this, [=](){this->cursorSendToLevel(selectionModel);});
    ///
    /// ...
    ///
    contextMenu_main->addAction(&actionOpenInBrowser);
    contextMenu_main->addSeparator();
    contextMenu_main->addAction(&actionCopy);
    contextMenu_main->addAction(&actionSave);
    contextMenu_main->addSeparator();
    contextMenu_main->addAction(&actionSendToOsint);
    contextMenu_main->addAction(&actionSendToIp);
    contextMenu_main->addAction(&actionSendToBrute);
    contextMenu_main->addAction(&actionSendToActive);
    contextMenu_main->addAction(&actionSendToRecords);
    contextMenu_main->addAction(&actionSendToLevel);
    contextMenu_main->addAction(&actionSendToSave);
    ///
    /// ...
    ///
    contextMenu_main->exec(QCursor::pos());
}

/**************************** Open in Browser **************************/
void BaseClass::cursorOpenInBrowser(QItemSelectionModel *selectionModel){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
    }
}

/***************************** Send Results *****************************/
void BaseClass::actionSendToOsint(ENGINE engineName){
    emit a_sendToOsint(engineName);
    emit changeTabToOsint();
}
void BaseClass::actionSendToBrute(ENGINE engineName){
    emit a_sendToBrute(engineName);
    emit changeTabToBrute();
}
void BaseClass::actionSendToSave(ENGINE engineName){
    emit a_sendToSave(engineName);
    emit changeTabToSave();
}
void BaseClass::actionSendToRecords(ENGINE engineName){
    emit a_sendToRecord(engineName);
    emit changeTabToRecords();
}
void BaseClass::actionSendToIp(ENGINE engineName){
    emit a_sendToIp(engineName);
    emit changeTabToIp();
}
void BaseClass::actionSendToActive(ENGINE engineName){
    emit a_sendToActive(engineName);
    emit changeTabToActive();
}
void BaseClass::actionSendToLevel(ENGINE engineName){
    emit a_sendToLevel(engineName);
    emit changeTabToLevel();
}

//......
void BaseClass::cursorSendToOsint(QItemSelectionModel *selectionModel){
    emit c_sendToOsint(selectionModel);
    emit changeTabToOsint();
}
void BaseClass::cursorSendToActive(QItemSelectionModel *selectionModel){
    emit c_sendToActive(selectionModel);
    emit changeTabToActive();
}
void BaseClass::cursorSendToSave(QItemSelectionModel *selectionModel){
    emit c_sendToSave(selectionModel);
    emit changeTabToSave();
}
void BaseClass::cursorSendToRecords(QItemSelectionModel *selectionModel){
    emit c_sendToRecord(selectionModel);
    emit changeTabToRecords();
}
void BaseClass::cursorSendToIp(QItemSelectionModel *selectionModel){
    emit c_sendToIp(selectionModel);
    emit changeTabToIp();
}
void BaseClass::cursorSendToBrute(QItemSelectionModel *selectionModel){
    emit c_sendToBrute(selectionModel);
    emit changeTabToBrute();
}
void BaseClass::cursorSendToLevel(QItemSelectionModel *selectionModel){
    emit c_sendToLevel(selectionModel);
    emit changeTabToLevel();
}

/********************************** COPY & SAVE ************************************/

void BaseClass::actionSave(ENGINE engineName, CHOICE choice){
    QString filename = QFileDialog::getSaveFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(filename.isEmpty()){
        return;
    }
    //...
    QStandardItemModel *model;
    if(engineName == ENGINE::IP){
        model = m_resultsModel->ip;
    }
    if(engineName == ENGINE::BRUTE){
        model = m_resultsModel->brute;
    }
    if(engineName == ENGINE::LEVEL){
        model = m_resultsModel->level;
    }
    if(engineName == ENGINE::OSINT){
        model = m_resultsModel->osint;
    }
    if(engineName == ENGINE::ACTIVE){
        model = m_resultsModel->active;
    }
    if(engineName == ENGINE::RECORDS){
        model = m_resultsModel->record;
    }
    //...
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        if(choice == CHOICE::susbdomains){
            for(int i = 0; i != model->rowCount(); ++i){
                file.write((model->item(i, 0)->text()+NEWLINE).toUtf8());
            }
        }
        if(choice == CHOICE::ipaddress){
            for(int i = 0; i != model->rowCount(); ++i){
                file.write((model->item(i, 1)->text()+NEWLINE).toUtf8());
            }
        }
        if(choice == CHOICE::all){
            for(int i = 0; i != model->rowCount(); ++i){
                file.write((model->item(i, 0)->text()+":"+model->item(i, 1)->text()+NEWLINE).toUtf8());
            }
        }

        file.close();
    }
}
void BaseClass::actionCopy(ENGINE engineName, CHOICE choice){
    QStandardItemModel *model;
    if(engineName == ENGINE::IP){
        model = m_resultsModel->ip;
    }
    if(engineName == ENGINE::BRUTE){
        model = m_resultsModel->brute;
    }
    if(engineName == ENGINE::LEVEL){
        model = m_resultsModel->level;
    }
    if(engineName == ENGINE::OSINT){
        model = m_resultsModel->osint;
    }
    if(engineName == ENGINE::ACTIVE){
        model = m_resultsModel->active;
    }
    if(engineName == ENGINE::RECORDS){
        model = m_resultsModel->record;
    }
    ///
    /// ...
    ///
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data = "";
    if(choice == CHOICE::susbdomains){
        for(int i = 0; i != model->rowCount(); ++i){
            data.append(model->item(i, 0)->text()+NEWLINE);
        }
    }
    if(choice == CHOICE::ipaddress){
        for(int i = 0; i != model->rowCount(); ++i){
            data.append(model->item(i, 1)->text()+NEWLINE);
        }
    }
    if(choice == CHOICE::all){
        for(int i = 0; i != model->rowCount(); ++i){
            data.append(model->item(i, 0)->text()+"|"+model->item(i, 1)->text()+NEWLINE);
        }
    }
    clipboard->setText(data);
}

//......
void BaseClass::cursorSave(QItemSelectionModel *selectionModel){
    QString filename = QFileDialog::getSaveFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(filename.isEmpty()){
        return;
    }
    //...
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            file.write((index.data().toString()+NEWLINE).toUtf8());
        }
        file.close();
    }
}
void BaseClass::cursorCopy(QItemSelectionModel *selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data = "";
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        data.append(index.data().toString());
        data.append(NEWLINE);
    }
    clipboard->setText(data);
}

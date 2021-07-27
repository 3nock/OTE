#ifndef BASECLASS_H
#define BASECLASS_H

/*******************************************
                INCLUDES
********************************************/
#include "src/core.h"

/*******************************************
                STRUCTURES
********************************************/
struct Widgets{
    // targets widgets...
    QListWidget *listWidget_targets = nullptr;
    QLabel *label_targetsCount = nullptr;
    QLineEdit *lineEdit_targetInput = nullptr;
    // logs widgets...
    QListWidget *listWidget_logs = nullptr;
};
typedef Widgets Widgets;

/*******************************************
                 CLASS
********************************************/
class BaseClass : public QWidget {
        Q_OBJECT

    private:
        ResultsModel *m_resultsModel;
        QSet<QString> m_targetSet;

    public:
        Widgets *widgets;
        int activeThreads = 0;

    public:
        explicit BaseClass(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr);
        //...
        void contextMenu_actionButton(ENGINE engineName, QPoint &pos);
        void contextMenu_rightClick(QItemSelectionModel* selectionModel);
        //...
        void logs(QString log);
        //...
        void addTargets();
        void clearTargets();
        void removeTargets();
        void loadTargetsFromFile();

    signals:
        void stopScan();
        void pauseScan();
        //...
        void sendLog(QString log);
        void sendStatus(QString status);
        //...
        void a_sendToOsint(ENGINE engineName);
        void a_sendToActive(ENGINE engineName);
        void a_sendToBrute(ENGINE engineName);
        void a_sendToRecord(ENGINE engineName);
        void a_sendToIp(ENGINE engineName);
        void a_sendToLevel(ENGINE engineName);
        void a_sendToProject(ENGINE engineName);
        //...
        void c_sendToOsint(QItemSelectionModel* selectionModel);
        void c_sendToActive(QItemSelectionModel* selectionModel);
        void c_sendToBrute(QItemSelectionModel* selectionModel);
        void c_sendToRecord(QItemSelectionModel* selectionModel);
        void c_sendToIp(QItemSelectionModel* selectionModel);
        void c_sendToLevel(QItemSelectionModel* selectionModel);
        void c_sendToProject(QItemSelectionModel* selectionModel);
        //...
        void changeTabToOsint();
        void changeTabToBrute();
        void changeTabToActive();
        void changeTabToIp();
        void changeTabToRecords();
        void changeTabToLevel();
        void changeTabToProject();

    public slots:
        void a_receiveTargets(ENGINE engineName);
        void c_receiveTargets(QItemSelectionModel *selectionModel);

    private slots:
        void actionSendToOsint(ENGINE engineName);
        void actionSendToActive(ENGINE engineName);
        void actionSendToBrute(ENGINE engineName);
        void actionSendToLevel(ENGINE engineName);
        void actionSendToRecords(ENGINE engineName);
        void actionSendToIp(ENGINE engineName);
        //...
        void actionSave(ENGINE engineName, CHOICE);
        void actionCopy(ENGINE engineName, CHOICE);
        //...
        void cursorOpenInBrowser(QItemSelectionModel *selectionModel);
        void cursorSendToOsint(QItemSelectionModel *selectionModel);
        void cursorSendToActive(QItemSelectionModel *selectionModel);
        void cursorSendToRecords(QItemSelectionModel *selectionModel);
        void cursorSendToIp(QItemSelectionModel *selectionModel);
        void cursorSendToBrute(QItemSelectionModel *selectionModel);
        void cursorSendToLevel(QItemSelectionModel *selectionModel);
        //...
        void cursorSave(QItemSelectionModel *selectionModel);
        void cursorCopy(QItemSelectionModel *selectionModel);
};

#endif // BASECLASS_H

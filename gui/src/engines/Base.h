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
        ~BaseClass();
        //...
        void contextMenu_actionButton(ENGINE engineName, QPoint &pos);
        void contextMenu_rightClick(QItemSelectionModel*);
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
        void a_sendToOsint(ENGINE, CHOICE);
        void a_sendToActive(ENGINE, CHOICE);
        void a_sendToBrute(ENGINE, CHOICE);
        void a_sendToRecord(ENGINE, CHOICE);
        void a_sendToIp(ENGINE, CHOICE);
        void a_sendToLevel(ENGINE, CHOICE);
        void a_sendToProject(ENGINE, CHOICE);
        //...
        void c_sendToOsint(QItemSelectionModel*);
        void c_sendToActive(QItemSelectionModel*);
        void c_sendToBrute(QItemSelectionModel*);
        void c_sendToRecord(QItemSelectionModel*);
        void c_sendToIp(QItemSelectionModel*);
        void c_sendToLevel(QItemSelectionModel*);
        void c_sendToProject(QItemSelectionModel*);
        //...
        void changeTabToOsint();
        void changeTabToBrute();
        void changeTabToActive();
        void changeTabToIp();
        void changeTabToRecords();
        void changeTabToLevel();
        void changeTabToProject();

    public slots:
        void a_receiveTargets(ENGINE engineName, CHOICE);
        void c_receiveTargets(QItemSelectionModel*);

    private slots:
        void actionSendToOsint(ENGINE, CHOICE);
        void actionSendToActive(ENGINE, CHOICE);
        void actionSendToBrute(ENGINE, CHOICE);
        void actionSendToLevel(ENGINE, CHOICE);
        void actionSendToRecords(ENGINE, CHOICE);
        void actionSendToIp(ENGINE, CHOICE);
        //...
        void actionSave(ENGINE engineName, CHOICE);
        void actionCopy(ENGINE engineName, CHOICE);
        //...
        void cursorOpenInBrowser(QItemSelectionModel*);
        void cursorSendToOsint(QItemSelectionModel*);
        void cursorSendToActive(QItemSelectionModel*);
        void cursorSendToRecords(QItemSelectionModel*);
        void cursorSendToIp(QItemSelectionModel*);
        void cursorSendToBrute(QItemSelectionModel*);
        void cursorSendToLevel(QItemSelectionModel*);
        //...
        void cursorSave(QItemSelectionModel*);
        void cursorCopy(QItemSelectionModel*);
};

#endif // BASECLASS_H
